#pragma once
#include <string>
#include <vector>
#include <utility>
#include <iosfwd>

enum class OrderStatus {
    SUCCESS,
    PRODUCT_NOT_FOUND,
    OUT_OF_STOCK,
    INSUFFICIENT_FUNDS,
    CANNOT_PROVIDE_CHANGE
};

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
    const std::vector<Product>& getProducts() const;
    const std::vector<Coin>& getCoins() const;
    OrderStatus orderCoffee(int number, int insertedAmount, int& change);
    bool insertCoin(int coinValue, int& insertedTotal);
    bool saveConfiguration(const std::string& filename);

private:
    std::vector<Product> products;
    std::vector<Coin> coins;
    std::vector<int> insertedCoins;

    bool calculateChange(int change, std::vector<std::pair<int, int>>& changeCoins);
    void refundCoins();
};