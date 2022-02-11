#ifndef EVILMAGE_H
#define EVILMAGE_H
#include "Entity.h"
#include <string>
#include <vector>



class EvilMage : public Entity {

    private:
    vector<string> potionList;
public:
    //CONSTRUCTOR & DESTRUCTOR:
    EvilMage();
    ~EvilMage();

    //MOVES:
    void attack(Entity* victim);              //Deals damage and slight disadvantage
    void hex(Entity* victim);                 //Opponent can't heal for 3 turns
    void createPotion();                      //Creates a random potion (heal, adv, AC)
    void useRandomPotion(Entity* victim);     //Uses a random potion, throws an error if you don't have at least 1
    int getPotionCount();                     //Returns the number of potions it has

};


#endif //EVILMAGE_H
