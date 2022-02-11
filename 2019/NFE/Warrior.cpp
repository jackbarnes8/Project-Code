#include <iostream>
#include <string>
#include <cstdlib>
#include "Warrior.h"
#include "Entity.h"
#include "Player.h"
using namespace std;
extern int roll (int number, int sides);
extern int advroll (int number, int sides, int advantage);



// Constructor    
Warrior::Warrior(string playername) : Player(playername){

    HP = 31;                       // Current health of entity right now 
    maxHP = 31;                    // Health of entity at the start of the match
    baseDMG = 4;                   // Warrior does 1d8+4 (shield = true) or 1d10+4 (shield = false)
    AC = 18;                       // Warrior starts with an AC of 16 however when shield is true "raised", AC is 16
    startAC = 18;                  // AC at the start of the game
    type = "warrior";              // Type of the entity
    huntersMark = 0;               // Number of turns that the hunter's mark as left each time it is used, 3 turns of hunter's mark is added
    shield = false;
}

// Deconstructor
Warrior::~Warrior(){
    
}

// Updates both the player and within the code any status effects (Hunter's mark)
void Warrior::updateStatus(){
    Player::updateStatus();
    if (huntersMark == 1){
       cout << "Your hunter's mark has worn off..." << endl;
   }
   huntersMark = max(0,huntersMark-1); // makes sure hunter's mark does not go below 0
}

// Actions

// Raise shield = AC goes to 18, damage goes to 1d8+baseDMG. Lower shield = AC goes to 16, damage goes to 1d10+baseDMG
void Warrior::raiseLowerShield(){
    //check if shield is raised or lowered (true or false)
    shield = not shield;
    if (shield == true)
    {
        cout << "You valiantly raise your shield. Your defense increases, but your attack suffers." << endl;
    } else {
        cout << "You have lowered your shield. Your attack increases, but your defense suffers." << endl;
    }
}

// For the next 3 turns, player does an extra 1d6 damage. This can stack
void Warrior::increaseHuntersMark(){
    huntersMark += 3;
    cout << "You have increased your Hunter's Mark! You feel magic through your fingertips and your damage output has increased." << endl;
}


// Hit monster with warhammer. if shield = true, damage is 1d8+4. If shield = false, damage is 1d10+4.
void Warrior::attack(Entity* victim){
    int roll2hit = roll(1,20)+5;
    int dmg = 0;
    // check if the warrior hits the monster if roll2hit is less than the monster's AC the player misses, else, the player hits.
    if (roll2hit<victim->getAC()){
        cout << "\nYou attacked the " << victim->getType() << " but you miss." << endl;    
    }else{
        //check if the shield is raised and change the damage accordingly
        if (shield){
            dmg = advroll(1,8,advantage)+baseDMG;
            if (huntersMark > 0) {dmg += advroll(1,6,advantage);}
            victim->alterHP(-dmg);
            cout << "\nYou attacked the " << victim->getType() << " with your shield up and do " << dmg << " hit points of damage!" << endl;
        }else{
            dmg = advroll(1,10,advantage)+baseDMG+1;
            if (huntersMark > 0) {dmg += advroll(1,6,advantage);}
            victim->alterHP(-dmg);
            cout << "\nYou attacked the " << victim->getType() << " and do " << dmg << " hit points of damage!" << endl;
        }
    }
}

