#include "MessageParserTests.h"
#include "TestUtils.h"

#include "../network/MessageParser.h"

namespace
{
    void testParseLimitOrder();
    void testParseMarketOrder();
    void testParseCancel();
    void testParseSnapshot();
    void testParseBestBid();
    void testParseBestAsk();
    void testParseTradeHistory();
    void testInvalidCommand();
}

void runMessageParserTests()
{
    beginSuite("MESSAGE PARSER TESTS");

    testParseLimitOrder();

    endSuite();
}
namespace
{
    void testParseLimitOrder()
{
    MessageParser parser;

    Message message =
        parser.parse(
            "PLACE AAPL BUY LIMIT 100 150");

    expect(message.type == MessageType::PLACE_ORDER,
           "Limit - Type");

    expect(message.symbol == "AAPL",
           "Limit - Symbol");

    expect(message.order != nullptr,
           "Limit - Order Exists");

    expect(message.order->side == Side::BUY,
           "Limit - Side");

    expect(message.order->type == OrderType::LIMIT,
           "Limit - Order Type");

    expect(message.order->remainingQuantity == 100,
           "Limit - Quantity");

    expect(message.order->price.has_value(),
           "Limit - Price Exists");

    expect(message.order->price.value() == 150,
           "Limit - Price");
}
}