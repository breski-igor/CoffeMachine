#include "CoffeeMachine.h"
#include "tinyxml.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

bool CoffeeMachine::loadConfiguration(std::istream& inputStream) {
    std::string xmlContent((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
    TiXmlDocument doc;

    if (!doc.Parse(xmlContent.c_str())) {
        std::cerr << "I cannot load the configuration XML." << std::endl;
        return false;
    }

    TiXmlElement* root = doc.FirstChildElement("CoffeeMachine");
    if (!root) {
        std::cerr << "Invalid configuration file format." << std::endl;
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
            std::cerr << "Missing required attribute in product." << std::endl;
            continue;
        }

        try {
            Product p;
            p.number = std::stoi(number);
            p.name = name;
            p.price = std::stod(priceStr);
            p.stock = std::stoi(stockStr);
            products.push_back(p);
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing product: " << e.what() << std::endl;
        }
    }

    for (TiXmlElement* elem = root->FirstChildElement("Coin"); elem != nullptr; elem = elem->NextSiblingElement("Coin")) {
        const char* valueStr = elem->Attribute("value");
        const char* countStr = elem->Attribute("count");

        if (!valueStr || !countStr) {
            std::cerr << "Missing required attribute in coin." << std::endl;
            continue;
        }

        try {
            Coin c;
            c.value = std::stod(valueStr);
            c.count = std::stoi(countStr);
            coins.push_back(c);
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing coins: " << e.what() << std::endl;
        }
    }

    return true;
}


void CoffeeMachine::showProducts() const {
    std::cout << "Available products:" << std::endl;
    for (const auto& p : products) {
        std::cout << "Name: " << p.name << ", Price: " << p.price << " euros (Stock: " << p.stock << ")" << std::endl;
    }
}

void CoffeeMachine::showCoins() const {
    std::cout << "coin stock:" << std::endl;
    double total = 0;
    for (const auto& c : coins) {
        std::cout << "Value: " << c.value << " eur, Amount: " << c.count << std::endl;
        total += c.value * c.count;
    }
    std::cout << "Total value: " << total << " eur." << std::endl;
}


bool CoffeeMachine::insertCoin(double coinValue, double& insertedTotal) {
    for (auto& c : coins) {
        if (fabs(c.value - coinValue) < 1e-6) {
            c.count++;
            return true;
        }
    }
    std::cout << "Invalid coin" << std::endl;
	insertedTotal -= coinValue;
    return false;
}


bool CoffeeMachine::calculateChange(double change, std::vector<std::pair<double, int>>& changeCoins) {
    std::vector<Coin> sortedCoins = coins;
    std::sort(sortedCoins.begin(), sortedCoins.end(), [](const Coin& a, const Coin& b) {
        return a.value > b.value;
        });

    double remaining = change;
    for (auto& coin : sortedCoins) {
        int num = 0;
        while (remaining >= coin.value - 1e-6 && coin.count > num) {
            remaining -= coin.value;
            num++;
        }
        if (num > 0) {
            changeCoins.push_back(std::make_pair(coin.value, num));
        }
    }
    return (remaining <= 1e-6);
}

bool CoffeeMachine::orderCoffee(const int number, double insertedAmount) {
    auto it = std::find_if(products.begin(), products.end(), [&](const Product& p) {
        return p.number == number;
        });

    if (it == products.end()) {
        std::cout << "Product not found." << std::endl;
        return false;
    }

    if (it->stock <= 0) {
        std::cout << "Product out of stock." << std::endl;
        return false;
    }

    double priceInEuros = it->price;
    if (insertedAmount < priceInEuros) {
        std::cout << "Insufficient funds." << std::endl;
        return false;
    }

    double changeRequired = insertedAmount - priceInEuros;
    std::vector<std::pair<double, int>> changeCoins;
    if (!calculateChange(changeRequired, changeCoins)) {
        std::cout << "Machine cannot provide change." << std::endl;
        return false;
    }

    for (auto& changePair : changeCoins) {
        double coinValue = changePair.first;
        int num = changePair.second;
        for (auto& c : coins) {
            if (fabs(c.value - coinValue) < 1e-6) {
                c.count -= num;
                break;
            }
        }
    }
    it->stock--;

    std::cout << "Ordered: " << it->name << ". Change returned: " << changeRequired << " euros." << std::endl;

    saveConfiguration("XMLFile.xml");
    return true;

}

bool CoffeeMachine::saveConfiguration(const std::string& filename) const {
    TiXmlDocument doc;

    TiXmlElement* root = new TiXmlElement("CoffeeMachine");
    doc.LinkEndChild(root);

    for (const auto& product : products) {
        TiXmlElement* productElem = new TiXmlElement("Product");
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

    if (!doc.SaveFile(filename.c_str())) {
        std::cerr << "Error: Could not save the updated XML file!" << std::endl;
        return false;
    }

    std::cout << "Configuration successfully saved to " << filename << std::endl;
    return true;
}
