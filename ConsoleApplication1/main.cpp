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

    int choice = 0;
    do {
        std::cout << "\nChoose an option:" << std::endl;
        std::cout << "1. Show product stock" << std::endl;
        std::cout << "2. Show coin stock" << std::endl;
        std::cout << "3. Order a drink" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cin >> choice;

        switch (choice) {
        case 1:
            machine.showProducts();
            break;
        case 2:
            machine.showCoins();
            break;
        }
    } while (choice != 0);



    return 0;
}
