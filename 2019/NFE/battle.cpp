#include <iostream>
#include <string>
#include "Warrior.h"
#include "Entity.h"
#include "Player.h"
#include "Bugbear.h"
#include "EvilMage.h"
#include "Bard.h"
#include <cstdlib>
#include <unistd.h>
#include <math.h>
using namespace std;
extern int roll (int, int);
extern void flavourText(string);



int main() {

    cout << "It is recomended that you play in full screen. To play the game, please enter 1,2,3 or 4 unless instructed otherwise." << endl;
    cout << "Press enter to continue...";
    cin.ignore(); //TIP CAME FROM: https://stackoverflow.com/questions/42818899/detecting-enter-key-in-c
    usleep(500000);
    cout << "Are you ready?" << endl;
    usleep(500000);
    cout << "3" << endl; usleep(500000); cout << "2" << endl; usleep(500000); cout << "1" << endl; usleep(500000);

    cout << "  ███▄    █  █    ██  ██▓     ██▓         █████▒██▓  ▄████  ██░ ██ ▄▄▄█████▓▓█████  ██▀███     ▓█████ ▒██   ██▒ ▄████▄  ▓█████  ██▓███  ▄▄▄█████▓ ██▓ ▒█████   ███▄    █\n"; usleep(100000);
    cout << " ██ ▀█   █  ██  ▓██▒▓██▒    ▓██▒       ▓██   ▒▓██▒ ██▒ ▀█▒▓██░ ██▒▓  ██▒ ▓▒▓█   ▀ ▓██ ▒ ██▒   ▓█   ▀ ▒▒ █ █ ▒░▒██▀ ▀█  ▓█   ▀ ▓██░  ██▒▓  ██▒ ▓▒▓██▒▒██▒  ██▒ ██ ▀█   █ \n"; usleep(100000);
    cout << "▓██  ▀█ ██▒▓██  ▒██░▒██░    ▒██░       ▒████ ░▒██▒▒██░▄▄▄░▒██▀▀██░▒ ▓██░ ▒░▒███   ▓██ ░▄█ ▒   ▒███   ░░  █   ░▒▓█    ▄ ▒███   ▓██░ ██▓▒▒ ▓██░ ▒░▒██▒▒██░  ██▒▓██  ▀█ ██▒\n"; usleep(100000);
    cout << "▓██▒  ▐▌██▒▓▓█  ░██░▒██░    ▒██░       ░▓█▒  ░░██░░▓█  ██▓░▓█ ░██ ░ ▓██▓ ░ ▒▓█  ▄ ▒██▀▀█▄     ▒▓█  ▄  ░ █ █ ▒ ▒▓▓▄ ▄██▒▒▓█  ▄ ▒██▄█▓▒ ▒░ ▓██▓ ░ ░██░▒██   ██░▓██▒  ▐▌██▒\n"; usleep(100000);
    cout << "▒██░   ▓██░▒▒█████▓ ░██████▒░██████▒   ░▒█░   ░██░░▒▓███▀▒░▓█▒░██▓  ▒██▒ ░ ░▒████▒░██▓ ▒██▒   ░▒████▒▒██▒ ▒██▒▒ ▓███▀ ░░▒████▒▒██▒ ░  ░  ▒██▒ ░ ░██░░ ████▓▒░▒██░   ▓██░\n"; usleep(500000);
    cout << "░ ▒░   ▒ ▒ ░▒▓▒ ▒ ▒ ░ ▒░▓  ░░ ▒░▓  ░    ▒ ░   ░▓   ░▒   ▒  ▒ ░░▒░▒  ▒ ░░   ░░ ▒░ ░░ ▒▓ ░▒▓░   ░░ ▒░ ░▒▒ ░ ░▓ ░░ ░▒ ▒  ░░░ ▒░ ░▒▓▒░ ░  ░  ▒ ░░   ░▓  ░ ▒░▒░▒░ ░ ▒░   ▒ ▒ \n"; usleep(200000);
    cout << "░ ░░   ░ ▒░░░▒░ ░ ░ ░ ░ ▒  ░░ ░ ▒  ░    ░      ▒ ░  ░   ░  ▒ ░▒░ ░    ░     ░ ░  ░  ░▒ ░ ▒░    ░ ░  ░░░   ░▒ ░  ░  ▒    ░ ░  ░░▒ ░         ░     ▒ ░  ░ ▒ ▒░ ░ ░░   ░ ▒░\n"; usleep(200000);
    cout << "   ░   ░ ░  ░░░ ░ ░   ░ ░     ░ ░       ░ ░    ▒ ░░ ░   ░  ░  ░░ ░  ░         ░     ░░   ░       ░    ░    ░  ░           ░   ░░         ░       ▒ ░░ ░ ░ ▒     ░   ░ ░ \n"; usleep(200000);
    cout << "         ░    ░         ░  ░    ░  ░           ░        ░  ░  ░  ░            ░  ░   ░           ░  ░ ░    ░  ░ ░         ░  ░                   ░      ░ ░           ░ \n"; usleep(200000);

    //PROMPT PLAYER FOR NAME AND DECLARE PLAYER OBJECTS
    string* name = new string;//this was originally string name = "";
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, *name, '\n');
    cout << "\n\n";
    int playAgain = 1;

    //THE GAME LOOP
