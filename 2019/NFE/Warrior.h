#ifndef WARRIOR_H
#define WARRIOR_H
#include "Entity.h"
#include "Player.h"
#include <iostream>
#include <string>
using namespace std;


class Warrior : public Player {
private:
    bool shield;                    // If shield is raised = true, if shield is lowered = false
    int huntersMark;                // Number of turns that the hunter's mark as left each time it is used, 3 turns of hunter's mark is added
    
  public:
    Warrior(string playername);     // Constructor
    ~Warrior();                     // Deconstructor ***ENTITY SHOULD HAVE THE DECON
    
    // Actions
    void raiseLowerShield();        // Raise shield = AC goes to 18, damage goes to 1d8+baseDMG. Lower shield = AC goes to 16, damage goes to 1d10+baseDMG
    void increaseHuntersMark();     // For the next x turns, player does an extra 1d6 damage
    void updateStatus();
    void attack(Entity* victim);    // Hit victim with warhammer. If shield = true, damage is 1d8+4. If shield = false, damage is 1d10+4.
    
};

#endif //WARRIOR_H

