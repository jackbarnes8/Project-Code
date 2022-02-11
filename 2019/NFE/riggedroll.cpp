#include <iostream>
#include <math.h>
#include <cstdlib>
#include <vector>
using namespace std;
extern vector<int> futurerolls;



int roll (int number, int sides)
{
    if (futurerolls.empty()) {
        cout << "ERROR! ERROR! ROLL IS OUT OF FUTUREROLLS. " << number << "d" << sides << "." << endl;
        return 1;
    } else {
        int result = futurerolls.at(0);
        cout << "   (Rolled " << result << ", for a " << number << "d" << sides << ")" << endl;
        futurerolls.erase(futurerolls.begin());
        return result;
    }    
}

int advroll (int number, int sides, int advantage) {
    if (futurerolls.empty()) {
        cout << "ERROR! ERROR! ADVROLL IS OUT OF FUTUREROLLS. " << number << "d" << sides << ", with adv " << advantage << "." << endl;
        return 1;
    } else {
        int result = futurerolls.at(0);
        cout << "   (Rolled " << result << ", for a " << number << "d" << sides << " with adv " << advantage << ")" << endl;
        futurerolls.erase(futurerolls.begin());
        return result;
    }
}