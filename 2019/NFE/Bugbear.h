#ifndef BUGBEAR_H
#define BUGBEAR_H
#include "Entity.h"
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;



class Bugbear : public Entity {

    public:
    Bugbear();                      // Constructor
    void attack(Entity* victim);    // Basic attack. Bugbear does a multiattack with lots of little hits
    ~Bugbear();                     // Deconstructor
    void glare(Entity* victim);     // Gives player disadvantage
    void flex();                    // Gives the bugbear advantage
    void doNothing();               // Bugbear does nothing but does print some flavour text
};

#endif //BUGBEAR_H