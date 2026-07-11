#include "HttpServer.h"

#include "HttpSession.h"

HttpServer::HttpServer(
    boost::asio::io_context& ioContext,
    unsigned short port,
    Exchange& exchange)
    : acceptor(
          ioContext,
          boost::asio::ip::tcp::endpoint(
    boost::asio::ip::tcp::v4(),
    port)),
      exchange(exchange)
{
    doAccept();
}

void HttpServer::doAccept()
{
    acceptor.async_accept(
        [this](
            boost::system::error_code ec,
            boost::asio::ip::tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<HttpSession>(
                    std::move(socket),exchange)
                    ->start();
            }

            doAccept();
        });
}