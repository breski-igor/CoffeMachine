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
    bool orderCoffee(const std::string& productName, double insertedAmount);
    bool insertCoin(double coinValue, double& insertedTotal);
    bool saveConfiguration(const std::string& filename) const;

    

private:
    std::vector<Product> products;
    std::vector<Coin> coins;
    bool calculateChange(double change, std::vector<std::pair<double, int>>& changeCoins);
};


