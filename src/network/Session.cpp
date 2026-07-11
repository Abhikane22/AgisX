#include "Session.h"

Session::Session(
    boost::asio::ip::tcp::socket socket,
    Exchange& exchange)
    : socket(std::move(socket)),
      exchange(exchange)
{
}

void
Session::start()
{
    doRead();
}

void
Session::doRead()
{
    auto self = shared_from_this();

    socket.async_read_some(
        boost::asio::buffer(buffer),
        [this, self](
            boost::system::error_code ec,
            std::size_t length)
        {
            if (!ec)
            {
                processMessage(
                    std::string(
                        buffer.data(),
                        length));
            }
        });
}

void
Session::processMessage(
    const std::string& text)
{
    Message message =
        parser.parse(text);

    std::string response;

    switch (message.type)
    {
    case MessageType::PLACE_ORDER:
    {
        auto result =
        exchange.placeOrder(
            message.symbol,
            std::move(message.order));

        response =
            "Order Accepted\n"
            "Order ID: " +
            std::to_string(result.orderId) +
            "\nTrades: " +
            std::to_string(result.trades.size());
        break;
    }

    case MessageType::CANCEL_ORDER:
    {
        bool cancelled =
            exchange.cancelOrder(
                message.symbol,
                message.orderId);

        response =
            cancelled
            ? "Order Cancelled"
            : "Order Not Found";

        break;
    }

    case MessageType::SNAPSHOT:
    {
        response =
            exchange.snapshot(
                message.symbol);

        break;
    }

    case MessageType::BEST_BID:
    {
        response =
            std::to_string(
                exchange.bestBid(
                    message.symbol));

        break;
    }

    case MessageType::BEST_ASK:
    {
        response =
            std::to_string(
                exchange.bestAsk(
                    message.symbol));

        break;
    }

    case MessageType::TRADE_HISTORY:
    {
        response =
            "Trades: " +
            std::to_string(
                exchange.tradeHistory(
                    message.symbol).size());

        break;
    }
    }

    doWrite(response);
}

void
Session::doWrite(
    const std::string& response)
{
    auto self = shared_from_this();

    boost::asio::async_write(
        socket,
        boost::asio::buffer(response),
        [this, self](
            boost::system::error_code ec,
            std::size_t)
        {
            if (!ec)
            {
                doRead();
            }
        });
}