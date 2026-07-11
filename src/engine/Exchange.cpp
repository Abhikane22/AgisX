#include "Exchange.h"

#include <stdexcept>

// Exchange::Exchange() = default;

MatchingEngine&
Exchange::getOrCreateEngine(
    const std::string& symbol)
{
    return engines[symbol];
}

MatchingEngine*
Exchange::findEngine(
    const std::string& symbol)
{
    auto it = engines.find(symbol);

    if (it == engines.end())
    {
        return nullptr;
    }

    return &it->second;
}
const MatchingEngine*
Exchange::findEngine(
    const std::string& symbol) const
{
    auto it = engines.find(symbol);

    if (it == engines.end())
    {
        return nullptr;
    }

    return &it->second;
}
OrderResult
Exchange::placeOrder(
    const std::string& symbol,
    std::unique_ptr<Order> order)
{
    if (!order)
    {
        throw std::invalid_argument(
            "Null order.");
    }

    MatchingEngine& engine =
        getOrCreateEngine(symbol);

    order->id = nextOrderId++;

    OrderResult result;
    result.orderId = order->id;

    if (order->type == OrderType::LIMIT)
    {
        result.trades =
            engine.placeLimitOrder(
                std::move(order));
    }
    else
    {
        result.trades =
            engine.placeMarketOrder(
                std::move(order));
    }

    return result;
}
bool
Exchange::cancelOrder(
    const std::string& symbol,
    OrderId id)
{
    MatchingEngine* engine =
        findEngine(symbol);

    if (engine == nullptr)
    {
        return false;
    }

    return engine->cancelOrder(id);
}
Price
Exchange::bestBid(
    const std::string& symbol) const
{
    const MatchingEngine* engine =
        findEngine(symbol);

    if (engine == nullptr)
    {
        return -1;
    }

    return engine->bestBid();
}
Price
Exchange::bestAsk(
    const std::string& symbol) const
{
    const MatchingEngine* engine =
        findEngine(symbol);

    if (engine == nullptr)
    {
        return -1;
    }

    return engine->bestAsk();
}
std::string
Exchange::snapshot(
    const std::string& symbol) const
{
    const MatchingEngine* engine =
        findEngine(symbol);

    if (engine == nullptr)
    {
        return "Unknown Symbol";
    }

    return engine->snapshot();
}
const std::vector<Trade>&
Exchange::tradeHistory(
    const std::string& symbol) const
{
    const MatchingEngine* engine =
        findEngine(symbol);

    if (engine == nullptr)
    {
        throw std::runtime_error(
            "Unknown symbol.");
    }

    return engine->tradeHistory();
}