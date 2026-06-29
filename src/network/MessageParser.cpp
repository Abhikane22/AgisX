#include "MessageParser.h"

#include <sstream>
#include <stdexcept>

Message
MessageParser::parse(
    const std::string& text) const
{
    std::istringstream input(text);

    std::string command;
    input >> command;

    if (command == "PLACE")
    {
        return parsePlaceOrder(text);
    }

    if (command == "CANCEL")
    {
        return parseCancelOrder(text);
    }

    if (command == "SNAPSHOT")
    {
        return parseSnapshot(text);
    }

    if (command == "BEST_BID")
    {
        return parseBestBid(text);
    }

    if (command == "BEST_ASK")
    {
        return parseBestAsk(text);
    }

    if (command == "TRADE_HISTORY")
    {
        return parseTradeHistory(text);
    }

    throw std::invalid_argument(
        "Unknown command.");
}
Message
MessageParser::parsePlaceOrder(
    const std::string& text) const
{
    std::istringstream input(text);

    std::string command;
    std::string symbol;
    std::string sideString;
    std::string typeString;

    input
        >> command
        >> symbol
        >> sideString
        >> typeString;

    Side side;

    if (sideString == "BUY")
    {
        side = Side::BUY;
    }
    else if (sideString == "SELL")
    {
        side = Side::SELL;
    }
    else
    {
        throw std::invalid_argument(
            "Invalid side.");
    }

    Message message;

    message.type = MessageType::PLACE_ORDER;
    message.symbol = symbol;

    if (typeString == "LIMIT")
    {
        Quantity quantity;
        Price price;

        input >> quantity >> price;

        message.order =
            std::make_unique<Order>(
                0,
                side,
                OrderType::LIMIT,
                quantity,
                price);
    }
    else if (typeString == "MARKET")
    {
        Quantity quantity;

        input >> quantity;

        message.order =
            std::make_unique<Order>(
                0,
                side,
                OrderType::MARKET,
                quantity);
    }
    else
    {
        throw std::invalid_argument(
            "Invalid order type.");
    }

    return message;
}
Message
MessageParser::parseCancelOrder(
    const std::string& text) const
{
    std::istringstream input(text);

    std::string command;

    Message message;

    input
        >> command
        >> message.symbol
        >> message.orderId;

    message.type = MessageType::CANCEL_ORDER;

    return message;
}
Message
MessageParser::parseSnapshot(
    const std::string& text) const
{
    std::istringstream input(text);

    std::string command;

    Message message;

    input
        >> command
        >> message.symbol;

    message.type = MessageType::SNAPSHOT;

    return message;
}
Message
MessageParser::parseBestBid(
    const std::string& text) const
{
    std::istringstream input(text);

    std::string command;

    Message message;

    input
        >> command
        >> message.symbol;

    message.type = MessageType::BEST_BID;

    return message;
}
Message
MessageParser::parseBestAsk(
    const std::string& text) const
{
    std::istringstream input(text);

    std::string command;

    Message message;

    input
        >> command
        >> message.symbol;

    message.type = MessageType::BEST_ASK;

    return message;
}
Message
MessageParser::parseTradeHistory(
    const std::string& text) const
{
    std::istringstream input(text);

    std::string command;

    Message message;

    input
        >> command
        >> message.symbol;

    message.type = MessageType::TRADE_HISTORY;

    return message;
}

