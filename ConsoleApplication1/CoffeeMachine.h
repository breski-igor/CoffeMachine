#pragma once
#include <string>
#include <vector>
#include <utility>
#include <iosfwd>

struct Product {
    int number;
    std::string name;
    int price;
    int stock;
};

struct Coin {
    int value;
    int count;
};

class CoffeeMachine {
public:
    bool loadConfiguration(std::istream& inputStream);
    void showProducts() const;
    void showCoins() const;
    bool orderCoffee(const int number, int insertedAmount);
    bool insertCoin(int coinValue, int& insertedTotal);
    bool saveConfiguration(const std::string& filename);

private:
    std::vector<Product> products;
    std::vector<Coin> coins;
    std::vector<int> insertedCoins;

    bool calculateChange(int change, std::vector<std::pair<int, int>>& changeCoins);
    void refundCoins();
};