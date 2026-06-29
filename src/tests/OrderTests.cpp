#include "OrderTests.h"
#include "TestUtils.h"

#include "../orderbook/Order.h"
#include "../core/Enums.h"

#include <sstream>
#include <stdexcept>

namespace
{
    void testLimitConstructor();
    void testMarketConstructor();
    void testReduceQuantity();
    void testFilled();
    void testQuantityUnderflow();
    void testStreamOperator();
    
}

void runOrderTests()
{
    beginSuite("ORDER TESTS");

    testLimitConstructor();
    testMarketConstructor();
    testReduceQuantity();
    testFilled();
    testQuantityUnderflow();
    testStreamOperator();


    endSuite();
}
namespace
{

void testLimitConstructor()
{
    Order order(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    expect(order.id == 1,
           "Limit Constructor - ID");

    expect(order.side == Side::BUY,
           "Limit Constructor - Side");

    expect(order.type == OrderType::LIMIT,
           "Limit Constructor - Type");

    expect(order.remainingQuantity == 100,
           "Limit Constructor - Quantity");

    expect(order.price.has_value(),
           "Limit Constructor - Price Exists");

    expect(order.price.value() == 150,
           "Limit Constructor - Price");

    expect(order.next == nullptr,
           "Limit Constructor - Next Pointer");

    expect(order.prev == nullptr,
           "Limit Constructor - Previous Pointer");

    expect(order.parent == nullptr,
           "Limit Constructor - Parent Pointer");
}
void testMarketConstructor()
{
    Order order(
        2,
        Side::SELL,
        OrderType::MARKET,
        50);

    expect(order.id == 2,
           "Market Constructor - ID");

    expect(order.side == Side::SELL,
           "Market Constructor - Side");

    expect(order.type == OrderType::MARKET,
           "Market Constructor - Type");

    expect(order.remainingQuantity == 50,
           "Market Constructor - Quantity");

    expect(!order.price.has_value(),
           "Market Constructor - No Price");

    expect(order.next == nullptr,
           "Market Constructor - Next Pointer");

    expect(order.prev == nullptr,
           "Market Constructor - Previous Pointer");

    expect(order.parent == nullptr,
           "Market Constructor - Parent Pointer");

    expect(!order.filled(),
           "Market Constructor - Not Filled");
}
void testReduceQuantity()
{
    Order order(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    order.reduceQuantity(40);

    expect(order.remainingQuantity == 60,
           "Reduce Quantity - Remaining");

    expect(!order.filled(),
           "Reduce Quantity - Not Filled");

    expect(order.price.has_value() &&
           order.price.value() == 150,
           "Reduce Quantity - Price Unchanged");
}
void testFilled()
{
    Order order(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    order.reduceQuantity(100);

    expect(order.remainingQuantity == 0,
           "Filled - Remaining Quantity");

    expect(order.filled(),
           "Filled - Order Filled");
}
void testQuantityUnderflow()
{
    Order order(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    bool exceptionThrown = false;

    try
    {
        order.reduceQuantity(150);
    }
    catch (const std::runtime_error&)
    {
        exceptionThrown = true;
    }

    expect(exceptionThrown,
           "Quantity Underflow - Exception");
}
void testStreamOperator()
{
    Order order(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    std::ostringstream out;

    out << order;

    std::string str = out.str();

    expect(!str.empty(),
           "Stream Operator - Output");

    expect(str.find("ID") != std::string::npos,
           "Stream Operator - Contains ID");

    expect(str.find("BUY") != std::string::npos,
           "Stream Operator - Contains Side");

    expect(str.find("LIMIT") != std::string::npos,
           "Stream Operator - Contains Type");
}
}