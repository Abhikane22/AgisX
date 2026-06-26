#pragma once

#include <chrono>
#include <optional>
#include <iostream>

#include "../core/Types.h"
#include "../core/Enums.h"

// Forward declaration to avoid circular dependency.
struct Limit;

/*
 * Represents a single order submitted to the exchange.
 */
struct Order
{
    // ---------------- Identity ----------------
    OrderId id;

    // ------------ Trading Information ----------
    Side side;
    OrderType type;

    Quantity remainingQuantity;

    // Present only for LIMIT orders.
    std::optional<Price> price;

    // Time when the order entered the exchange.
    std::chrono::steady_clock::time_point entryTime;

    // ---------- Doubly Linked List -------------
    Order* next = nullptr;
    Order* prev = nullptr;

    // -------- Parent Price Level ---------------
    Limit* parent = nullptr;

    // Constructor
    Order(
        OrderId id,
        Side side,
        OrderType type,
        Quantity quantity,
        std::optional<Price> price = std::nullopt);

    // Debug utility
    void print() const;
};

// Stream output operator
std::ostream& operator<<(std::ostream& os, const Order& order);