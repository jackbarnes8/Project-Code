#include <iostream>
#include <string>
#include <cstdlib>
#include "Bard.h"
#include "Entity.h"
#include "Player.h"
using namespace std;
extern int roll (int number, int sides);
extern int advroll (int number, int sides, int advantage);



// Constructor    
Bard::Bard(string playername) : Player(playername){
    
    HP = 25;                       // Current health of entity right now 
    maxHP = HP;                    // Health of entity at the start of the match
    baseDMG = 3;                   // Bard does 1d4+baseDMG
    startAC = 15;                  // AC at the start of the game
    AC = startAC;                  // Bard starts with an AC of 15
    type = "bard";                 // Type of the entity
    numFireball = 2;	    	   // Bard can throw 2 fireballs
    inspiration = 0;               // For the next 3 turns, player does an extra 1d5 damage and has advantage
    // 4 responses after a foe is hit with the player's insults
    insultHit.push_back("\nNot your best insult, but it got the point across.");
    insultHit.push_back("\nThat wasn't very nice of you to say that.");
    insultHit.push_back("\nYou hear noticeable gasp from... somewhere...");
    insultHit.push_back("\nAfter you finished your string of insults, you swear you could have saw a small tear fall from their cheek.");
}

// Deconstructor
Bard::~Bard(){
    
}

// Updates player and within the code any status effects (inspiration)
void Bard::updateStatus(){
    Player::updateStatus();
    if (inspiration == 1){
       cout << "\nYou are no longer inspired..." << endl;
   }
   inspiration = max(0,inspiration-1); // makes sure inspiration does not go below 0
}

// Actions

// For the next 3 turns, player does an extra 1d5 damage and gives player advantage
void Bard::useInspiration(){
    inspiration += 3;
    advantage += 3;
    cout << "You sing a song of valor. It feels with you determination! You will land hits and hit harder for the next 3 turns." << endl;
}

// Bard does 7 damage (this damage does not change, unless inspiration is used.
void Bard::fireball(Entity* victim){
    
    int roll2hit = advroll(1,20,advantage)+5;
    int dmg = 0;
    
    // check if the Bard hits the monster if roll2hit is less than the monster's AC the player misses, else, the player hits.
    if (roll2hit<victim->getAC()){
        cout << "\nYou threw a fire ball at the " << victim->getType() << " but you miss." << endl;    
    }else{
        //check if the bard has inspiration and change the damage accordingly
        if (inspiration > 0){
            dmg = 7+advroll(1,5,advantage);
            victim->alterHP(-dmg);
            cout << "\nYou threw a scorching fireball at the " << victim->getType() << " and do " << dmg << " hit points of damage! You may have burnt your fingertips..." << endl;
            cout << "                            ____" << endl;
            cout << "                     __,-~~/~    `---." << endl;
            cout << "                   _/_,---(      ,    )" << endl;
            cout << "               __ /        <    /   )  \\_\"__" << endl;
            cout << "- ------===;;;\'====------------------===;;;===----- -  -" << endl;
            cout << "                  \\/  ~\"~\"~\"~\"~\"~\\~\"~)~\"/\"" << endl;
            cout << "                  (_ (   \\  (     >    \\)" << endl;
            cout << "                   \\_( _ <         >_>'" << endl;
            cout << "                      ~ `-i\' ::>|--\"" << endl;
            cout << "                          I;|.|.|\"" << endl;
            cout << "                         <|i::|i|`." << endl;
            cout << "                        (` ^\'\"`-\' \")\"" << endl;
        }else{
            dmg = 7;
            victim->alterHP(-dmg);
            cout << "\nYou threw a fireball at the " << victim->getType() << " and do " << dmg << " hit points of damage!" << endl;
        }
    }
}

// Player enters an insult string and does 1d4+baseDMG damage and gives disadvantage for 3 turns
void Bard::attack(Entity* victim){
    
    int roll2hit = advroll(1,20,advantage)+7;
    int dmg = 0;
    // get player to input an insult
    string insult = "";
    cout << "\nRemembering your bard training, you know the best way to destory any foe, is with insults!\n\tWhat will you say?: ";
    // cin >> insult;
    cin.ignore();
    getline(cin, insult, '\n');
    
    
        if (roll2hit<victim->getAC()){
        cout << "\nYou open your mouth but no words come out... The " << victim->getType() << " smiles." << endl;    
    }else{
                   
        //check if the bard has inpiration and change the damage accordingly
        if (inspiration > 0){;
        	dmg = advroll(1,10,advantage)+baseDMG;
            victim->alterHP(-dmg);
            cout << insultHit[(dmg-baseDMG)-1] << "\nYou do " << dmg << " hit points of damage!" << endl;
        }else{
            dmg = advroll(1,4,advantage)+baseDMG;
            victim->alterHP(-dmg);
            cout << insultHit[(dmg-baseDMG)-1] << "\nYou do " << dmg << " hit points of damage!" << endl;
        }
    }
}
