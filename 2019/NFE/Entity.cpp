#include "Entity.h"
#include <assert.h>
#include <algorithm>



Entity::Entity() {
//rubbish numbers so we know when something is not working
    //HP = 69;
    maxHP = -69;
    baseDMG = -42069;
    AC = -360.0;
    startAC = -1000.0;
    type = "missingno";
    advantage = 0;
    hex = 0;
};

//To be run each turn, decreasing any effects on the Entity (AC moves back to startAC, hex moves back to 0 and advantage moves back to 0)
void Entity::updateStatus() {
    if (AC < startAC)  {AC++;};
    if (AC > startAC)  {AC--;};
    if (hex > 0)       {hex--;};
    if (advantage > 0) {advantage--;};
    if (advantage < 0) {advantage++;};
}

//All "getX" will return X;
//All "setX" will replace X with a new value; These sets check if the values you give them are valid. Rather than returning false if they're not, they just throw an error via assert.
//All "alterX" will add a value to X;
void Entity::alterHP(int myHP) {
    HP = min(max(0,HP+myHP),maxHP); //clamps HP+myHP to be between 0 and maxHP
};

void Entity::setMaxHP(int myMaxHP) {
    assert(myMaxHP >= 0);
    maxHP = myMaxHP;
};

void Entity::setBaseDMG(int myBaseDMG) {
    assert(myBaseDMG >= 0);
    baseDMG = myBaseDMG;
};

void Entity::alterAC(int myAC) {
    AC += myAC; //can be more than the start, and can be negative
};

void Entity::setStartAC(int myStartAC) {
    startAC = myStartAC;
};

void Entity::setType(string myType) {
    assert(myType != "" && myType != "missingno");
    type = myType;
};

void Entity::setAdvantage(int myAdvantage) {
    advantage = myAdvantage;
};

void Entity::setHex(int myHex) {
    hex = myHex;
}

int Entity::getHP() {
    return HP;
};
int Entity::getMaxHP() {
    return maxHP;
};
int Entity::getBaseDMG() {
    return baseDMG;
};
int Entity::getAC() {
    return AC;
};
int Entity::getStartAC() {
    return startAC;
};
string Entity::getType() {
    return type;
};
int Entity::getAdvantage() {
    return advantage;
};
int Entity::getHex() {
    return hex;
}
