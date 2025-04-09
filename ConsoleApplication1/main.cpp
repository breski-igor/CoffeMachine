#include "CoffeeMachine.h"
#include <iostream>
#include <fstream>

void displayProducts(const std::vector<Product>& products) {
    std::cout << "Available products:" << std::endl;
    for (const auto& p : products) {
        std::cout << p.number << " - " << p.name
            << ", Price: " << p.price
            << " euros (Stock: " << p.stock << ")" << std::endl;
    }
}

void displayCoins(const std::vector<Coin>& coins) {
    std::cout << "Coin stock:" << std::endl;
    int total = 0;
    for (const auto& c : coins) {
        std::cout << "Value: " << c.value << " eur, Amount: " << c.count << std::endl;
        total += c.value * c.count;
    }
    std::cout << "Total value: " << total << " eur." << std::endl;
}

void order(CoffeeMachine& machine) {
        displayProducts(machine.getProducts());
        int number;
        bool validInput = false;

        do {
            std::cout << "Enter the number of the drink or 0 to exit: ";
            std::cin >> number;

            if (std::cin.fail() || (number != 0 && number != 1 && number != 2 && number != 3)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Enter the number of the drink or 0 to exit : \n";
            }
            else {
                validInput = true;
            }
        } while (!validInput);

        if (number == 0) {
            std::cout << "Exiting.\n";
            return;
        }

        int insertedTotal = 0;
        int coin = 0;
        std::cout << "Insert money (enter 0 to finish):" << std::endl;
        while (true) {
            std::cout << "Insert a coin: ";
            std::cin >> coin;
            if (coin == 0) break;
            if (machine.insertCoin(coin, insertedTotal)) {
                std::cout << "Total inserted: " << insertedTotal << " euros." << std::endl;
            }
            else {
                std::cout << "Invalid coin. Please try again." << std::endl;
            }
        }

        OrderStatus status = machine.orderCoffee(number, insertedTotal);
        switch (status) {
        case OrderStatus::SUCCESS:
            std::cout << "Coffee ordered successfully!" << std::endl;
            break;
        case OrderStatus::PRODUCT_NOT_FOUND:
            std::cout << "Product not found." << std::endl;
            break;
        case OrderStatus::OUT_OF_STOCK:
            std::cout << "Product out of stock." << std::endl;
            break;
        case OrderStatus::INSUFFICIENT_FUNDS:
            std::cout << "Insufficient funds." << std::endl;
            break;
        case OrderStatus::CANNOT_PROVIDE_CHANGE:
            std::cout << "Machine cannot provide change. Refunding money." << std::endl;
            break;
        }
    }
        

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " <XMLFile.xml>" << std::endl;
		return 1;
	}

    CoffeeMachine machine;
    std::ifstream configFile(argv[1]);

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
            displayProducts(machine.getProducts());
            break;
        case 2:
            displayCoins(machine.getCoins());
            break;
        case 3: 
			order(machine);
			break;
        
        case 0:
			machine.saveConfiguration(argv[1]);
            std::cout << "Exiting." << std::endl;
            break;
        default:
            std::cout << "Unknown." << std::endl;
        }
    } while (choice != 0);

    return 0;
}