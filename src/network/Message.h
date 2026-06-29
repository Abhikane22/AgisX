#pragma once

#include <memory>
#include <string>

#include "../core/Enums.h"
#include "../core/Types.h"
#include "../orderbook/Order.h"

struct Message
{
    // Type of client request
    MessageType type;

    // Stock symbol
    std::string symbol;

    // Present only for PLACE_ORDER
    std::unique_ptr<Order> order;

    // Present only for CANCEL_ORDER
    OrderId orderId = 0;
};