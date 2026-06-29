#pragma once

#include <boost/asio.hpp>

#include "../engine/Exchange.h"

class TcpServer
{
public:

    TcpServer(
        boost::asio::io_context& ioContext,
        unsigned short port,
        Exchange& exchange);

private:

    void doAccept();

private:

    boost::asio::ip::tcp::acceptor acceptor;

    Exchange& exchange;
};