#ifndef BARD_H
#define BARD_H
#include "Entity.h"
#include "Player.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;



class Bard : public Player {
private:
    
    int numFireball;		        	 // Bard can throw 2 fireballs
    int inspiration;                     // For the next 3 turns, player does an extra 1d5 damage
    vector<string> insultHit;            // 4 responses after a foe is hit with the player's insults
    
    public:
    Bard(string playername);             // Constructor
    ~Bard();                             // Deconstructor    
    void updateStatus();                 // Updates player and within the code any status effects (inspiration)
    
    // Actions
    void fireball(Entity* victim);            // Bard does 10 damage (this damage does not change)
    void attack(Entity* victim);              // Player enters an insult string and does 1d4+3 damage and gives disadvantage for 3 turns
    void useInspiration();                    // For the next 3 turns, player does an extra 1d5 damage and gives player advantage
    
};

#endif //BARD_H

