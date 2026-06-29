#pragma once

enum class Side
{
    BUY,
    SELL
};

enum class OrderType
{
    LIMIT,
    MARKET
};

enum class MessageType
{
    PLACE_ORDER,
    CANCEL_ORDER,
    SNAPSHOT,
    BEST_BID,
    BEST_ASK,
    TRADE_HISTORY
};