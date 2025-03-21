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
        std::cerr << "Error: Cannot load the configuration XML." << std::endl;
        return false;
    }

    TiXmlElement* root = doc.FirstChildElement("CoffeeMachine");
    if (!root) {
        std::cerr << "Error: Invalid configuration file format. Root element 'CoffeeMachine' not found." << std::endl;
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
            std::cerr << "Warning: Missing required attribute in product. Skipping this product." << std::endl;
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
            std::cerr << "Warning: Missing required attribute in coin. Skipping this coin." << std::endl;
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
    std::cout << "Coin stock:" << std::endl;
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
            insertedTotal += coinValue;
            insertedCoins.push_back(std::make_pair(coinValue, 1));
            return true;
        }
    }
    std::cout << "Invalid coin." << std::endl;
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

void CoffeeMachine::refundCoins() {
    for (const auto& coin : insertedCoins) {
        double coinValue = coin.first;
        int num = coin.second;
        for (auto& c : coins) {
            if (fabs(c.value - coinValue) < 1e-6) {
                c.count -= num;
                break;
            }
        }
    }
    insertedCoins.clear();
    std::cout << "Inserted coins refunded." << std::endl;
}

bool CoffeeMachine::orderCoffee(const int number, double insertedAmount) {
    auto it = std::find_if(products.begin(), products.end(), [&](const Product& p) {
        return p.number == number;
        });

    if (it == products.end()) {
        std::cout << "Product not found." << std::endl;
        refundCoins();
        return false;
    }

    if (it->stock <= 0) {
        std::cout << "Product out of stock." << std::endl;
        refundCoins();
        return false;
    }

    double priceInEuros = it->price;
    if (insertedAmount < priceInEuros) {
        std::cout << "Insufficient funds." << std::endl;
        refundCoins();
        return false;
    }

    double changeRequired = insertedAmount - priceInEuros;
    std::vector<std::pair<double, int>> changeCoins;
    if (!calculateChange(changeRequired, changeCoins)) {
        std::cout << "Machine cannot provide change. Refunding money." << std::endl;
        refundCoins();
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
    insertedCoins.clear();

    //saveConfiguration("C:\\Users\\Korisnik\\Desktop\\test.xml"); sprema mi najnormalnije u testni file koji je na desktopu
	//saveConfiguration("C:\\Users\\Korisnik\\Desktop\\coffe\\ConsoleApplication1\\ConsoleApplication1\\XMLFile.xml"); // apsolutna putanja filea sa kojeg sam učitao konfiguraciju na koju ne mogu upisivati
	saveConfiguration("XMLFile.xml"); // relativna putanja filea sa kojeg sam učitao konfiguraciju na koju mogu upisivati
    return true;
}

bool CoffeeMachine::saveConfiguration(const std::string& filename) {

    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "");
    doc.LinkEndChild(decl);

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
	doc.Print(); // ovo sam dodao da vidim da li stvarno postoji sadrzaj koji se sprema u xml file
    
    if (!doc.SaveFile(filename)) {
        std::cerr << "Error: Could not save the updated XML file!" << std::endl;

		std::cerr << "Error: " << doc.ErrorDesc() << std::endl; // ovo sam dodao da vidim koji je tocno error, doduse nista mi ne javlja 
        
        return false;
    }

    std::cout << "Configuration successfully saved to " << filename << std::endl;
    return true;
}