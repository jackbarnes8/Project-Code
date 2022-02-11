#include <iostream>
#include <string>
#include "Entity.h"
#include "Bugbear.h"
#include <unistd.h>
using namespace std;
extern int roll (int number, int sides);


//Constructor
Bugbear::Bugbear() : Entity() {
	HP = 27;
	maxHP = 27;
	baseDMG = 4;
	AC = 16;
	startAC = 16;
	type = "Bugbear";
}

// Deconstructor
Bugbear::~Bugbear() {

}

//ACTIONS

// Gives player disadvantage for 3 turns
void Bugbear::glare(Entity* victim){

    cout << "You suddenly feel cold as the Bugbear glares you down...\nYou will have a harder time landing hits like this." << endl;
    victim->setAdvantage(-3);

}

// Gives the bugbear advantage. This will print some flavour text and asceii art
void Bugbear::flex(){

    cout << "The bugbear stops what it is doing, you feel a chill down your spine... You get the feeling that something amazing is just about to happen.\n\n\n";
    usleep(1000000);
    cout << ".\n";
    usleep(2000000);
    cout << "..\n";
    usleep(2000000);
    cout << "...\n" << endl;
    usleep(2000000);
    cout << "______________$$$$$$$$$$____________________\n";
    cout << "_____________$$__$_____$$$$$________________\n";
    cout << "_____________$$_$$__$$____$$$$$$$$__________\n";
    cout << "____________$$_$$__$$$$$________$$$_________\n";
    cout << "___________$$_$$__$$__$$_$$$__$$__$$________\n";
    cout << "___________$$_$$__$__$$__$$$$$$$$__$$_______\n";
    cout << "____________$$$$$_$$_$$$_$$$$$$$$_$$$_______\n";
    cout << "_____________$$$$$$$$$$$$$_$$___$_$$$$______\n";
    cout << "________________$$_$$$______$$$$$_$$$$______\n";
    cout << "_________________$$$$_______$$$$$___$$$_____\n";
    cout << "___________________________$$_$$____$$$$____\n";
    cout << "___________________________$$_$$____$$$$$___\n";
    cout << "__________________________$$$$$_____$$$$$$__\n";
    cout << "_________________________$__$$_______$$$$$__\n";
    cout << "________________________$$$_$$________$$$$$_\n";
    cout << "________________________$$$___________$$$$$_\n";
    cout << "_________________$$$$___$$____________$$$$$$\n";
    cout << "__$$$$$$$$____$$$$$$$$$$_$____________$$$_$$\n";
    cout << "_$$$$$$$$$$$$$$$______$$$$$$$___$$____$$_$$$\n";
    cout << "$$________$$$$__________$_$$$___$$$_____$$$$\n";
    cout << "$$______$$$_____________$$$$$$$$$$$$$$$$$_$$\n";
    cout << "$$______$$_______________$$_$$$$$$$$$$$$$$$_\n";
    cout << "$$_____$_$$$$$__________$$$_$$$$$$$$$$$$$$$_\n";
    cout << "$$___$$$__$$$$$$$$$$$$$$$$$__$$$$$$$$$$$$$__\n";
    cout << "$$_$$$$_____$$$$$$$$$$$$________$$$$$$__$___\n";
    cout << "$$$$$$$$$$$$$$_________$$$$$______$$$$$$$___\n";
    cout << "$$$$_$$$$$______________$$$$$$$$$$$$$$$$____\n";
    cout << "$$__$$$$_____$$___________$$$$$$$$$$$$$_____\n";
    cout << "$$_$$$$$$$$$$$$____________$$$$$$$$$$_______\n";
    cout << "$$_$$$$$$$hg$$$____$$$$$$$$__$$$____________\n";
    cout << "$$$$__$$$$$$$$$$$$$$$$$$$$$$$$______________\n";
    cout << "$$_________$$$$$$$$$$$$$$$__________________\n\n";
    usleep(1000000);
    cout << "The bugbear flexes on you.\n The bugbear feels more confident. He will land attacks more easily now." << endl;
    advantage += 3;

}

// Bugbear does nothing but does print some flavour text
void Bugbear::doNothing()
{
    cout << "\t\t...\n The bugbear seems to have forgotten what it is doing." << endl;
}


// Basic attack for Bugbear. When the bugbear hits, it does 1-4 attacks on the player, dealing 2d4 damage each time
void Bugbear::attack(Entity* victim)
{
	int roll2hit = roll(1,20)+5;
    int dmg = 0;
    int totalDMG = 0;
    // check if the bugbear hits the monster if roll2hit is less than the monster's AC the player misses, else, the player hits.
     if (roll2hit<victim->getAC()){
         cout << "\nThe bugbear attacks you, but it misses." << endl;    
    }else{
        // run a for-loop for each of the bugbear's attacks
        int attackCount = roll(1,4);
        for (int i=0; i<attackCount; i++){
        	dmg = roll(2,4);
        	totalDMG += dmg;
        	cout << "The Bugbear attacks! Attack " << i+1 << " does " << dmg << " hit point(s) of damage!" << endl;
            usleep(500000);
        }
        victim->alterHP(-totalDMG);
        cout << "\nYou bleed out a total of " << totalDMG << " hit points of damage! -ouch. " << endl;
    }
}