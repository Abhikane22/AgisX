#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../MatchingEngine/MatchingEngine.h"
#include "../orderbook/Order.h"
#include "../orderbook/Trade.h"
#include "OrderResult.h"
class Exchange
{
public:

    Exchange() = default;

    // Route an order to the corresponding matching engine.
    OrderResult
    placeOrder(
        const std::string& symbol,
        std::unique_ptr<Order> order);
    // Cancel an existing order.
    bool
    cancelOrder(
        const std::string& symbol,
        OrderId id);

    // Market data.
    Price
    bestBid(
        const std::string& symbol) const;

    Price
    bestAsk(
        const std::string& symbol) const;

    std::string
    snapshot(
        const std::string& symbol) const;

    // Executed trades.
    const std::vector<Trade>&
    tradeHistory(
        const std::string& symbol) const;
private:
    OrderId nextOrderId = 1;
private:

    // Creates a matching engine if one does not exist.
    MatchingEngine&
    getOrCreateEngine(
        const std::string& symbol);

    // Non-const lookup.
    MatchingEngine*
    findEngine(
        const std::string& symbol);

    // Const lookup.
    const MatchingEngine*
    findEngine(
        const std::string& symbol) const;

private:

    std::unordered_map<
        std::string,
        MatchingEngine
    > engines;
};