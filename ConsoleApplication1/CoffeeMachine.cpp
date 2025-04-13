#include "CoffeeMachine.h"
#include "tinyxml.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <fstream>

bool CoffeeMachine::loadConfiguration(std::istream& inputStream) {
    std::string xmlContent((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
    TiXmlDocument doc;

    if (!doc.Parse(xmlContent.c_str())) {
        return false;
    }

    TiXmlElement* root = doc.FirstChildElement("CoffeeMachine");
    if (!root) {
        return false;
    }

    products.clear();
    coins.clear();

    for (TiXmlElement* elem = root->FirstChildElement("Product"); elem != nullptr; elem = elem->NextSiblingElement("Product")) {
        const char* number = elem->Attribute("number");
        const char* name = elem->Attribute("name");
        const char* priceStr = elem->Attribute("price");
        const char* stockStr = elem->Attribute("stock");

        if (!number || !name || !priceStr || !stockStr) {
            continue;
        }

        try {
            Product p;
            p.number = std::stoi(number);
            p.name = name;
            p.price = std::stoi(priceStr);
            p.stock = std::stoi(stockStr);
            products.push_back(p);
        }
        catch (const std::exception&) {
            continue;
        }
    }

    for (TiXmlElement* elem = root->FirstChildElement("Coin"); elem != nullptr; elem = elem->NextSiblingElement("Coin")) {
        const char* valueStr = elem->Attribute("value");
        const char* countStr = elem->Attribute("count");

        if (!valueStr || !countStr) {
            continue;
        }

        try {
            Coin c;
            c.value = std::stoi(valueStr);
            c.count = std::stoi(countStr);
            coins.push_back(c);
        }
        catch (const std::exception&) {
            continue;
        }
    }

    return true;
}

const std::vector<Product>& CoffeeMachine::getProducts() const {
    return products;
}

const std::vector<Coin>& CoffeeMachine::getCoins() const {
    return coins;
}

bool CoffeeMachine::insertCoin(int coinValue, int& insertedTotal) {
    for (auto& c : coins) {
        if (c.value == coinValue) {
            c.count++;
            insertedTotal += coinValue;
            insertedCoins.push_back(coinValue);
            return true;
        }
    }
    return false;
}

bool CoffeeMachine::calculateChange(int change, std::vector<std::pair<int, int>>& changeCoins) {
    std::vector<Coin> sortedCoins = coins;
    std::sort(sortedCoins.begin(), sortedCoins.end(), [](const Coin& a, const Coin& b) {
        return a.value > b.value;
        });

    int remaining = change;
    for (auto& coin : sortedCoins) {
        int num = 0;
        while (remaining >= coin.value && coin.count > num) {
            remaining -= coin.value;
            num++;
        }
        if (num > 0) {
            changeCoins.push_back(std::make_pair(coin.value, num));
        }
    }
    return (remaining == 0);
}

void CoffeeMachine::refundCoins() {
    for (const auto& coinValue : insertedCoins) {
        for (auto& c : coins) {
            if (c.value == coinValue) {
                c.count--;
                break;
            }
        }
    }
    insertedCoins.clear();
}

OrderStatus CoffeeMachine::orderCoffee(int number, int insertedAmount, int& change) {
    auto it = std::find_if(products.begin(), products.end(), [&](const Product& p) {
        return p.number == number;
        });

    if (it == products.end()) {
        refundCoins();
        return OrderStatus::PRODUCT_NOT_FOUND;
    }

    if (it->stock <= 0) {
        refundCoins();
        return OrderStatus::OUT_OF_STOCK;
    }

    int priceInEuros = it->price;
    if (insertedAmount < priceInEuros) {
        refundCoins();
        return OrderStatus::INSUFFICIENT_FUNDS;
    }

    change = insertedAmount - priceInEuros;
    std::vector<std::pair<int, int>> changeCoins;
    if (!calculateChange(change, changeCoins)) {
        refundCoins();
        return OrderStatus::CANNOT_PROVIDE_CHANGE;
    }

    for (auto& changePair : changeCoins) {
        int coinValue = changePair.first;
        int num = changePair.second;
        for (auto& c : coins) {
            if (c.value == coinValue) {
                c.count -= num;
                break;
            }
        }
    }

    it->stock--;
    insertedCoins.clear();
    return OrderStatus::SUCCESS;
}

bool CoffeeMachine::saveConfiguration(const std::string& filename) {
    TiXmlDocument doc;
    doc.LinkEndChild(new TiXmlDeclaration("1.0", "utf-8", ""));

    TiXmlElement* root = new TiXmlElement("CoffeeMachine");
    doc.LinkEndChild(root);

    for (const auto& product : products) {
        TiXmlElement* productElem = new TiXmlElement("Product");
        productElem->SetAttribute("number", product.number);
        productElem->SetAttribute("name", product.name.c_str());
        productElem->SetAttribute("price", std::to_string(product.price).c_str());
        productElem->SetAttribute("stock", std::to_string(product.stock).c_str());
        root->LinkEndChild(productElem);
    }

    for (const auto& coin : coins) {
        TiXmlElement* coinElem = new TiXmlElement("Coin");
        coinElem->SetAttribute("value", std::to_string(coin.value).c_str());
        coinElem->SetAttribute("count", std::to_string(coin.count).c_str());
        root->LinkEndChild(coinElem);
    }

    return doc.SaveFile(filename);
}