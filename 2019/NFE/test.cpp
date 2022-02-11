//
// Created by User on 22/10/2019.
//

#include "Entity.h"
#include "Player.h"
#include "Warrior.h"
#include "Bard.h"
#include "Bugbear.h"
#include "EvilMage.h"

#include <iostream>
#include <vector>
#include <cassert>



using namespace std;
extern int roll(int, int);
extern int advroll(int, int, int);

vector<int> futurerolls = {2,100,6, //Warrior::attack()
                           100,6, //Warrior::raiseLowerShield()
                           100,6,5,100,6,5,100,6, //Warrior::increaseHuntersMark()
                           8,1,8,1,5, //Player::drinkPotion() and Player::hex
                           0,12, //Bard::fireball()
                           12,5,12,5,12, //Bard::useInspiration()
                           2,5,6, //EvilMage::attack()
                            //EvilMage::hex() requires no rolls
                           1,2,3,4, //EvilMage::createPotion()
                           1,3,1,1,1,1,2,1,3, //EvilMage::useRandomPotion()
                            //Bugbear::glare() requires no rolls
                            //Bugbear::flex() requires no rolls
                            //Bugbear::doNothing() requires no rolls
                           1,5,3,2,3,4, //Bugbear::attack()
                           };

int main() {
    
    //WARRIOR USER HAS 1 MILLION HEALTH AND 10 AC
    Warrior testwuser = Warrior("Buster");
    testwuser.setMaxHP(1000000); //sets maxHP to a million
    testwuser.alterHP(1000000-testwuser.getHP()); //sets HP to a million
    //Their baseDMG is still 4
    testwuser.setStartAC(10); //sets startAC to 10
    testwuser.alterAC(10-testwuser.getAC()); //sets AC to 10
    int expectedWUserHealth = 1000000; //subtract from this as you take damage
    
    //BUGBEAR FOE HAS 1 MILLION HEALTH AND 10 AC
    Bugbear testbfoe = Bugbear();
    testbfoe.setMaxHP(1000000); //sets maxHP to a million
    testbfoe.alterHP(1000000-testbfoe.getHP()); //sets HP to a million
    //Their baseDMG is still 4
    testbfoe.setStartAC(10); //sets startAC to 10
    testbfoe.alterAC(10-testbfoe.getAC()); //sets AC to 10
    int expectedBFoeHealth = 1000000; //subtract from this as you take damage
    
    //WARRIOR::ATTACK()
    testwuser.attack(&testbfoe); //ROLLS 2, so it misses
    /**/ assert(testbfoe.getHP() == expectedBFoeHealth);
    testwuser.attack(&testbfoe); //ROLLS 100, so it hits; ROLLS 6, so it does 6+4=11 damage
    /**/ expectedBFoeHealth -= 11;
    /**/ assert(testbfoe.getHP() == expectedBFoeHealth);
    
    //WARRIOR::RAISELOWERSHIELD()
    testwuser.raiseLowerShield();
    testwuser.attack(&testbfoe); //ROLLS 100, so it hits; ROLLS 6, so it does 6+4=10 damage
    /**/ expectedBFoeHealth -= 10;
    /**/ assert(testbfoe.getHP() == expectedBFoeHealth);
    testwuser.raiseLowerShield();
    
    //WARRIOR::INCREASEHUNTERSMARK()
    testwuser.increaseHuntersMark(); //sets huntersMark to 3
    testwuser.attack(&testbfoe); //ROLLS 100, so it hits; ROLLS 6, so it does 6+4+1=11 damage; ROLLS 5, so it does 5 extra damage;
    /**/ expectedBFoeHealth -= 11+5;
    /**/ assert(testbfoe.getHP() == expectedBFoeHealth);
    testwuser.updateStatus(); //huntersmark now down to 2
    testwuser.updateStatus(); //huntersmark now down to 1
    testwuser.attack(&testbfoe); //ROLLS 100, so it hits; ROLLS 6, so it does 6+4+1=11 damage; ROLLS 5, so it does 5 extra damage;
    /**/ expectedBFoeHealth -= 11+5;
    /**/ assert(testbfoe.getHP() == expectedBFoeHealth);
    testwuser.updateStatus(); //huntersmark now down to 0
    testwuser.attack(&testbfoe); //ROLLS 100, so it hits; ROLLS 6, so it does 6+4+1=11 damage
    /**/ expectedBFoeHealth -= 11;
    /**/ assert(testbfoe.getHP() == expectedBFoeHealth);
    
    //PLAYER::DRINKPOTION() AND PLAYER::HEX (for warrior)
    //Initial damage
    testwuser.alterHP(-20);
    /**/ expectedWUserHealth -= 20;
    /**/ assert(testwuser.getHP() == expectedWUserHealth);
    //Succeeds at healing
    testwuser.drinkPotion(); //ROLLS 8, so it heals 8+2=10; ROLLS 1, so you get flavour text 1
    /**/ expectedWUserHealth += 10;
    /**/ assert(testwuser.getHP() == expectedWUserHealth);
    //Fails due to hex, and later succeeds once it wears off
    testwuser.setHex(2);
    testwuser.drinkPotion(); //Fails to heal (or roll) due to hex
    /**/ assert(testwuser.getHP() == expectedWUserHealth);
    testwuser.updateStatus();
    testwuser.drinkPotion(); //Fails to heal (or roll) due to hex
    /**/ assert(testwuser.getHP() == expectedWUserHealth);
    testwuser.updateStatus();
    testwuser.drinkPotion(); //ROLLS 8, so it heals 8+2=10; ROLLS 1, so you get flavour text 1
    testwuser.updateStatus();
    /**/ expectedWUserHealth += 10;
    /**/ assert(testwuser.getHP() == expectedWUserHealth);
    //Says that it's unnecessary, because it already has max health
    testwuser.drinkPotion(); //ROLLS 5, so it heals 3+2=5, but fails because it's already at full health
    /**/ assert(testwuser.getPotions() == 0);
    //Fails because you're out of potions
    testwuser.drinkPotion();
    /**/ assert(testwuser.getHP() == expectedWUserHealth);
    /**/ assert(testwuser.getPotions() == 0);


    // -----------------------------------------------------------------------------------------------------------------

    
    //BARD USER HAS 1 MILLION HEALTH AND 10 AC
    Bard testbuser = Bard("Buster 2.0");
    testbuser.setMaxHP(1000000); //sets maxHP to a million
    testbuser.alterHP(1000000-testwuser.getHP()); //sets HP to a million
    //Their baseDMG is still 3
    testwuser.setStartAC(10); //sets startAC to 10
    testwuser.alterAC(10-testwuser.getAC()); //sets AC to 10
    int expectedBUserHealth = 1000000; //subtract from this as you take damage
    
    //TODO: BARD::ATTACK (WHICH TAKES PIPED INPUT)
    
    //BARD::FIREBALL()
    //You miss
    testbuser.fireball(&testbfoe); //ROLLS 0, so it misses
    /**/ assert(testbfoe.getHP() == expectedBFoeHealth);
    //You hit it with a fireball
    testbuser.fireball(&testbfoe); //ROLLS 12, so it hits, and deals 7 damage
    /**/ expectedBFoeHealth -= 7;
    /**/ assert(testbfoe.getHP() == expectedBFoeHealth);
    
    //BARD::INSPIRATION()
    //You get inspiration
    testbuser.useInspiration(); //Inspiration is now at 3
    //You throw a fireball and hit it
    testbuser.fireball(&testbfoe); //ROLLS 12, so it hits; ROLLS 5, so it deals 7+5=12 damage
    /**/ expectedBFoeHealth -= 12;
    /**/ assert(testbfoe.getHP() == expectedBFoeHealth);
    //Lose two of your three inspiration
    testbuser.updateStatus();
    testbuser.updateStatus();
    //You throw another fireball and still hit it with inspiration
    testbuser.fireball(&testbfoe); //ROLLS 12, so it hits; ROLLS 5, so it deals 7+5=12 damage
    /**/ expectedBFoeHealth -= 12;
    /**/ assert(testbfoe.getHP() == expectedBFoeHealth);
    //Lose your last inspiration
    testbuser.updateStatus();
    //Now without inspiration, you hit it with a fireball
    testbuser.fireball(&testbfoe); //ROLLS 12, so it hits, and deals 7 damage
    /**/ expectedBFoeHealth -= 7;
    /**/ assert(testbfoe.getHP() == expectedBFoeHealth);

    
    // -----------------------------------------------------------------------------------------------------------------
    

    //EVIL MAGE FOE HAS 1 MILLION HEALTH AND 10 AC
    EvilMage testmfoe = EvilMage();
    testmfoe.setMaxHP(1000000); //sets maxHP to a million
    testmfoe.alterHP(1000000-testmfoe.getHP()); //sets HP to a million
    //Their baseDMG is still 3
    testmfoe.setStartAC(10); //sets startAC to 10
    testmfoe.alterAC(10-testmfoe.getAC()); //sets AC to 10
    int expectedMFoeHealth = 1000000; //subtract from this as you take damage
    
    //EVILMAGE::ATTACK()
    testmfoe.attack(&testwuser); //ROLLS A 2, so it misses;
    /**/ assert(testwuser.getHP() == expectedWUserHealth);
    testmfoe.attack(&testwuser); //ROLLS A 5, so it hits; ROLLS A 6, so it does 6+3=9 damage, and sets testwuser's advantage to -1
    /**/ expectedWUserHealth -= 9;
    /**/ assert(testwuser.getHP() == expectedWUserHealth);
    /**/ assert(testwuser.getAdvantage() == -1);
    /**/ testwuser.updateStatus();
    
    //EVILMAGE::HEX()
    testmfoe.hex(&testwuser);
    /**/ assert(testwuser.getHex() == 3);
    testwuser.updateStatus();
    testwuser.updateStatus();
    testwuser.updateStatus();
    /**/ assert(testwuser.getHex() == 0);
    
    //EVILMAGE::CREATEPOTION()
    /**/ assert(testmfoe.getPotionCount() == 0);
    testmfoe.createPotion(); //ROLLS 1, creating a healing potion
    testmfoe.createPotion(); //ROLLS 2, creating a berzerkium potion
    testmfoe.createPotion(); //ROLLS 3, creating a weakness splash potion
    testmfoe.createPotion(); //ROLLS 4, creating an agiltiy potion
    /**/ assert(testmfoe.getPotionCount() == 4);
    
  //EVILMAGE::USERANDOMPOTION()
   //Healing potion
    testmfoe.alterHP(-10);
    /**/ expectedMFoeHealth -= 10;
    /**/ assert(testmfoe.getHP() == expectedMFoeHealth);
    testmfoe.useRandomPotion(&testwuser); //ROLLS 1, using heal; ROLLS 3, healing 9
    /**/ expectedMFoeHealth += 9;
    /**/ assert(testmfoe.getHP() == expectedMFoeHealth);
    
    //Berzerkium potion
    /**/ assert(testmfoe.getAdvantage() == 0);
    testmfoe.useRandomPotion(&testwuser); //ROLLS 1, using berz; ROLLS 1, giving it 2 adv
    /**/ assert(testmfoe.getAdvantage() == 2);
    testmfoe.updateStatus();
    testmfoe.updateStatus();
    /**/ assert(testmfoe.getAdvantage() == 0);
    
    //Weakness splash potion
    /**/ assert(testwuser.getAdvantage() == 0);
    testmfoe.useRandomPotion(&testwuser); //ROLLS 1, using weak; ROLLS 1, damaging you by 1; ROLLS 2, giving you -2 adv
    /**/ expectedWUserHealth -= 1;
    /**/ assert(testwuser.getHP() == expectedWUserHealth);
    /**/ assert(testwuser.getAdvantage() == -2);
    testwuser.updateStatus();
    testwuser.updateStatus();
    /**/ assert(testwuser.getAdvantage() == 0);
    
    //Agility potion
    /**/ assert(testmfoe.getAC() == 10);
    testmfoe.useRandomPotion(&testwuser); //ROLLS 1, using agil; ROLLS 3, increasing AC by that
    /**/ assert(testmfoe.getAC() == 13);
    testmfoe.updateStatus();
    testmfoe.updateStatus();
    testmfoe.updateStatus();
    /**/ assert(testmfoe.getAC() == 10);

    
    // -----------------------------------------------------------------------------------------------------------------
    
    
    //BUGBEAR::GLARE()
    /**/ assert(testwuser.getAdvantage() == 0);
    testbfoe.glare(&testwuser);
    /**/ assert(testwuser.getAdvantage() == -3);
    testwuser.updateStatus();
    testwuser.updateStatus();
    testwuser.updateStatus();
    /**/ assert(testwuser.getAdvantage() == 0);
    
    //BUGBEAR::FLEX()
    /**/ assert(testbfoe.getAdvantage() == 0);
    testbfoe.flex();
    /**/ assert(testbfoe.getAdvantage() == 3);
    testbfoe.updateStatus();
    testbfoe.updateStatus();
    testbfoe.updateStatus();
    /**/ assert(testbfoe.getAdvantage() == 0);
    
    //BUGBEAR::DONOTHING()
    testbfoe.doNothing();
    
    //BUGBEAR::ATTACK()
    testbfoe.attack(&testwuser); //ROLLS 1, so it misses
    /**/ assert(testwuser.getHP() == expectedWUserHealth);
    testbfoe.attack(&testwuser); //ROLLS 5, so it hits; ROLLS 3, so it does 3 attacks; ROLLS 2, so it does 2 damage; ROLLS 3, so it does 3 damage; ROLLS 4, so it does 4 damage
    /**/ expectedWUserHealth -= 2+3+4;
    /**/ assert(testwuser.getHP() == expectedWUserHealth);
    
    
    
    
    //TODO:
    // Bugbear::glare(Entity* victim)
    // Bugbear::flex()
    // Bugbear::doNothing()
    // Bugbear::attack() (both misses and hits)
    
    
    
    
    
    
    
    /*To test the mage:
     * Use the dummy players already in place, with like a million health
     * EvilMage::attack();
     * EvilMage::hex(); (just check that it works)
     * EvilMage::createPotion() and usePotion() - create all four potions, then use (and check) all four potions
     * */
    
    
    //DID: Got riggedroll() and the assert() systems working in a functional, non-buggy, easy-to-use, consistent way
    
    /*TODO: Test the following:
     * 
     * We'll probably want a dummy foe here, like a bugbear with a kpthwdfgillion health
     * 
     * */
    
    cout << endl;
    cout << "NOW FOR 2 DUMMY ROLLS (MEANT TO FAIL):" << endl;
    roll(1,3);
    advroll(1,3,4);
}