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

    configFile.close();
    
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
        case 3: {
            std::cout << "1 - Espresso\n";
            std::cout << "2 - Cappuccino\n";
            std::cout << "3 - Latte\n";
            std::cout << "Enter the number of the drink: ";
            int number;
            std::cin >> number;

            int insertedTotal = 0;
            int coin = 0.0;
            std::cout << "Insert money (enter 0 to finish):" << std::endl;
            while (true) {
                std::cout << "Insert a coin: ";
                std::cin >> coin;
                if (coin == 0)
                    break;
                machine.insertCoin(coin, insertedTotal);
                std::cout << "Total inserted: " << insertedTotal << " euros." << std::endl;
            }

            if (!machine.orderCoffee(number, insertedTotal)) {
                std::cout << "Transaction failed." << std::endl;
                
            }
            break;
        }
        case 0:
            std::cout << "Exiting." << std::endl;
            break;
        default:
            std::cout << "Unknown." << std::endl;
        }
    } while (choice != 0);

    return 0;
}
