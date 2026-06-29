#include "Trade.h"

Trade::Trade(
    OrderId buyId,
    OrderId sellId,
    Quantity quantity,
    Price price)
    : buyOrderId(buyId),
      sellOrderId(sellId),
      quantity(quantity),
      executionPrice(price),
      timestamp(std::chrono::steady_clock::now())
{
}

std::ostream&
operator<<(std::ostream& os,
           const Trade& trade)
{
    os
        << "Trade { "
        << "Buyer: " << trade.buyOrderId
        << ", Seller: " << trade.sellOrderId
        << ", Quantity: " << trade.quantity
        << ", Price: " << trade.executionPrice
        << " }";

    return os;
}