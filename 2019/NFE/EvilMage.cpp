#include "EvilMage.h"
#include <iostream>
#include <unistd.h>
#include <assert.h>
using namespace std;
extern int roll (int number, int sides);
extern int advroll(int number, int sides, int advantage);



EvilMage::EvilMage() : Entity() {
    maxHP = 20;
    HP = maxHP;
    baseDMG = 3;
    AC = 12;
    startAC = 12;
    type = "evil mage";
    potionList = {}; // a vector of potions that the evil mage has created and can pull from
}

// Deals damage and disadvantage for one turn
void EvilMage::attack(Entity* victim) {
    
    int roll2hit = roll(1,20)+5;
    //Roll to check if the evil mage hits. If roll to hit is less than the player's AC, the attack misses, else, it hits
    if (roll2hit<victim->getAC()){
        cout << "\nThe evil mage attacks you, but it misses." << endl;
    }else{
        int dmg = advroll(1,7,advantage)+baseDMG;
        cout << "The evil mage fires a magical spell at you! It does " << dmg << " hit point(s) of damage. Your vision blurrs and you feel a bit dizzy! \n It will be harder to hit the evil mage for the next turn." << endl;
        victim->alterHP(-dmg);
        victim->setAdvantage(-1);  //Also removes any 'advantage' effects you may have
        usleep(500000);
    }
};

//Casts a three-turn hex on the opponent
void EvilMage::hex(Entity* victim) {
    victim->setHex(3);
    cout << "The evil mage casts a hex on you! You find yourself unable to heal for the next 3 turns..." << endl;
    usleep(500000);
};

//Creates a random potion (heal, adv, AC)
void EvilMage::createPotion() {
    switch (roll(1,4)) {
        case 1:
            potionList.push_back("heal");
            cout << "The evil mage creates a healing potion, that it can drink later." << endl;
            break;
        case 2:
            potionList.push_back("berz");
            cout << "The evil mage creates a berzerkium potion, that it can drink later to increase its attack strength." << endl;
            break; //this would just give it advantage
        case 3:
            potionList.push_back("weak");
            cout << "The evil mage creates a weakness splash potion, that it can throw at you later to decrease your attack." << endl; //this would just give you disadvantage
            break;
        case 4:
            potionList.push_back("agil");
            cout << "The evil mage creates an agility potion, that it can drink later to make it harder to hit." << endl;
            break;
    }
};

//Uses a random potion, throws an error if you don't have at least 1
void EvilMage::useRandomPotion(Entity* victim) {

    assert(getPotionCount()); //fails if it's 0

    int potionInt = roll(1,getPotionCount())-1;
    string potionString = potionList.at(potionInt);
    potionList.erase (potionList.begin() + potionInt);

    if (potionString == "heal") {
        int healAmount = roll(1,4)+6;
        alterHP(healAmount);
        cout << "The evil mage drinks a healing potion, healing it " << healAmount << " hit points!" << endl;

    } else if (potionString == "berz") {
        int advantageIncreaseAmount = roll(1,3)+1;
        setAdvantage(advantageIncreaseAmount);
        cout << "The evil mage drinks a berzerkium potion! Its attacks will be more powerful for the next " << advantageIncreaseAmount << " turns." << endl;

    } else if (potionString == "weak") {
        int damageAmount = advroll(1,2,advantage);
        int disadvantageLength = roll(1,3);
        victim -> alterHP(-damageAmount);
        victim -> setAdvantage(-disadvantageLength);
        cout << "The evil mage throws a weakness splash potion at you! The glass shatters on your skin, dealing you " << damageAmount << " damage, and making you weaker for " << disadvantageLength << " turns." << endl;

    } else if (potionString == "agil") {
        int ACIncreaseAmount = roll(1,4);
        alterAC(ACIncreaseAmount);
        cout << "The evil mage drinks an agility potion, which makes it harder to hit for the next " << ACIncreaseAmount << " turns." << endl;

    } else {
        cout << "The evil mage attempts to use a potion that doesn't exist! And it destroys the very fabric of this reality! It crashes the game. This should never happen in normal gameplay (i.e. there's a bug somewhere)." << endl;
        assert(0);

    }
};

int EvilMage::getPotionCount() {
    return potionList.size();
};

EvilMage::~EvilMage() {

}
