#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <string>

#include "../core/Types.h"
#include "../core/Enums.h"

#include "Order.h"
#include "Limit.h"

class OrderBook
{
public:
    OrderBook() = default;

    // Insert an order into the book
    void addOrder(std::unique_ptr<Order> order);

    // Remove an order by ID
    bool removeOrder(OrderId id);

    // Lookup
    Order* findOrder(OrderId id) const;

    // Best prices
    Price bestBid() const;
    Price bestAsk() const;

    Order* bestBidOrder() const;
    Order* bestAskOrder() const;

    // Debugging
    std::string snapshot() const;
    size_t size() const;
    bool empty() const;
    // OrderBook.h

    Order* bestOrder(Side side) const;

    bool empty(Side side) const;
    
    // Consume quantity from an existing resting order.
    // Returns true if the order was completely filled and removed.
    bool consumeOrder(OrderId id, Quantity qty);
private:

    // BUY side (highest price first)
    std::map<
        Price,
        std::unique_ptr<Limit>,
        std::greater<Price>
    > buyLevels;

    // SELL side (lowest price first)
    std::map<
        Price,
        std::unique_ptr<Limit>
    > sellLevels;

    // Order ownership
    std::unordered_map<
        OrderId,
        std::unique_ptr<Order>
    > orders;

private:

    Limit* getOrCreateLimit(Side side, Price price);

    void removeEmptyLimit(Side side, Price price);
};