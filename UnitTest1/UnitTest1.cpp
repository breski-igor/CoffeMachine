#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include <cstdio>
#include "../ConsoleApplication1/CoffeeMachine.h" 
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CoffeeMachineTests
{
    TEST_CLASS(CoffeeMachineUnitTests)
    {
    public:
        TEST_METHOD(TestLoadConfiguration)
        {
            const char* tempFile = "tempConfig.xml";
            std::ofstream ofs(tempFile);
            ofs << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            ofs << "<CoffeeMachine>\n";
            ofs << "    <Product name=\"Espresso\" price=\"1.50\" stock=\"5\" />\n";
            ofs << "    <Coin value=\"1.00\" count=\"10\" />\n";
            ofs << "</CoffeeMachine>\n";
            ofs.close();

            CoffeeMachine machine;
            bool loaded = machine.loadConfiguration(tempFile);
            Assert::IsTrue(loaded, L"Konfiguracijski file treba biti uspjesno ucitan.");

            remove(tempFile);
        }
        /*
        TEST_METHOD(TestOrderCoffeeInsufficientMoney)
        {
            const char* tempFile = "tempConfig.xml";
            std::ofstream ofs(tempFile);
            ofs << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            ofs << "<CoffeeMachine>\n";
            ofs << "    <Product name=\"Espresso\" price=\"1.50\" stock=\"5\" />\n";
            ofs << "    <Coin value=\"1.00\" count=\"10\" />\n";
            ofs << "</CoffeeMachine>\n";
            ofs.close();

            CoffeeMachine machine;
            machine.loadConfiguration(tempFile);

            bool result = machine.orderCoffee("Espresso", 1.0);
            Assert::IsFalse(result, L"Nedovoljno ubacenih kovanica");

            remove(tempFile);
        }
        
        TEST_METHOD(TestOrderCoffeeSuccessful)
        {
            const char* tempFile = "tempConfig.xml";
            std::ofstream ofs(tempFile);
            ofs << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            ofs << "<CoffeeMachine>\n";
            ofs << "    <Product name=\"Espresso\" price=\"1.50\" stock=\"5\" />\n";
            ofs << "    <Coin value=\"0.50\" count=\"10\" />\n";
            ofs << "    <Coin value=\"1.00\" count=\"10\" />\n";
            ofs << "</CoffeeMachine>\n";
            ofs.close();

            CoffeeMachine machine;
            machine.loadConfiguration(tempFile);

            bool result = machine.orderCoffee("Espresso", 2.00);
            Assert::IsTrue(result, L"Uspjesna narudžba.");

            remove(tempFile);
        }

        TEST_METHOD(TestOrderCoffeeProductNotFound)
        {
            const char* tempFile = "tempConfig.xml";
            std::ofstream ofs(tempFile);
            ofs << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            ofs << "<CoffeeMachine>\n";
            ofs << "    <Product name=\"Cappuccino\" price=\"2.00\" stock=\"5\" />\n";
            ofs << "    <Coin value=\"1.00\" count=\"10\" />\n";
            ofs << "</CoffeeMachine>\n";
            ofs.close();

            CoffeeMachine machine;
            machine.loadConfiguration(tempFile);

            bool result = machine.orderCoffee("Espresso", 2.00);
            Assert::IsFalse(result, L"Nepostojeci proizvod");

            remove(tempFile);
        }

        TEST_METHOD(TestOrderCoffeeOutOfStock)
        {
            const char* tempFile = "tempConfig.xml";
            std::ofstream ofs(tempFile);
            ofs << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            ofs << "<CoffeeMachine>\n";
            ofs << "    <Product name=\"Espresso\" price=\"1.50\" stock=\"0\" />\n";
            ofs << "    <Coin value=\"1.00\" count=\"10\" />\n";
            ofs << "    <Coin value=\"0.50\" count=\"10\" />\n";
            ofs << "</CoffeeMachine>\n";
            ofs.close();

            CoffeeMachine machine;
            machine.loadConfiguration(tempFile);

            bool result = machine.orderCoffee("Espresso", 2.00);
            Assert::IsFalse(result, L"Nije na stanju.");

            remove(tempFile);
        }

        TEST_METHOD(TestOrderCoffeeInsufficientChange)
        {
            const char* tempFile = "tempConfig.xml";
            std::ofstream ofs(tempFile);
            ofs << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            ofs << "<CoffeeMachine>\n";
            ofs << "    <Product name=\"Espresso\" price=\"1.50\" stock=\"5\" />\n";
            ofs << "    <Coin value=\"1.00\" count=\"10\" />\n";
            ofs << "</CoffeeMachine>\n";
            ofs.close();

            CoffeeMachine machine;
            machine.loadConfiguration(tempFile);

            bool result = machine.orderCoffee("Espresso", 2.00);
            Assert::IsFalse(result, L"Nedostatak kovanica za vraćanje tocnog iznosa.");

            remove(tempFile);
        }*/
    };
}
