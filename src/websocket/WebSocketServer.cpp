#include "WebSocketServer.h"
#include "WebSocketSession.h"

WebSocketServer::WebSocketServer(
    boost::asio::io_context& ioContext,
    unsigned short port,
    TradeDispatcher& dispatcher)
    :
    acceptor(
        ioContext,
        tcp::endpoint(
            tcp::v4(),
            port)),
    tradeDispatcher(dispatcher)
{
    doAccept();
}
void WebSocketServer::doAccept()
{
    acceptor.async_accept(
        [this]
        (
            boost::system::error_code ec,
            boost::asio::ip::tcp::socket socket
        )
        {
            if (!ec)
            {
                auto session =
                        std::make_shared<WebSocketSession>(
                            std::move(socket));

                    tradeDispatcher.subscribe(session);
                    session->start();
            }

            doAccept();
        });
}