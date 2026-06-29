#pragma once

#include <chrono>
#include <iostream>

#include "../core/Types.h"

struct Trade
{
    OrderId buyOrderId;
    OrderId sellOrderId;

    Quantity quantity;

    Price executionPrice;

    std::chrono::steady_clock::time_point timestamp;

    Trade(
        OrderId buyId,
        OrderId sellId,
        Quantity quantity,
        Price price);

};

std::ostream& operator<<(std::ostream& os,
                         const Trade& trade);