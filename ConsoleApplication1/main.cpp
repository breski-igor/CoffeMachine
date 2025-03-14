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
        case 3: {
            std::cout << "Enter the name of the drink: ";
            std::string product;
            std::cin >> product;

            double insertedTotal = 0.0;
            double coin = 0.0;
            std::cout << "Insert money (enter 0 to finish):" << std::endl;
            while (true) {
                std::cout << "Insert a coin: ";
                std::cin >> coin;
                if (coin == 0)
                    break;
                insertedTotal += coin;
                machine.insertCoin(coin, insertedTotal);
                std::cout << "Total inserted: " << insertedTotal << " euros." << std::endl;
            }

            if (!machine.orderCoffee(product, insertedTotal)) {
                std::cout << "Transaction failed." << std::endl;
                
            }
            break;
        }
        case 0:
            std::cout << "Exiting..." << std::endl;
            break;
        default:
            std::cout << "Unknown option." << std::endl;
        }
    } while (choice != 0);

    return 0;
}
