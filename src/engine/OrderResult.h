#pragma once

#include <vector>

#include "../core/Types.h"
#include "../orderbook/Trade.h"

struct OrderResult
{
    OrderId orderId;

    std::vector<Trade> trades;
};