while(playAgain > 0) {
    Warrior userWarrior(*name);
    Bard userBard(*name);
    string choice; //The players attack choice. Possibly needs a better name?

    //GENERATES SEED.
    int namenumber = 1;
    for (int i = 0; i < name->size(); i++) {
        namenumber = (namenumber * int((*name).at(i))) % 5000 + 20;
    }
    srand(namenumber);

    //PROMPT PLAYER FOR CLASS CHOICE
    cout << "Choose your class: (1) for Warrior, (2) for Bard." << endl;
    int playerChoice;
    cin >> playerChoice;

    while(playerChoice != 1 && playerChoice != 2) {
        cout << "Please enter a valid choice." << endl;
        cout << "Choose your class: (1) for Warrior, (2) for Bard." << endl;
        cin >> playerChoice;
    }


    //PROMPT PLAYER FOR FOE CHOICE
    cout << "Choose your enemy: (1) for Bugbear, (2) for Evil Mage." << endl;
    int foeChoice;
    cin >> foeChoice; //1 will be bugbear, 2 will be evil mage.
    while(foeChoice != 1 && foeChoice != 2) {
        cout << "Please enter a valid choice." << endl;
        cout << "Choose your enemy: (1) for Bugbear, (2) for Evil Mage." << endl;
        cin >> foeChoice;
    }
    

    Bugbear foeBugbear = Bugbear();
    EvilMage foeEvilMage = EvilMage();
    int foeAttackChoice; //What the enemy will choose as its attack.




    //DISPLAY WHAT THE FOE IS
    switch(foeChoice) {
        case 1: cout << "A " << foeBugbear.getType() << " appears! "; flavourText("startBattle"); break;
        case 2: cout << "An " << foeEvilMage.getType() << " appears! "; flavourText("startBattle"); break;
    }



    //THE BATTLE LOOP
    while(userWarrior.getHP() != 0 && userBard.getHP() != 0) {

        flavourText("startTurn"); //flavour text to introduce the turn.
        //PRINT HP BARS
        if (1) { //put this in an if statement purely so that I can fold it away in the editor
            //PRINT YOUR HP BAR
            cout << "Your HP: ";
            if(playerChoice == 1) {
                for (int i = 0; i < userWarrior.getHP(); ++i) { //Print current HP.
                    cout << "█";
                }
                for (int i = 0; i < userWarrior.getMaxHP()-userWarrior.getHP(); ++i) { //Print remaining HP.
                    cout << "░";
                }
                cout << " " << userWarrior.getHP() << "/" << userWarrior.getMaxHP();
            } else if(playerChoice == 2) {
                for (int i = 0; i < userBard.getHP(); ++i) { //Print current HP.
                    cout << "█";
                }
                for (int i = 0; i < userBard.getMaxHP()-userBard.getHP(); ++i) { //Print remaining HP.
                    cout << "░";
                }
                cout << " " << userBard.getHP() << "/" << userBard.getMaxHP();
            }
            cout << "\t\t";
            //PRINT FOE HP BAR
            cout << "Enemy HP: ";
            if(foeChoice == 1) {
                for (int i = 0; i < foeBugbear.getHP(); ++i)
                {
                    cout << "█";
                }
                for (int i = 0; i < foeBugbear.getMaxHP()-foeBugbear.getHP(); ++i)
                {
                    cout << "░";
                }
                cout << " " << foeBugbear.getHP() << "/" << foeBugbear.getMaxHP();
                cout << endl;
            } else if(foeChoice == 2) {
                for (int i = 0; i < foeEvilMage.getHP(); ++i)
                {
                    cout << "█";
                }
                for (int i = 0; i < foeEvilMage.getMaxHP()-foeEvilMage.getHP(); ++i)
                {
                    cout << "░";
                }
                cout << " " << foeEvilMage.getHP() << "/" << foeEvilMage.getMaxHP();
                cout << endl;
            }
        }

        //PLAYER TURN
        if(playerChoice == 1) { //if Warrior.
            cout << "What will you do? The choice is at your fingertips...\n(1) = Attack\n(2) = Drink Potion\n(3) = Raise/Lower Shield\n(4) = Hunters Mark\n";
            cin >> choice; //Move choice.
            if(choice == "1"){
                switch(foeChoice) { //check what the foe is
                case 1: userWarrior.attack(&foeBugbear); break;
                case 2: userWarrior.attack(&foeEvilMage); break;
                }
            }else if(choice =="2"){
                userWarrior.drinkPotion();
                flavourText("heal");
            }else if (choice =="3"){
                userWarrior.raiseLowerShield(); //need to have a message
            }else if (choice =="4"){
                userWarrior.increaseHuntersMark(); //need to have a message
            } else {
                cout << "You hurt yourself in your confusion.\n"; userWarrior.alterHP(-1);
            }
            userWarrior.updateStatus();
        } else if (playerChoice == 2) { //If Bard.
            cout << "What will you do? The choice is at your fingertips...\n(1) = Attack\n(2) = Drink potion\n(3) = Fireball\n(4) = Inspiration\n";
            cin >> choice; //Move choice.
            if(choice == "1"){ //Normal attack
                switch(foeChoice) { //check what the foe is
                    case 1: userBard.attack(&foeBugbear); break;
                    case 2: userBard.attack(&foeEvilMage); break;
                }
            }else if(choice =="2"){ //Drink potion
                userBard.drinkPotion();
                flavourText("heal");
            }else if (choice =="3"){ //Fireball
                switch(foeChoice) { //check what the foe is
                    case 1: userBard.fireball(&foeBugbear); break;
                    case 2: userBard.fireball(&foeEvilMage); break;
                }
            }else if (choice =="4"){ //Use inspiration
                userBard.useInspiration();
            } else {
                cout << "You hurt yourself in your confusion.\n"; userBard.alterHP(-1);
            }
            userBard.updateStatus();
        }

        //CHECK FOE HP IS 0.
        if(foeBugbear.getHP() == 0 || foeEvilMage.getHP() == 0){ //Check for monsters HP, end loop if dead (0 HP)
            break;
        }
        usleep(500000);
        cout << endl;

        //MONSTERS TURN
        if(foeChoice == 1) { //If Bugbear.
            foeAttackChoice = roll(1,4);
            if(foeAttackChoice == 1) { //Normal attack.
                switch(playerChoice) { //check what the player is
                    case 1: foeBugbear.attack(&userWarrior); break;
                    case 2: foeBugbear.attack(&userBard); break;
                }
            } else if(foeAttackChoice == 2) { //Glare
                switch(playerChoice) { //check what the player is
                    case 1: foeBugbear.glare(&userWarrior); break;
                    case 2: foeBugbear.glare(&userBard); break;
                }
            } else if(foeAttackChoice == 3) { //Flex
                foeBugbear.flex();
            } else if(foeAttackChoice == 4) { //Do nothing
                foeBugbear.doNothing();
            }
            foeBugbear.updateStatus();
        } else if(foeChoice == 2) { //If Evil Mage.
            foeAttackChoice = roll(1,4);
            if(foeAttackChoice == 1) { //Normal attack.
                switch(playerChoice) {
                    case 1: foeEvilMage.attack(&userWarrior); break;
                    case 2: foeEvilMage.attack(&userBard); break;
                }
            } else if(foeAttackChoice == 2) { //Hex
                switch(playerChoice) {
                    case 1: foeEvilMage.hex(&userWarrior); break;
                    case 2: foeEvilMage.hex(&userBard); break;
                }
            } else if(foeAttackChoice == 3) { //Create potion
                foeEvilMage.createPotion();
            } else if(foeAttackChoice == 4) { //Use potion.
                if(foeEvilMage.getPotionCount() > 0) { //The mage will only use a potion if he has one.
                    switch(playerChoice) {
                        case 1: foeEvilMage.useRandomPotion(&userWarrior); break;
                        case 2: foeEvilMage.useRandomPotion(&userBard); break;
                        }
                    } else if(foeEvilMage.getPotionCount() == 0) { //if his potion count is 0, he creates one instead.
                        foeEvilMage.createPotion();
                }
            }
            foeEvilMage.updateStatus();
        }

        cout << "Turn Over.\n---------------------------------------------------------------------------------------------\n\n";
    }
        //PRINT WINNING/LOSING MESSAGE
    if (userWarrior.getHP() == 0 || userBard.getHP() == 0) {
        cout << "`YMM\'   `MM\' .g8\"\"8q. `7MMF\'   `7MF\'    `7MM\"\"\"Yb. `7MMF\'`7MM\"\"\"YMM  `7MM\"\"\"Yb.   " << endl; usleep(100000);
        cout << "  VMA   ,V .dP\'    `YM. MM       M        MM    `Yb. MM    MM    `7    MM    `Yb. " << endl; usleep(100000);
        cout << "   VMA ,V  dM\'      `MM MM       M        MM     `Mb MM    MM   d      MM     `Mb " << endl; usleep(100000);
        cout << "    VMMP   MM        MM MM       M        MM      MM MM    MMmmMM      MM      MM " << endl; usleep(100000);
        cout << "     MM    MM.      ,MP MM       M        MM     ,MP MM    MM   Y  ,   MM     ,MP " << endl; usleep(100000);
        cout << "     MM    `Mb.    ,dP\' YM.     ,M        MM    ,dP\' MM    MM     ,M   MM    ,dP\' " << endl; usleep(100000);
        cout << "   .JMML.    `\"bmmd\"\'    `bmmmmd\"\'      .JMMmmmdP\' .JMML..JMMmmmmMMM .JMMmmmdP\'   " << endl; usleep(100000);
    } else {
        cout << "▓██   ██▓ ▒█████   █    ██    ▓█████▄ ▓█████   █████▒▓█████ ▄▄▄     ▄▄▄█████▓▓█████ ▓█████▄    ▓██   ██▓ ▒█████   █    ██  ██▀███       █████▒▒█████  ▓█████  ▐██▌ " << endl; usleep(100000);
        cout << " ▒██  ██▒▒██▒  ██▒ ██  ▓██▒   ▒██▀ ██▌▓█   ▀ ▓██   ▒ ▓█   ▀▒████▄   ▓  ██▒ ▓▒▓█   ▀ ▒██▀ ██▌    ▒██  ██▒▒██▒  ██▒ ██  ▓██▒▓██ ▒ ██▒   ▓██   ▒▒██▒  ██▒▓█   ▀  ▐██▌ " << endl; usleep(100000);
        cout << "  ▒██ ██░▒██░  ██▒▓██  ▒██░   ░██   █▌▒███   ▒████ ░ ▒███  ▒██  ▀█▄ ▒ ▓██░ ▒░▒███   ░██   █▌     ▒██ ██░▒██░  ██▒▓██  ▒██░▓██ ░▄█ ▒   ▒████ ░▒██░  ██▒▒███    ▐██▌ " << endl; usleep(100000);
        cout << "  ░ ▐██▓░▒██   ██░▓▓█  ░██░   ░▓█▄   ▌▒▓█  ▄ ░▓█▒  ░ ▒▓█  ▄░██▄▄▄▄██░ ▓██▓ ░ ▒▓█  ▄ ░▓█▄   ▌     ░ ▐██▓░▒██   ██░▓▓█  ░██░▒██▀▀█▄     ░▓█▒  ░▒██   ██░▒▓█  ▄  ▓██▒ " << endl; usleep(100000);
        cout << "  ░ ██▒▓░░ ████▓▒░▒▒█████▓    ░▒████▓ ░▒████▒░▒█░    ░▒████▒▓█   ▓██▒ ▒██▒ ░ ░▒████▒░▒████▓      ░ ██▒▓░░ ████▓▒░▒▒█████▓ ░██▓ ▒██▒   ░▒█░   ░ ████▓▒░░▒████▒ ▒▄▄  " << endl; usleep(100000);
        cout << "   ██▒▒▒ ░ ▒░▒░▒░ ░▒▓▒ ▒ ▒     ▒▒▓  ▒ ░░ ▒░ ░ ▒ ░    ░░ ▒░ ░▒▒   ▓▒█░ ▒ ░░   ░░ ▒░ ░ ▒▒▓  ▒       ██▒▒▒ ░ ▒░▒░▒░ ░▒▓▒ ▒ ▒ ░ ▒▓ ░▒▓░    ▒ ░   ░ ▒░▒░▒░ ░░ ▒░ ░ ░▀▀▒ " << endl; usleep(500000);
        cout << " ▓██ ░▒░   ░ ▒ ▒░ ░░▒░ ░ ░     ░ ▒  ▒  ░ ░  ░ ░       ░ ░  ░ ▒   ▒▒ ░   ░     ░ ░  ░ ░ ▒  ▒     ▓██ ░▒░   ░ ▒ ▒░ ░░▒░ ░ ░   ░▒ ░ ▒░    ░       ░ ▒ ▒░  ░ ░  ░ ░  ░ " << endl; usleep(200000);
        cout << " ▒ ▒ ░░  ░ ░ ░ ▒   ░░░ ░ ░     ░ ░  ░    ░    ░ ░       ░    ░   ▒    ░         ░    ░ ░  ░     ▒ ▒ ░░  ░ ░ ░ ▒   ░░░ ░ ░   ░░   ░     ░ ░   ░ ░ ░ ▒     ░       ░ " << endl; usleep(200000);
        cout << " ░ ░         ░ ░     ░           ░       ░  ░           ░  ░     ░  ░           ░  ░   ░        ░ ░         ░ ░     ░        ░                   ░ ░     ░  ░ ░    " << endl; usleep(200000);
        cout << " ░ ░                           ░                                                     ░          ░ ░                                                               " << endl; usleep(200000);
    }

    cout << "Battle over. Play again? (1 = yes. 0 = no) \n" << endl;
    cin >> playAgain;
    while(playAgain != 1 && playAgain != 0) {
        cout << "Please enter a valid choice." << endl;
        cout << "Battle over. Play again? (1 = yes. 0 = no) \n" << endl;
        cin >> playAgain;
    }
}

    return 0;
}