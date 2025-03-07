#include "CoffeeMachine.h"
#include <iostream>
#include <fstream>

int main() {
    CoffeeMachine machine;
    std::ifstream configFile("XMLFile.xml");

    if (!configFile) {
        std::cout << "Cannot open the XML file." << std::endl;
        return 1;
    }

    if (!machine.loadConfiguration(configFile)) {
        std::cout << "Error loading configuration." << std::endl;
        return 1;
    }

    std::cout << "Configuration successfull." << std::endl;
    return 0;
}
