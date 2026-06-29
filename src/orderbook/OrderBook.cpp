#include "OrderBook.h"

#include <sstream>
#include <stdexcept>

Limit* OrderBook::getOrCreateLimit(Side side, Price price)
{
    if (side == Side::BUY)
    {
        auto it = buyLevels.find(price);

        if (it != buyLevels.end())
        {
            return it->second.get();
        }

        auto limit = std::make_unique<Limit>(price);

        Limit* rawLimit = limit.get();

        buyLevels.emplace(price, std::move(limit));

        return rawLimit;
    }

    auto it = sellLevels.find(price);

    if (it != sellLevels.end())
    {
        return it->second.get();
    }

    auto limit = std::make_unique<Limit>(price);

    Limit* rawLimit = limit.get();

    sellLevels.emplace(price, std::move(limit));

    return rawLimit;
}

void OrderBook::removeEmptyLimit(Side side, Price price)
{
    if (side == Side::BUY)
    {
        auto it = buyLevels.find(price);

        if (it != buyLevels.end() && it->second->empty())
        {
            buyLevels.erase(it);
        }
    }
    else
    {
        auto it = sellLevels.find(price);

        if (it != sellLevels.end() && it->second->empty())
        {
            sellLevels.erase(it);
        }
    }
}

void OrderBook::addOrder(std::unique_ptr<Order> order)
{
    if (!order)
    {
        throw std::invalid_argument("Cannot insert a null order.");
    }

    if (!order->price.has_value())
    {
        throw std::invalid_argument(
            "Market orders cannot be inserted into the OrderBook.");
    }

    Order* rawOrder = order.get();

    Limit* limit = getOrCreateLimit(
        rawOrder->side,
        rawOrder->price.value());

    limit->addOrder(rawOrder);

    orders.emplace(rawOrder->id, std::move(order));
}

Order* OrderBook::findOrder(OrderId id) const
{
    auto it = orders.find(id);

    if (it == orders.end())
    {
        return nullptr;
    }

    return it->second.get();
}
Order* OrderBook::bestBidOrder() const
{
    if (buyLevels.empty())
    {
        return nullptr;
    }

    return buyLevels.begin()->second->head;
}
Order* OrderBook::bestAskOrder() const
{
    if (sellLevels.empty())
    {
        return nullptr;
    }

    return sellLevels.begin()->second->head;
}

bool OrderBook::removeOrder(OrderId id)
{
    auto it = orders.find(id);

    if (it == orders.end())
    {
        return false;
    }

    Order* order = it->second.get();

    if (order->parent == nullptr)
    {
        return false;
    }

    Limit* limit = order->parent;

    Price price = limit->price;
    Side side = order->side;

    limit->removeOrder(order);

    removeEmptyLimit(side, price);

    orders.erase(it);

    return true;
}

size_t OrderBook::size() const
{
    return orders.size();
}

bool OrderBook::consumeOrder(OrderId id, Quantity qty)
{
    Order* order = findOrder(id);

    if (!order)
    {
        return false;
    }

    Limit* limit = order->parent;

    order->reduceQuantity(qty);

    limit->reduceVolume(qty);

    if (order->filled())
    {
        removeOrder(id);
        return true;
    }

    return false;
}
Price OrderBook::bestBid() const
{
    if (buyLevels.empty())
    {
        return -1;
    }

    return buyLevels.begin()->first;
}

Price OrderBook::bestAsk() const
{
    if (sellLevels.empty())
    {
        return -1;
    }

    return sellLevels.begin()->first;
}
bool OrderBook::empty() const
{
    return orders.empty();
}
Order* OrderBook::bestOrder(Side side) const
{
    return (side == Side::BUY)
        ? bestBidOrder()
        : bestAskOrder();
}

bool OrderBook::empty(Side side) const
{
    return (side == Side::BUY)
        ? buyLevels.empty()
        : sellLevels.empty();
}
std::string OrderBook::snapshot() const
{
    std::ostringstream out;

    out << "==============================\n";
    out << "        ORDER BOOK\n";
    out << "==============================\n\n";

    out << "SELL SIDE\n";

    for (const auto& [price, limit] : sellLevels)
    {
        out << price
            << " | Orders: "
            << limit->orderCount
            << " | Volume: "
            << limit->totalVolume
            << '\n';
    }

    out << "\n------------------------------\n";

    out << "BUY SIDE\n";

    for (const auto& [price, limit] : buyLevels)
    {
        out << price
            << " | Orders: "
            << limit->orderCount
            << " | Volume: "
            << limit->totalVolume
            << '\n';
    }

    out << "==============================\n";

    return out.str();
}