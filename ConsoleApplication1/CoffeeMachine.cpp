#include "CoffeeMachine.h"
#include "tinyxml.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;


bool CoffeeMachine::loadConfiguration(const string& filename) {
    TiXmlDocument doc(filename.c_str());
    if (!doc.LoadFile()) {
        cerr << "Ne mogu učitati datoteku: " << filename << endl;
        return false;
    }

    TiXmlElement* root = doc.FirstChildElement("CoffeeMachine");
    if (!root) {
        cerr << "Neispravan format konfiguracijske datoteke." << endl;
        return false;
    }

    products.clear();
    coins.clear();

    for (TiXmlElement* elem = root->FirstChildElement("Product"); elem != nullptr; elem = elem->NextSiblingElement("Product")) {
        const char* name = elem->Attribute("name");
        const char* priceStr = elem->Attribute("price");
        const char* stockStr = elem->Attribute("stock");
        if (!name || !priceStr || !stockStr) {
            cerr << "Nedostaje obavezan atribut u proizvodu." << endl;
            continue;
        }
        Product p;
        p.name = name;
        p.price = atof(priceStr);
        p.stock = atoi(stockStr);
        products.push_back(p);
    }

    for (TiXmlElement* elem = root->FirstChildElement("Coin"); elem != nullptr; elem = elem->NextSiblingElement("Coin")) {
        const char* valueStr = elem->Attribute("value");
        const char* countStr = elem->Attribute("count");
        if (valueStr && countStr) {
            Coin c;
            c.value = atof(valueStr);
            c.count = atoi(countStr);
            coins.push_back(c);
        }
    }
    return true;

}
