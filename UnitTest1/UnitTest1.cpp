#include "CppUnitTest.h"
#include <sstream>
#include "../ConsoleApplication1/CoffeeMachine.h" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CoffeeMachineTests
{
    TEST_CLASS(CoffeeMachineUnitTests)
    {
    public:

        TEST_METHOD(TestLoadConfiguration)
        {
            std::stringstream xmlData;
            xmlData << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            xmlData << "<CoffeeMachine>\n";
            xmlData << "    <Product name=\"Espresso\" price=\"1.50\" stock=\"5\" />\n";
            xmlData << "    <Coin value=\"1.00\" count=\"10\" />\n";
            xmlData << "</CoffeeMachine>\n";

            CoffeeMachine machine;
            bool loaded = machine.loadConfiguration(xmlData);

            Assert::IsTrue(loaded, L"Configuration data successfully loaded.");
        }
        
        TEST_METHOD(TestOrderCoffeeInsufficientMoney)
        {
            std::stringstream xmlData;
            xmlData << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            xmlData << "<CoffeeMachine>\n";
            xmlData << "    <Product name=\"Espresso\" price=\"1.50\" stock=\"5\" />\n";
            xmlData << "    <Coin value=\"1.00\" count=\"10\" />\n";
            xmlData << "</CoffeeMachine>\n";

            CoffeeMachine machine;
            machine.loadConfiguration(xmlData);

            bool result = machine.orderCoffee("Espresso", 1.0);
            Assert::IsFalse(result, L"Not enough coins inserted.");
        }
        
        TEST_METHOD(TestOrderCoffeeSuccessful)
        {
            std::stringstream xmlData;
            xmlData << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            xmlData << "<CoffeeMachine>\n";
            xmlData << "    <Product name=\"Espresso\" price=\"1.50\" stock=\"5\" />\n";
            xmlData << "    <Coin value=\"0.50\" count=\"10\" />\n";
            xmlData << "    <Coin value=\"1.00\" count=\"10\" />\n";
            xmlData << "</CoffeeMachine>\n";

            CoffeeMachine machine;
            machine.loadConfiguration(xmlData);

            bool result = machine.orderCoffee("Espresso", 2.00);
            Assert::IsTrue(result, L"Order successful.");
        }

        TEST_METHOD(TestOrderCoffeeProductNotFound)
        {
            std::stringstream xmlData;
            xmlData << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            xmlData << "<CoffeeMachine>\n";
            xmlData << "    <Product name=\"Cappuccino\" price=\"2.00\" stock=\"5\" />\n";
            xmlData << "    <Coin value=\"1.00\" count=\"10\" />\n";
            xmlData << "</CoffeeMachine>\n";

            CoffeeMachine machine;
            machine.loadConfiguration(xmlData);

            bool result = machine.orderCoffee("Espresso", 2.00);
            Assert::IsFalse(result, L"Product does not exist in the configuration.");
        }

        TEST_METHOD(TestOrderCoffeeOutOfStock)
        {
            std::stringstream xmlData;
            xmlData << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            xmlData << "<CoffeeMachine>\n";
            xmlData << "    <Product name=\"Espresso\" price=\"1.50\" stock=\"0\" />\n";
            xmlData << "    <Coin value=\"1.00\" count=\"10\" />\n";
            xmlData << "    <Coin value=\"0.50\" count=\"10\" />\n";
            xmlData << "</CoffeeMachine>\n";

            CoffeeMachine machine;
            machine.loadConfiguration(xmlData);

            bool result = machine.orderCoffee("Espresso", 2.00);
            Assert::IsFalse(result, L"Product is out of stock.");
        }

        TEST_METHOD(TestOrderCoffeeInsufficientChange)
        {
            std::stringstream xmlData;
            xmlData << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            xmlData << "<CoffeeMachine>\n";
            xmlData << "    <Product name=\"Espresso\" price=\"1.50\" stock=\"5\" />\n";
            xmlData << "    <Coin value=\"1.00\" count=\"10\" />\n";
            xmlData << "</CoffeeMachine>\n";

            CoffeeMachine machine;
            machine.loadConfiguration(xmlData);

            bool result = machine.orderCoffee("Espresso", 2.00);
            Assert::IsFalse(result, L"Not enough coins to give the correct change.");
        }
    };
}
