#pragma once

#include <boost/asio.hpp>
#include "engine/Exchange.h"

class HttpServer
{
public:
    HttpServer(
        boost::asio::io_context& ioContext,
        unsigned short port,
        Exchange& exchange);

private:
    void doAccept();

    boost::asio::ip::tcp::acceptor acceptor;
    Exchange& exchange;
};