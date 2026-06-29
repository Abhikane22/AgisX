#include "MatchingEngineTests.h"
#include "TestUtils.h"

#include "../MatchingEngine/MatchingEngine.h"

namespace
{
    void testPlaceLimitOrder();
    void testFullMatch();
    void testPartialMatch();
    void testMultiLevelMatch();
    void testMarketBuy();
    void testMarketSell();
    void testCancelOrder();
    void testTradeHistory();
    void testSnapshot();
    void testFIFO();
}

void runMatchingEngineTests()
{
    beginSuite("MATCHING ENGINE TESTS");

    testPlaceLimitOrder();
    testFullMatch();
    testPartialMatch();
    testMultiLevelMatch();
    testFIFO();
    testMarketBuy();
    testMarketSell();

    testCancelOrder();

    testTradeHistory();

    testSnapshot();

    endSuite();
}
namespace
{
    void testPlaceLimitOrder()
{
    MatchingEngine engine;

    auto order = std::make_unique<Order>(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    auto trades =
        engine.placeLimitOrder(std::move(order));

    expect(trades.empty(),
           "Place Limit - No Trades");

    expect(engine.bestBid() == 150,
           "Place Limit - Best Bid");

    expect(engine.bestAsk() == -1,
           "Place Limit - Best Ask");

    expect(engine.tradeHistory().empty(),
           "Place Limit - Empty History");
}
void testFullMatch()
{
    MatchingEngine engine;

    auto buy = std::make_unique<Order>(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    engine.placeLimitOrder(std::move(buy));

    auto sell = std::make_unique<Order>(
        2,
        Side::SELL,
        OrderType::LIMIT,
        100,
        149);

    auto trades =
        engine.placeLimitOrder(std::move(sell));

    expect(trades.size() == 1,
           "Full Match - Trade Created");

    expect(trades[0].buyOrderId == 1,
           "Full Match - Buyer");

    expect(trades[0].sellOrderId == 2,
           "Full Match - Seller");

    expect(trades[0].quantity == 100,
           "Full Match - Quantity");

    expect(trades[0].executionPrice == 150,
           "Full Match - Execution Price");

    expect(engine.tradeHistory().size() == 1,
           "Full Match - History");

    expect(engine.bestBid() == -1,
           "Full Match - Empty Bid");

    expect(engine.bestAsk() == -1,
           "Full Match - Empty Ask");
}
void testPartialMatch()
{
    MatchingEngine engine;

    auto buy = std::make_unique<Order>(
        1,
        Side::BUY,
        OrderType::LIMIT,
        100,
        150);

    engine.placeLimitOrder(std::move(buy));

    auto sell = std::make_unique<Order>(
        2,
        Side::SELL,
        OrderType::LIMIT,
        40,
        149);

    auto trades =
        engine.placeLimitOrder(std::move(sell));

    expect(trades.size() == 1,
           "Partial Match - Trade Created");

    expect(trades[0].quantity == 40,
           "Partial Match - Quantity");

    expect(trades[0].executionPrice == 150,
           "Partial Match - Execution Price");

    expect(engine.tradeHistory().size() == 1,
           "Partial Match - History");

    expect(engine.bestBid() == 150,
           "Partial Match - Best Bid");

    expect(engine.bestAsk() == -1,
           "Partial Match - Best Ask");

    std::string snapshot = engine.snapshot();

    expect(snapshot.find("60") != std::string::npos,
           "Partial Match - Remaining Quantity");
}
void testMultiLevelMatch()
{
    MatchingEngine engine;

    // BUY @150
    engine.placeLimitOrder(
        std::make_unique<Order>(
            1,
            Side::BUY,
            OrderType::LIMIT,
            100,
            150));

    // BUY @149
    engine.placeLimitOrder(
        std::make_unique<Order>(
            2,
            Side::BUY,
            OrderType::LIMIT,
            80,
            149));

    // BUY @148
    engine.placeLimitOrder(
        std::make_unique<Order>(
            3,
            Side::BUY,
            OrderType::LIMIT,
            60,
            148));

    // SELL 210 @147
    auto sell =
        std::make_unique<Order>(
            4,
            Side::SELL,
            OrderType::LIMIT,
            210,
            147);

    auto trades =
        engine.placeLimitOrder(std::move(sell));

    expect(trades.size() == 3,
           "Multi Level - Trade Count");

    expect(trades[0].buyOrderId == 1,
           "Multi Level - First Buyer");

    expect(trades[0].quantity == 100,
           "Multi Level - First Quantity");

    expect(trades[0].executionPrice == 150,
           "Multi Level - First Price");

    expect(trades[1].buyOrderId == 2,
           "Multi Level - Second Buyer");

    expect(trades[1].quantity == 80,
           "Multi Level - Second Quantity");

    expect(trades[1].executionPrice == 149,
           "Multi Level - Second Price");

    expect(trades[2].buyOrderId == 3,
           "Multi Level - Third Buyer");

    expect(trades[2].quantity == 30,
           "Multi Level - Third Quantity");

    expect(trades[2].executionPrice == 148,
           "Multi Level - Third Price");

    expect(engine.tradeHistory().size() == 3,
           "Multi Level - History");

    expect(engine.bestBid() == 148,
           "Multi Level - Remaining Best Bid");

    std::string snapshot = engine.snapshot();

    expect(snapshot.find("30") != std::string::npos,
           "Multi Level - Remaining Quantity");
}
void testFIFO()
{
    MatchingEngine engine;

    // First BUY arrives
    engine.placeLimitOrder(
        std::make_unique<Order>(
            1,
            Side::BUY,
            OrderType::LIMIT,
            100,
            150));

    // Second BUY at same price
    engine.placeLimitOrder(
        std::make_unique<Order>(
            2,
            Side::BUY,
            OrderType::LIMIT,
            100,
            150));

    // SELL arrives
    auto sell =
        std::make_unique<Order>(
            3,
            Side::SELL,
            OrderType::LIMIT,
            150,
            149);

    auto trades =
        engine.placeLimitOrder(std::move(sell));

    expect(trades.size() == 2,
           "FIFO - Trade Count");

    // First trade should consume Order #1
    expect(trades[0].buyOrderId == 1,
           "FIFO - First Buyer");

    expect(trades[0].quantity == 100,
           "FIFO - First Quantity");

    // Second trade should consume Order #2
    expect(trades[1].buyOrderId == 2,
           "FIFO - Second Buyer");

    expect(trades[1].quantity == 50,
           "FIFO - Second Quantity");

    expect(engine.bestBid() == 150,
           "FIFO - Remaining Bid");

    std::string snapshot = engine.snapshot();

    expect(snapshot.find("50") != std::string::npos,
           "FIFO - Remaining Quantity");
}
void testMarketBuy()
{
    MatchingEngine engine;

    engine.placeLimitOrder(
        std::make_unique<Order>(
            1,
            Side::SELL,
            OrderType::LIMIT,
            100,
            150));

    auto marketBuy =
        std::make_unique<Order>(
            2,
            Side::BUY,
            OrderType::MARKET,
            60);

    auto trades =
        engine.placeMarketOrder(std::move(marketBuy));

    expect(trades.size() == 1,
           "Market Buy - Trade");

    expect(trades[0].quantity == 60,
           "Market Buy - Quantity");

    expect(trades[0].executionPrice == 150,
           "Market Buy - Price");

    expect(engine.bestAsk() == 150,
           "Market Buy - Remaining Ask");
}
void testMarketSell()
{
    MatchingEngine engine;

    engine.placeLimitOrder(
        std::make_unique<Order>(
            1,
            Side::BUY,
            OrderType::LIMIT,
            100,
            150));

    auto marketSell =
        std::make_unique<Order>(
            2,
            Side::SELL,
            OrderType::MARKET,
            40);

    auto trades =
        engine.placeMarketOrder(std::move(marketSell));

    expect(trades.size() == 1,
           "Market Sell - Trade");

    expect(trades[0].quantity == 40,
           "Market Sell - Quantity");

    expect(trades[0].executionPrice == 150,
           "Market Sell - Price");

    expect(engine.bestBid() == 150,
           "Market Sell - Remaining Bid");
}
void testCancelOrder()
{
    MatchingEngine engine;

    engine.placeLimitOrder(
        std::make_unique<Order>(
            1,
            Side::BUY,
            OrderType::LIMIT,
            100,
            150));

    expect(engine.cancelOrder(1),
           "Cancel - Existing");

    expect(engine.bestBid() == -1,
           "Cancel - Empty Book");

    expect(!engine.cancelOrder(999),
           "Cancel - Invalid");
}
void testTradeHistory()
{
    MatchingEngine engine;

    engine.placeLimitOrder(
        std::make_unique<Order>(
            1,
            Side::BUY,
            OrderType::LIMIT,
            100,
            150));

    auto sell =
        std::make_unique<Order>(
            2,
            Side::SELL,
            OrderType::LIMIT,
            100,
            149);

    engine.placeLimitOrder(std::move(sell));

    expect(engine.tradeHistory().size() == 1,
           "Trade History - Size");

    expect(engine.tradeHistory()[0].buyOrderId == 1,
           "Trade History - Buyer");

    expect(engine.tradeHistory()[0].sellOrderId == 2,
           "Trade History - Seller");
}
void testSnapshot()
{
    MatchingEngine engine;

    engine.placeLimitOrder(
        std::make_unique<Order>(
            1,
            Side::BUY,
            OrderType::LIMIT,
            100,
            150));

    engine.placeLimitOrder(
        std::make_unique<Order>(
            2,
            Side::SELL,
            OrderType::LIMIT,
            100,
            155));

    std::string snap = engine.snapshot();

    expect(snap.find("BUY SIDE") != std::string::npos,
           "Snapshot - BUY");

    expect(snap.find("SELL SIDE") != std::string::npos,
           "Snapshot - SELL");

    expect(snap.find("150") != std::string::npos,
           "Snapshot - Bid");

    expect(snap.find("155") != std::string::npos,
           "Snapshot - Ask");
}
}