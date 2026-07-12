#pragma once

#include <boost/asio.hpp>
#include "../marketdata/TradeDispatcher.h"
class WebSocketServer
{
public:

    WebSocketServer(
    boost::asio::io_context& ioContext,
    unsigned short port,
    TradeDispatcher& dispatcher);

private:

    void doAccept();

    boost::asio::ip::tcp::acceptor acceptor;

     TradeDispatcher& tradeDispatcher;
};