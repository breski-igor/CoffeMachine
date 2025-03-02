#pragma once
#include <string>
#include <vector>
#include <utility>

using namespace std;
struct Product {
    string name;
    double price;
    int stock;
};

struct Coin {
    double value;
    int count;
};
class CoffeeMachine {
public:

    bool loadConfiguration(const string& filename);
	/*
    void showProducts() const;
    void showCoins() const;
    bool orderCoffee(const string& productName, double insertedAmount);
    void insertCoin(double coinValue);
    */

private:
    vector<Product> products;
    vector<Coin> coins;

};
