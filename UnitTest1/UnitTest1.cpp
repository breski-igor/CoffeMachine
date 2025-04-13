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
            xmlData << "    <Product number=\"1\" name=\"Espresso\" price=\"1\" stock=\"5\" />\n";
            xmlData << "    <Coin value=\"1\" count=\"10\" />\n";
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
            xmlData << "    <Product number=\"1\" name=\"Espresso\" price=\"2\" stock=\"5\" />\n";
            xmlData << "    <Coin value=\"1\" count=\"10\" />\n";
            xmlData << "</CoffeeMachine>\n";

            CoffeeMachine machine;
            machine.loadConfiguration(xmlData);
            int change;
            OrderStatus result = machine.orderCoffee(1, 1, change);
            Assert::AreEqual(static_cast<int>(OrderStatus::INSUFFICIENT_FUNDS), static_cast<int>(result),
                L"Should return INSUFFICIENT_FUNDS when not enough money inserted");
        }
        
        TEST_METHOD(TestOrderCoffeeSuccessful)
        {
            std::stringstream xmlData;
            xmlData << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            xmlData << "<CoffeeMachine>\n";
            xmlData << "    <Product number=\"1\" name=\"Espresso\" price=\"1\" stock=\"5\" />\n";
            xmlData << "    <Coin value=\"1\" count=\"10\" />\n";
            xmlData << "</CoffeeMachine>\n";

            CoffeeMachine machine;
            machine.loadConfiguration(xmlData);
            int change;
            OrderStatus result = machine.orderCoffee(1, 2, change);
            Assert::AreEqual(static_cast<int>(OrderStatus::SUCCESS), static_cast<int>(result),
                L"Should return SUCCESS when enough money is inserted");
            Assert::AreEqual(1, change,
                L"Should return 1 euro in change for 2 euro payment on 1 euro product");
        }
        
        TEST_METHOD(TestOrderCoffeeProductNotFound)
        {
            std::stringstream xmlData;
            xmlData << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            xmlData << "<CoffeeMachine>\n";
            xmlData << "    <Product number=\"1\" name=\"Cappuccino\" price=\"2\" stock=\"5\" />\n";
            xmlData << "    <Coin value=\"1\" count=\"10\" />\n";
            xmlData << "</CoffeeMachine>\n";

            CoffeeMachine machine;
            machine.loadConfiguration(xmlData);
            int change;
            OrderStatus result = machine.orderCoffee(2, 2, change);
            Assert::AreEqual(static_cast<int>(OrderStatus::PRODUCT_NOT_FOUND), static_cast<int>(result),
                L"Should return PRODUCT_NOT_FOUND when product doesn't exist");
        }

        TEST_METHOD(TestOrderCoffeeOutOfStock)
        {
            std::stringstream xmlData;
            xmlData << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            xmlData << "<CoffeeMachine>\n";
            xmlData << "    <Product number=\"1\" name=\"Espresso\" price=\"1.50\" stock=\"0\" />\n";
            xmlData << "    <Coin value=\"1.00\" count=\"10\" />\n";
            xmlData << "    <Coin value=\"0.50\" count=\"10\" />\n";
            xmlData << "</CoffeeMachine>\n";

            CoffeeMachine machine;
            machine.loadConfiguration(xmlData);
            int change;
            OrderStatus result = machine.orderCoffee(1, 2, change);
			Assert::AreEqual(static_cast<int>(OrderStatus::OUT_OF_STOCK), static_cast<int>(result),
				L"Should return OUT_OF_STOCK when product is out of stock");
        }

        TEST_METHOD(TestOrderCoffeeInsufficientChange)
        {
            std::stringstream xmlData;
            xmlData << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            xmlData << "<CoffeeMachine>\n";
            xmlData << "    <Product number=\"1\" name=\"Espresso\" price=\"1\" stock=\"5\" />\n";
            xmlData << "    <Coin value=\"1.00\" count=\"0\" />\n";
            xmlData << "</CoffeeMachine>\n";

            CoffeeMachine machine;
            machine.loadConfiguration(xmlData);
            int change;
            OrderStatus result = machine.orderCoffee(1, 2, change);
			Assert::AreEqual(static_cast<int>(OrderStatus::CANNOT_PROVIDE_CHANGE), static_cast<int>(result),
				L"Should return CANNOT_PROVIDE_CHANGE when unable to provide change");
        }
    };
}
