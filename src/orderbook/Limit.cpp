#include "Limit.h"
#include "Order.h"

Limit::Limit(Price price)
    : price(price)
{
}

void Limit::addOrder(Order* order)
{
    // Empty price level
    if (head == nullptr)
    {
        head = order;
        tail = order;
    }
    else
    {
        tail->next = order;
        order->prev = tail;
        tail = order;
    }

    order->parent = this;

    ++orderCount;
    totalVolume += order->remainingQuantity;
}
void Limit::removeOrder(Order* order)
{
    // Connect previous node
    if (order->prev != nullptr)
    {
        order->prev->next = order->next;
    }
    else
    {
        head = order->next;
    }

    // Connect next node
    if (order->next != nullptr)
    {
        order->next->prev = order->prev;
    }
    else
    {
        tail = order->prev;
    }

    --orderCount;
    totalVolume -= order->remainingQuantity;

    // Disconnect removed order
    order->next = nullptr;
    order->prev = nullptr;
    order->parent = nullptr;
}
void Limit::reduceVolume(Quantity qty)
{
    if (qty > totalVolume)
    {
        throw std::runtime_error("Volume underflow.");
    }

    totalVolume -= qty;
}

bool Limit::empty() const
{
    return orderCount == 0;
}