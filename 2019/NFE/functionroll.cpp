#include <iostream>
#include <math.h>
#include <cstdlib>
using namespace std;
// These is a functions to roll x amount of dice with y sides and return the sum. 



//this function will only roll a dice (it is often used like a random number generator)
int roll (int number, int sides)
{
    int mydice = 0;
    // roll the dice number times in this for-loop
    for (int i=0; i<number; i++)
    {
        // add the random number to mydice + 1 so it never rolls a zero
        mydice += ceil(rand()%(sides))+1;
    }
    // return my dice
    return mydice;
}

//this function rolls dice and takes into account any advantage
int advroll (int number, int sides, int advantage) {
    int finalroll = 0;

    if (advantage > 0) {
        finalroll = max(roll(number, sides), roll(number, sides));
    } else if (advantage < 0) {
        finalroll = min(roll(number, sides), roll(number, sides));
    } else if (advantage == 0) {
        finalroll = roll(number, sides);
    }
    return finalroll;
}
