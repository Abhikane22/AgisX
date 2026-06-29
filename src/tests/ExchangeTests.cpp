#include "ExchangeTests.h"
#include "TestUtils.h"

#include "../engine/Exchange.h"

namespace
{
    void testEmptyExchange();
    void testCreateEngine();
    void testMultipleSymbols();
    void testCancelOrder();
    void testTradeHistory();
    void testSnapshot();

}

void runExchangeTests()
{
    beginSuite("EXCHANGE TESTS");

    testEmptyExchange();
    testCreateEngine();
    testMultipleSymbols();
    testCancelOrder();
    testSnapshot();
    testTradeHistory();

    endSuite();
}
namespace
{
    void testEmptyExchange()
{
    Exchange exchange;

    expect(exchange.bestBid("AAPL") == -1,
           "Empty - Best Bid");

    expect(exchange.bestAsk("AAPL") == -1,
           "Empty - Best Ask");

    expect(exchange.cancelOrder("AAPL", 1) == false,
           "Empty - Cancel");

    expect(exchange.snapshot("AAPL") ==
           "Unknown Symbol",
           "Empty - Snapshot");
}
void testCreateEngine()
{
    Exchange exchange;

    auto order =
        std::make_unique<Order>(
            1,
            Side::BUY,
            OrderType::LIMIT,
            100,
            150);

    auto trades =
        exchange.placeOrder(
            "AAPL",
            std::move(order));

    expect(trades.empty(),
           "Create Engine - No Trades");

    expect(exchange.bestBid("AAPL") == 150,
           "Create Engine - Best Bid");

    expect(exchange.bestAsk("AAPL") == -1,
           "Create Engine - Best Ask");

    expect(exchange.tradeHistory("AAPL").empty(),
           "Create Engine - Empty History");
}
void testMultipleSymbols()
{
    Exchange exchange;

    exchange.placeOrder(
        "AAPL",
        std::make_unique<Order>(
            1,
            Side::BUY,
            OrderType::LIMIT,
            100,
            150));

    exchange.placeOrder(
        "TSLA",
        std::make_unique<Order>(
            2,
            Side::BUY,
            OrderType::LIMIT,
            80,
            200));

    expect(exchange.bestBid("AAPL") == 150,
           "Multiple Symbols - AAPL");

    expect(exchange.bestBid("TSLA") == 200,
           "Multiple Symbols - TSLA");

    expect(exchange.bestAsk("AAPL") == -1,
           "Multiple Symbols - AAPL Ask");

    expect(exchange.bestAsk("TSLA") == -1,
           "Multiple Symbols - TSLA Ask");
}
void testCancelOrder()
{
    Exchange exchange;

    exchange.placeOrder(
        "AAPL",
        std::make_unique<Order>(
            1,
            Side::BUY,
            OrderType::LIMIT,
            100,
            150));

    expect(exchange.cancelOrder("AAPL", 1),
           "Cancel - Existing");

    expect(exchange.bestBid("AAPL") == -1,
           "Cancel - Book Empty");

    expect(!exchange.cancelOrder("AAPL", 999),
           "Cancel - Invalid Order");

    expect(!exchange.cancelOrder("TSLA", 1),
           "Cancel - Unknown Symbol");
}
void testSnapshot()
{
    Exchange exchange;

    exchange.placeOrder(
        "AAPL",
        std::make_unique<Order>(
            1,
            Side::BUY,
            OrderType::LIMIT,
            100,
            150));

    exchange.placeOrder(
        "AAPL",
        std::make_unique<Order>(
            2,
            Side::SELL,
            OrderType::LIMIT,
            50,
            155));

    std::string snapshot =
        exchange.snapshot("AAPL");

    expect(snapshot.find("BUY SIDE") != std::string::npos,
           "Snapshot - BUY");

    expect(snapshot.find("SELL SIDE") != std::string::npos,
           "Snapshot - SELL");

    expect(snapshot.find("150") != std::string::npos,
           "Snapshot - Bid");

    expect(snapshot.find("155") != std::string::npos,
           "Snapshot - Ask");
}
void testTradeHistory()
{
    Exchange exchange;

    exchange.placeOrder(
        "AAPL",
        std::make_unique<Order>(
            1,
            Side::BUY,
            OrderType::LIMIT,
            100,
            150));

    exchange.placeOrder(
        "AAPL",
        std::make_unique<Order>(
            2,
            Side::SELL,
            OrderType::LIMIT,
            100,
            149));

    const auto& history =
        exchange.tradeHistory("AAPL");

    expect(history.size() == 1,
           "History - Size");

    expect(history[0].buyOrderId == 1,
           "History - Buyer");

    expect(history[0].sellOrderId == 2,
           "History - Seller");

    expect(history[0].quantity == 100,
           "History - Quantity");
}
}