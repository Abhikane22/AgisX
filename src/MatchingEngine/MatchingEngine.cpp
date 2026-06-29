#include "MatchingEngine.h"

#include <algorithm>
#include <stdexcept>

//------------------------------------------------------------
// Constructor
//------------------------------------------------------------

MatchingEngine::MatchingEngine()
    : lastTradePrice(std::nullopt)
{
}

//------------------------------------------------------------
// Trade History
//------------------------------------------------------------

const std::vector<Trade>&
MatchingEngine::tradeHistory() const
{
    return history;
}

//------------------------------------------------------------
// Snapshot
//------------------------------------------------------------

std::string
MatchingEngine::snapshot() const
{
    return orderBook.snapshot();
}

//------------------------------------------------------------
// Best Bid
//------------------------------------------------------------

Price
MatchingEngine::bestBid() const
{
    return orderBook.bestBid();
}

//------------------------------------------------------------
// Best Ask
//------------------------------------------------------------

Price
MatchingEngine::bestAsk() const
{
    return orderBook.bestAsk();
}

//------------------------------------------------------------
// Cancel Order
//------------------------------------------------------------

bool
MatchingEngine::cancelOrder(OrderId id)
{
    return orderBook.removeOrder(id);
}
std::vector<Trade>
MatchingEngine::match(std::unique_ptr<Order>& incoming)
{
    std::vector<Trade> trades;

    Side opposite =
        (incoming->side == Side::BUY)
            ? Side::SELL
            : Side::BUY;

    while (!incoming->filled())
    {
        Order* resting = nullptr;

        if (opposite == Side::BUY)
        {
            resting = orderBook.bestBidOrder();
        }
        else
        {
            resting = orderBook.bestAskOrder();
        }

        if (resting == nullptr)
        {
            break;
        }

        // Price crossing check...

        if (incoming->type == OrderType::LIMIT)
        {
            bool crossed = false;

            if (incoming->side == Side::BUY)
            {
                crossed =
                    incoming->price.value() >=
                    resting->price.value();
            }
            else
            {
                crossed =
                    incoming->price.value() <=
                    resting->price.value();
            }

            if (!crossed)
            {
                break;
            }
        }

        // Quantity
        Quantity tradedQuantity =
            std::min(
                incoming->remainingQuantity,
                resting->remainingQuantity);

        // Execution price
        Price executionPrice =
            resting->price.value();

        // Trade
        if (incoming->side == Side::BUY)
        {
            trades.emplace_back(
                incoming->id,
                resting->id,
                tradedQuantity,
                executionPrice);
        }
        else
        {
            trades.emplace_back(
                resting->id,
                incoming->id,
                tradedQuantity,
                executionPrice);
        }

        lastTradePrice = executionPrice;

        OrderId restingId = resting->id;

        orderBook.consumeOrder(
            restingId,
            tradedQuantity);

        incoming->reduceQuantity(
            tradedQuantity);
    }

    history.insert(
        history.end(),
        trades.begin(),
        trades.end());

    return trades;
}

std::vector<Trade>
MatchingEngine::placeLimitOrder(
    std::unique_ptr<Order> order)
{
    if (!order)
    {
        throw std::invalid_argument(
            "Null order.");
    }

    if (order->type != OrderType::LIMIT)
    {
        throw std::invalid_argument(
            "Expected LIMIT order.");
    }

    auto trades = match(order);

    //--------------------------------------------------
    // Remaining quantity becomes resting liquidity
    //--------------------------------------------------

    if (!order->filled())
    {
        orderBook.addOrder(std::move(order));
    }

    return trades;
}
std::vector<Trade>
MatchingEngine::placeMarketOrder(
    std::unique_ptr<Order> order)
{
    if (!order)
    {
        throw std::invalid_argument(
            "Null order.");
    }

    if (order->type != OrderType::MARKET)
    {
        throw std::invalid_argument(
            "Expected MARKET order.");
    }

    return match(order);
}