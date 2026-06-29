#include <string>
#include "OrderBookTests.h"
#include "TestUtils.h"

#include "../orderbook/OrderBook.h"
#include "../orderbook/Order.h"

namespace
{
    void testEmptyBook();
    void testAddBuyOrder();
    void testAddSellOrder();
    void testFindOrder();
    void testRemoveOrder();
    void testRemoveInvalidOrder();
    void testBestBid();
    void testBestAsk();
    void testConsumePartial();
     void testBestBidBestAsk();
    void testMultiplePriceLevels();
    void testConsumeComplete();
    void testSnapshot();
}

void runOrderBookTests()
{
    beginSuite("ORDER BOOK TESTS");

    testEmptyBook();
    testAddBuyOrder();
    testAddSellOrder();
    testFindOrder();
    testRemoveOrder();
    testRemoveInvalidOrder();
    testConsumePartial();
    testConsumeComplete();
    testBestBidBestAsk();
    testMultiplePriceLevels();
    testSnapshot();

    endSuite();
}

namespace
{
    void testEmptyBook()
{
    OrderBook book;

    expect(book.empty(),
           "Empty Book");

    expect(book.size() == 0,
           "Initial Size");

    expect(book.bestBid() == -1,
           "Empty Best Bid");

    expect(book.bestAsk() == -1,
           "Empty Best Ask");

    expect(book.bestBidOrder() == nullptr,
           "Empty Best Bid Order");

    expect(book.bestAskOrder() == nullptr,
           "Empty Best Ask Order");
}
void testAddBuyOrder()
{
    OrderBook book;

    auto order = std::make_unique<Order>(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    Order* ptr = order.get();

    book.addOrder(std::move(order));

    expect(!book.empty(),
           "Add BUY - Book Not Empty");

    expect(book.size() == 1,
           "Add BUY - Size");

    expect(book.bestBid() == 150,
           "Add BUY - Best Bid");

    expect(book.bestAsk() == -1,
           "Add BUY - Best Ask");

    expect(book.findOrder(1) == ptr,
           "Add BUY - Lookup");

    expect(book.bestBidOrder() == ptr,
           "Add BUY - Best Bid Order");
}
void testAddSellOrder()
{
    OrderBook book;

    auto order = std::make_unique<Order>(
        2,
        Side::SELL,
        OrderType::LIMIT,
        80,
        155);

    Order* ptr = order.get();

    book.addOrder(std::move(order));

    expect(!book.empty(),
           "Add SELL - Book Not Empty");

    expect(book.size() == 1,
           "Add SELL - Size");

    expect(book.bestBid() == -1,
           "Add SELL - Best Bid");

    expect(book.bestAsk() == 155,
           "Add SELL - Best Ask");

    expect(book.findOrder(2) == ptr,
           "Add SELL - Lookup");

    expect(book.bestAskOrder() == ptr,
           "Add SELL - Best Ask Order");
}
void testFindOrder()
{
    OrderBook book;

    auto order1 = std::make_unique<Order>(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    auto order2 = std::make_unique<Order>(
        2,
        Side::SELL,
        OrderType::LIMIT,
        80,
        155);

    Order* ptr1 = order1.get();
    Order* ptr2 = order2.get();

    book.addOrder(std::move(order1));
    book.addOrder(std::move(order2));

    expect(book.findOrder(1) == ptr1,
           "Find Order - BUY");

    expect(book.findOrder(2) == ptr2,
           "Find Order - SELL");

    expect(book.findOrder(999) == nullptr,
           "Find Order - Invalid");
}
void testRemoveOrder()
{
    OrderBook book;

    auto order = std::make_unique<Order>(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    book.addOrder(std::move(order));

    expect(book.removeOrder(1),
           "Remove Order - Success");

    expect(book.empty(),
           "Remove Order - Empty Book");

    expect(book.size() == 0,
           "Remove Order - Size");

    expect(book.findOrder(1) == nullptr,
           "Remove Order - Lookup");

    expect(book.bestBid() == -1,
           "Remove Order - Best Bid");

    expect(book.bestBidOrder() == nullptr,
           "Remove Order - Best Bid Order");
}
void testRemoveInvalidOrder()
{
    OrderBook book;

    expect(!book.removeOrder(999),
           "Remove Invalid Order");
}
void testConsumePartial()
{
    OrderBook book;

    auto order = std::make_unique<Order>(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    book.addOrder(std::move(order));

    expect(!book.consumeOrder(1, 40),
           "Consume Partial - Not Removed");

    Order* remaining = book.findOrder(1);

    expect(remaining != nullptr,
           "Consume Partial - Order Exists");

    expect(remaining->remainingQuantity == 60,
           "Consume Partial - Remaining Quantity");

    expect(book.size() == 1,
           "Consume Partial - Size");

    expect(book.bestBid() == 150,
           "Consume Partial - Best Bid");
}
void testConsumeComplete()
{
    OrderBook book;

    auto order = std::make_unique<Order>(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    book.addOrder(std::move(order));

    expect(book.consumeOrder(1, 100),
           "Consume Complete - Removed");

    expect(book.findOrder(1) == nullptr,
           "Consume Complete - Lookup");

    expect(book.empty(),
           "Consume Complete - Empty");

    expect(book.size() == 0,
           "Consume Complete - Size");

    expect(book.bestBid() == -1,
           "Consume Complete - Best Bid");
}
void testBestBidBestAsk()
{
    OrderBook book;

    book.addOrder(std::make_unique<Order>(
        1, Side::BUY, OrderType::LIMIT, 100, 150));

    book.addOrder(std::make_unique<Order>(
        2, Side::BUY, OrderType::LIMIT, 100, 155));

    book.addOrder(std::make_unique<Order>(
        3, Side::BUY, OrderType::LIMIT, 100, 152));

    book.addOrder(std::make_unique<Order>(
        4, Side::SELL, OrderType::LIMIT, 100, 160));

    book.addOrder(std::make_unique<Order>(
        5, Side::SELL, OrderType::LIMIT, 100, 158));

    book.addOrder(std::make_unique<Order>(
        6, Side::SELL, OrderType::LIMIT, 100, 165));

    expect(book.bestBid() == 155,
           "Best Bid");

    expect(book.bestAsk() == 158,
           "Best Ask");

    expect(book.bestBidOrder()->id == 2,
           "Best Bid Order");

    expect(book.bestAskOrder()->id == 5,
           "Best Ask Order");
}
void testMultiplePriceLevels()
{
    OrderBook book;

    book.addOrder(std::make_unique<Order>(
        1, Side::BUY, OrderType::LIMIT, 100, 150));

    book.addOrder(std::make_unique<Order>(
        2, Side::BUY, OrderType::LIMIT, 100, 151));

    book.addOrder(std::make_unique<Order>(
        3, Side::BUY, OrderType::LIMIT, 100, 152));

    expect(book.size() == 3,
           "Multiple Levels - Size");

    expect(book.bestBid() == 152,
           "Multiple Levels - Highest Bid");

    book.removeOrder(3);

    expect(book.bestBid() == 151,
           "Multiple Levels - After Remove");

    book.removeOrder(2);

    expect(book.bestBid() == 150,
           "Multiple Levels - Second Remove");
}
void testSnapshot()
{
    OrderBook book;

    book.addOrder(std::make_unique<Order>(
        1, Side::BUY, OrderType::LIMIT, 100, 150));

    book.addOrder(std::make_unique<Order>(
        2, Side::SELL, OrderType::LIMIT, 80, 155));

    std::string snapshot = book.snapshot();

    expect(snapshot.find("BUY SIDE") != std::string::npos,
           "Snapshot - BUY");

    expect(snapshot.find("SELL SIDE") != std::string::npos,
           "Snapshot - SELL");

    expect(snapshot.find("150") != std::string::npos,
           "Snapshot - Bid Price");

    expect(snapshot.find("155") != std::string::npos,
           "Snapshot - Ask Price");
}
}