#include "CoffeeMachine.h"
#include "tinyxml.h"
#include <iostream>
#include <stdexcept>

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
        const char* name = elem->Attribute("name");
        const char* priceStr = elem->Attribute("price");
        const char* stockStr = elem->Attribute("stock");

        if (!name || !priceStr || !stockStr) {
            std::cerr << "Missing required attribute in product." << std::endl;
            continue;
        }

        try {
            Product p;
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
