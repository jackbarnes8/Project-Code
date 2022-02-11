#include "Entity.h"
#include "Player.h"
#include <iostream>
#include <string>
extern int roll (int, int);
extern void flavourText(string);
using namespace std;


Player::Player(string myName) : Entity() {
	name = myName;              // Name of the player
    numPotions = 3;             // Number of healing potions the player has
};


//All "getX" will return X;
//All "setX" will replace X with a new value; These sets check if the values you give them are valid. Rather than returning false if they're not, they just throw an error via assert.
//All "alterX" will add a value to X;

int Player::getPotions(){
    return numPotions;
}

string Player::getName(){
	return name;
}

void Player::setPotions(int addPotion){
    numPotions += addPotion;
}

// Heals player by 3d4+2 HP
void Player::drinkPotion(){
    int heal = 0;
    //check if the player has any potions
    if (numPotions <= 0) {
        cout << "You have no potions left!" << endl;
    } else if (hex > 0) {
        cout << "A hex has been cast on you! You can't heal. " << hex << " turn(s) remaining." << endl;
    }else{
        heal = min(roll(3,4)+2, getMaxHP()-getHP()); 
        alterHP(heal);
        numPotions--;
        if(heal == 0) {
            cout << "You are already at max HP and healed for nothing." << endl;
        }else{
            cout << "You drink a health potion and heal " << heal << " points. ";
            flavourText("heal");
        }
        cout << "You have " << numPotions << " health potions left." << endl;
    }
}

void Player::updateStatus() {
    if (hex == 1) {
        cout << "Your hex wore off. You can heal again now." << endl;
    }
    Entity::updateStatus();
}
