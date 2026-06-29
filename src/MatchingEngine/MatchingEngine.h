#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "../core/Types.h"
#include "../orderbook/OrderBook.h"
#include "../orderbook/Trade.h"

class MatchingEngine
{
public:

    MatchingEngine();

    std::vector<Trade>
    placeLimitOrder(std::unique_ptr<Order> order);

    std::vector<Trade>
    placeMarketOrder(std::unique_ptr<Order> order);

    bool cancelOrder(OrderId id);

    Price bestBid() const;
    Price bestAsk() const;

    std::string snapshot() const;

    const std::vector<Trade>&
    tradeHistory() const;

private:

    std::vector<Trade>
    match(std::unique_ptr<Order>& incoming);

private:

    OrderBook orderBook;

    std::vector<Trade> history;

    std::optional<Price> lastTradePrice;
};