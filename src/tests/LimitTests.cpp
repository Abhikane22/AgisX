#include "LimitTests.h"
#include "TestUtils.h"

#include "../orderbook/Limit.h"
#include "../orderbook/Order.h"

#include <stdexcept>

namespace
{
    void testEmptyConstructor();
    void testAddFirstOrder();
    void testAddMultipleOrders();
    void testRemoveHead();
    void testRemoveTail();
    void testRemoveMiddle();
    void testVolumeTracking();
    void testOrderCount();
    void testEmpty();
    void testReduceVolume();
    void testVolumeUnderflow();
}

void runLimitTests()
{
    beginSuite("LIMIT TESTS");

    testEmptyConstructor();
    testAddFirstOrder();
    testAddMultipleOrders();
    testRemoveHead();
    testRemoveTail();
    testRemoveMiddle();
    testVolumeTracking();
    testOrderCount();
    testReduceVolume();
    testVolumeUnderflow();


    endSuite();
}
namespace
{
    void testEmptyConstructor()
{
    Limit limit(150);

    expect(limit.price == 150,
           "Constructor - Price");

    expect(limit.head == nullptr,
           "Constructor - Head");

    expect(limit.tail == nullptr,
           "Constructor - Tail");

    expect(limit.orderCount == 0,
           "Constructor - Order Count");

    expect(limit.totalVolume == 0,
           "Constructor - Total Volume");

    expect(limit.empty(),
           "Constructor - Empty");
}
void testAddFirstOrder()
{
    Limit limit(150);

    Order order(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    limit.addOrder(&order);

    expect(limit.head == &order,
           "Add First Order - Head");

    expect(limit.tail == &order,
           "Add First Order - Tail");

    expect(order.parent == &limit,
           "Add First Order - Parent");

    expect(order.next == nullptr,
           "Add First Order - Next");

    expect(order.prev == nullptr,
           "Add First Order - Previous");

    expect(limit.orderCount == 1,
           "Add First Order - Order Count");

    expect(limit.totalVolume == 100,
           "Add First Order - Total Volume");

    expect(!limit.empty(),
           "Add First Order - Not Empty");
}
void testAddMultipleOrders()
{
    Limit limit(150);

    Order order1(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    Order order2(
        2,
        Side::BUY,
        OrderType::LIMIT,
        50,
        150);

    Order order3(
        3,
        Side::BUY,
        OrderType::LIMIT,
        75,
        150);

    limit.addOrder(&order1);
    limit.addOrder(&order2);
    limit.addOrder(&order3);

    //-----------------------------
    // Head & Tail
    //-----------------------------

    expect(limit.head == &order1,
           "Add Multiple - Head");

    expect(limit.tail == &order3,
           "Add Multiple - Tail");

    //-----------------------------
    // Forward Links
    //-----------------------------

    expect(order1.next == &order2,
           "Add Multiple - Order1 Next");

    expect(order2.next == &order3,
           "Add Multiple - Order2 Next");

    expect(order3.next == nullptr,
           "Add Multiple - Order3 Next");

    //-----------------------------
    // Backward Links
    //-----------------------------

    expect(order1.prev == nullptr,
           "Add Multiple - Order1 Prev");

    expect(order2.prev == &order1,
           "Add Multiple - Order2 Prev");

    expect(order3.prev == &order2,
           "Add Multiple - Order3 Prev");

    //-----------------------------
    // Parent
    //-----------------------------

    expect(order1.parent == &limit,
           "Add Multiple - Parent1");

    expect(order2.parent == &limit,
           "Add Multiple - Parent2");

    expect(order3.parent == &limit,
           "Add Multiple - Parent3");

    //-----------------------------
    // Statistics
    //-----------------------------

    expect(limit.orderCount == 3,
           "Add Multiple - Order Count");

    expect(limit.totalVolume == 225,
           "Add Multiple - Total Volume");
}
void testRemoveHead()
{
    Limit limit(150);

    Order order1(1, Side::BUY, OrderType::LIMIT, 100, 150);
    Order order2(2, Side::BUY, OrderType::LIMIT, 50, 150);
    Order order3(3, Side::BUY, OrderType::LIMIT, 75, 150);

    limit.addOrder(&order1);
    limit.addOrder(&order2);
    limit.addOrder(&order3);

    limit.removeOrder(&order1);

    expect(limit.head == &order2,
           "Remove Head - New Head");

    expect(order2.prev == nullptr,
           "Remove Head - Prev Null");

    expect(order2.next == &order3,
           "Remove Head - Next");

    expect(limit.tail == &order3,
           "Remove Head - Tail");

    expect(limit.orderCount == 2,
           "Remove Head - Order Count");

    expect(limit.totalVolume == 125,
           "Remove Head - Total Volume");

    expect(order1.parent == nullptr,
           "Remove Head - Parent Cleared");
}
void testRemoveTail()
{
    Limit limit(150);

    Order order1(1, Side::BUY, OrderType::LIMIT, 100, 150);
    Order order2(2, Side::BUY, OrderType::LIMIT, 50, 150);
    Order order3(3, Side::BUY, OrderType::LIMIT, 75, 150);

    limit.addOrder(&order1);
    limit.addOrder(&order2);
    limit.addOrder(&order3);

    limit.removeOrder(&order3);

    expect(limit.tail == &order2,
           "Remove Tail - New Tail");

    expect(order2.next == nullptr,
           "Remove Tail - Next Null");

    expect(order2.prev == &order1,
           "Remove Tail - Prev");

    expect(limit.head == &order1,
           "Remove Tail - Head");

    expect(limit.orderCount == 2,
           "Remove Tail - Order Count");

    expect(limit.totalVolume == 150,
           "Remove Tail - Total Volume");

    expect(order3.parent == nullptr,
           "Remove Tail - Parent Cleared");
}
void testRemoveMiddle()
{
    Limit limit(150);

    Order order1(1, Side::BUY, OrderType::LIMIT, 100, 150);
    Order order2(2, Side::BUY, OrderType::LIMIT, 50, 150);
    Order order3(3, Side::BUY, OrderType::LIMIT, 75, 150);

    limit.addOrder(&order1);
    limit.addOrder(&order2);
    limit.addOrder(&order3);

    limit.removeOrder(&order2);

    expect(order1.next == &order3,
           "Remove Middle - Order1 Next");

    expect(order3.prev == &order1,
           "Remove Middle - Order3 Prev");

    expect(limit.head == &order1,
           "Remove Middle - Head");

    expect(limit.tail == &order3,
           "Remove Middle - Tail");

    expect(limit.orderCount == 2,
           "Remove Middle - Order Count");

    expect(limit.totalVolume == 175,
           "Remove Middle - Total Volume");

    expect(order2.parent == nullptr,
           "Remove Middle - Parent Cleared");

    expect(order2.next == nullptr,
           "Remove Middle - Next Cleared");

    expect(order2.prev == nullptr,
           "Remove Middle - Prev Cleared");
}
void testVolumeTracking()
{
    Limit limit(150);

    Order order1(1, Side::BUY, OrderType::LIMIT, 100, 150);
    Order order2(2, Side::BUY, OrderType::LIMIT, 50, 150);

    limit.addOrder(&order1);

    expect(limit.totalVolume == 100,
           "Volume Tracking - After First Add");

    limit.addOrder(&order2);

    expect(limit.totalVolume == 150,
           "Volume Tracking - After Second Add");

    limit.removeOrder(&order1);

    expect(limit.totalVolume == 50,
           "Volume Tracking - After Remove");
}
void testOrderCount()
{
    Limit limit(150);

    Order order1(1, Side::BUY, OrderType::LIMIT, 100, 150);
    Order order2(2, Side::BUY, OrderType::LIMIT, 50, 150);

    expect(limit.orderCount == 0,
           "Order Count - Initially");

    limit.addOrder(&order1);

    expect(limit.orderCount == 1,
           "Order Count - First Add");

    limit.addOrder(&order2);

    expect(limit.orderCount == 2,
           "Order Count - Second Add");

    limit.removeOrder(&order2);

    expect(limit.orderCount == 1,
           "Order Count - After Remove");
}
void testReduceVolume()
{
    Limit limit(150);

    Order order1(1, Side::BUY, OrderType::LIMIT, 100, 150);

    limit.addOrder(&order1);

    limit.reduceVolume(40);

    expect(limit.totalVolume == 60,
           "Reduce Volume");
}
void testVolumeUnderflow()
{
    Limit limit(150);

    bool exceptionThrown = false;

    try
    {
        limit.reduceVolume(10);
    }
    catch (const std::runtime_error&)
    {
        exceptionThrown = true;
    }

    expect(exceptionThrown,
           "Volume Underflow");
}
}