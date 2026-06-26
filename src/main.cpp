#include <iostream>

#include "orderbook/Order.h"

int main()
{
    try
    {
        Order order1(
            1,
            Side::BUY,
            OrderType::LIMIT,
            100,
            150);

        Order order2(
            2,
            Side::SELL,
            OrderType::MARKET,
            50);

        std::cout << order1 << std::endl;
        std::cout << order2 << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}