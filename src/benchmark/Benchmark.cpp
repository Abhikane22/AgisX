#include "Benchmark.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <random>

#include "../engine/Exchange.h"
#include "../orderbook/Order.h"
const std::string symbol = "AAPL";
namespace
{
    constexpr int NUM_ORDERS = 100000;

    constexpr Price MIN_PRICE = 100;
    constexpr Price MAX_PRICE = 200;

    constexpr Quantity MIN_QTY = 1;
    constexpr Quantity MAX_QTY = 100;
}

void Benchmark::run()
{
    Exchange exchange;

    std::mt19937 rng(std::random_device{}());

    std::uniform_int_distribution<int> sideDist(0,1);
    std::uniform_int_distribution<int> priceDist(MIN_PRICE, MAX_PRICE);
    std::uniform_int_distribution<int> qtyDist(MIN_QTY, MAX_QTY);

    std::size_t tradesExecuted = 0;

    auto start =
        std::chrono::steady_clock::now();

    // Generate Orders
    for (OrderId id = 1; id <= NUM_ORDERS; ++id)
{
    Side side =
        sideDist(rng) == 0
        ? Side::BUY
        : Side::SELL;

    Price price =
        priceDist(rng);

    Quantity quantity =
        qtyDist(rng);

    auto order =
        std::make_unique<Order>(
            id,
            side,
            OrderType::LIMIT,
            quantity,
            price);

    auto result =
    exchange.placeOrder(
        symbol,
        std::move(order));

    tradesExecuted += result.trades.size();
}

    auto end =
        std::chrono::steady_clock::now();

    auto duration =
        std::chrono::duration_cast<
            std::chrono::milliseconds>(
                end-start);

    std::cout
        << "\n========== BENCHMARK ==========\n";

    std::cout
        << "Orders : "
        << NUM_ORDERS
        << '\n';

    std::cout
        << "Trades : "
        << tradesExecuted
        << '\n';

    std::cout
        << "Time : "
        << duration.count()
        << " ms\n";

    std::cout
        << "Orders/sec : "
        << (NUM_ORDERS * 1000.0 / duration.count())
        << '\n';

    std::cout
        << "===============================\n";
}