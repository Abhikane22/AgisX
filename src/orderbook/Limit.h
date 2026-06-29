#pragma once

#include <cstddef>

#include "../core/Types.h"

// Forward declaration
struct Order;

/*
 * Represents a single price level in the order book.
 * Maintains all orders at this price in FIFO order.
 */
struct Limit
{
    // Price represented by this level
    Price price;

    // Number of active orders
    std::size_t orderCount = 0;

    // Total shares at this price level
    Volume totalVolume = 0;

    // Doubly-linked list
    Order* head = nullptr;
    Order* tail = nullptr;

    explicit Limit(Price price);

    // Insert order at the end (FIFO)
    void addOrder(Order* order);

    // Remove an existing order
    void removeOrder(Order* order);
    void reduceVolume(Quantity qty);

    // Is this price level empty?
    bool empty() const;

    // Disable copying
    Limit(const Limit&) = delete;
    Limit& operator=(const Limit&) = delete;
};