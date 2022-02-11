#include <iostream>
#include <string>
using namespace std;
extern int roll (int, int);



void flavourText(string category)
{
    string start_battle[3] = {"You feel like you're going to have a bad time.", "If you haven't heard his theme song, it's because it doesn't exist.", "It enjoys fresh meat and long walks on the beach."};
    string start_turn[7] = {"You stand ready.", "The air is thick with fog.", "Your foe picks his nose.", "Your neck hurts from looking up at this foe.", "The chill bites.", "You feel tense.", "You suddenly came to the realization that this foe is really ugly."};
    string miss[5] = {"You go for it..... and you miss.", "You hurt your dignity more than this monster as you miss.", "Your attack bounces off the beast's shoulder.", "The beast side steps your attack.", "Your aim is off and your attack hits the wall."};
    string heal[5] = {"It's possible you may have a drinking problem.", "You feel the effects course throughout your veins.", "You take out a bandage to patch up your wounds.", "The potion is vibrant red, but upon consuming it, it tastes like pineapple.", "You have no idea where these keep coming from."};
    string returnedString;

    //Selects the appropriate category
    if(category == "startBattle")
    {
        returnedString = start_battle[roll(1,3)-1];
    }
    else if(category == "startTurn")
    {
        returnedString = start_turn[roll(1,7)-1];
    }
    else if(category == "miss")
    {
        returnedString = miss[roll(1,5)-1];
    }
    else if(category == "heal")
    {
        returnedString = heal[roll(1,5)-1];
    }

    cout << returnedString << endl;
    return;
}