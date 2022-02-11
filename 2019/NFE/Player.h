#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <string>
using namespace std;



class Player : public Entity
{
protected:
    int numPotions;                                 // Number of healing potions the player has
    string name;                                    // Birth name of the class. For player classes, this will be the user's name
public:
	Player(string myName);                          // Constructor
	int getPotions();                               // Returns the number of healing potions the player has
	string getName();
    void setPotions(int addPotion);                 // Change the number of healing potions
    void drinkPotion();                             // Heals player by 3d4+2 HP
    virtual void attack(Entity* victim) = 0;        // Basic attack that all entities have
    virtual void updateStatus();                    // Updates player and within the code any status effects (i.e. Hunter's mark)
};

#endif //PLAYER_H
