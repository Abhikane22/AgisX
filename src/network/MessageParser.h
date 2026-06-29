#pragma once

#include <string>

#include "Message.h"

class MessageParser
{
public:

    Message
    parse(
        const std::string& text) const;

private:

    Message
    parsePlaceOrder(
        const std::string& text) const;

    Message
    parseCancelOrder(
        const std::string& text) const;

    Message
    parseSnapshot(
        const std::string& text) const;

    Message
    parseBestBid(
        const std::string& text) const;

    Message
    parseBestAsk(
        const std::string& text) const;

    Message
    parseTradeHistory(
        const std::string& text) const;
};