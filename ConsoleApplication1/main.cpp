#include "CoffeeMachine.h"
#include <iostream>
using namespace std;

int main() {
    CoffeeMachine machine;
    const string configFile = "XMLFile.xml";
    if (!machine.loadConfiguration(configFile)) {
        cout << "Greška pri učitavanju konfiguracije." << endl;
        return 1;
    }

}
