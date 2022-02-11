// This is the super class that all player and monster inherits from this.
#ifndef ENTITY_H
#define ENTITY_H

#include <string>
using namespace std;



class Entity {
protected:
    int HP;                    // Current health of entity
    int maxHP;                 // Maximum heal of Entity (HP should never go higher than maxHP)
    int baseDMG;               // Damage dealt by entity on top of the a random number
    int AC;                    // Chance of being hit (1 to 20, with 20 being a hit)
    int startAC;               // Chance of being hit at the start of the match (AC may change)
    int advantage;             // -ve values mean disadvantage, +ve values mean advantage
    string type;               // Type of the entity (i.e. Warrior or Bugbear)
    int hex;                   // How many turns you've got left that you're hexed (and can't heal)


public:
    Entity();    //Constructor

    //All "getX" will return X;
    //All "setX" will replace X with a new value; These sets check if the values you give them are valid. Rather than returning false if they're not, they just throw an error via assert.
    //All "alterX" will add a value to X;
    void alterHP(int myHP);
    int getHP();
    void setMaxHP(int myMaxHP);
    int getMaxHP();
    void setBaseDMG(int myBaseDMG);
    int getBaseDMG();
    void alterAC(int myAC);
    int getAC();
    void setStartAC(int myStartAC);
    int getStartAC();
    void setType(string myType);
    string getType();
    void setAdvantage(int myAdvantage);
    int getAdvantage();
    void setHex(int myHex);
    int getHex();
    virtual void attack(Entity* victim) = 0;    // all classes will have a basic attack that calls this function
    virtual void updateStatus();                // Updates player and within the code any status effects (i.e. Hunter's mark)
};

#endif //ENTITY_H
