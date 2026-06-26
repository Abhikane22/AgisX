#include "Order.h"

#include <stdexcept>

// Constructor
Order::Order(
    OrderId id,
    Side side,
    OrderType type,
    Quantity quantity,
    std::optional<Price> price)
    : id(id),
      side(side),
      type(type),
      remainingQuantity(quantity),
      price(price),
      entryTime(std::chrono::steady_clock::now())
{
    // Quantity must be positive
    if (quantity <= 0)
    {
        throw std::invalid_argument(
            "Order quantity must be greater than zero.");
    }

    // LIMIT orders must contain a valid price
    if (type == OrderType::LIMIT)
    {
        if (!price.has_value())
        {
            throw std::invalid_argument(
                "Limit order must contain a price.");
        }

        if (price.value() <= 0)
        {
            throw std::invalid_argument(
                "Price must be greater than zero.");
        }
    }

    // MARKET orders should not contain a price
    if (type == OrderType::MARKET)
    {
        if (price.has_value())
        {
            throw std::invalid_argument(
                "Market order cannot contain a limit price.");
        }
    }
}

void Order::print() const
{
    std::cout << *this << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Order& order)
{
    os << "Order { ";

    os << "ID: " << order.id;

    os << ", Side: "
       << (order.side == Side::BUY ? "BUY" : "SELL");

    os << ", Type: "
       << (order.type == OrderType::LIMIT ? "LIMIT" : "MARKET");

    os << ", Quantity: "
       << order.remainingQuantity;

    if (order.price.has_value())
    {
        os << ", Price: "
           << order.price.value();
    }
    else
    {
        os << ", Price: MARKET";
    }

    os << " }";

    return os;
}