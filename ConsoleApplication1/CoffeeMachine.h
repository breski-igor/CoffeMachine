#pragma once
#include <string>
#include <vector>
#include <utility>

struct Product {
    std::string name;
    double price;
    int stock;
};

struct Coin {
    double value;
    int count;
};
class CoffeeMachine {
public:

    bool loadConfiguration(std::istream& inputStream);
	void showProducts() const;
    void showCoins() const;
    /*bool orderCoffee(const string& productName, double insertedAmount);
    void insertCoin(double coinValue);
    */

private:
    std::vector<Product> products;
    std::vector<Coin> coins;

};
