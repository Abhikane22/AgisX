#include "WebSocketSession.h"
#include "logging/Logger.h"
WebSocketSession::WebSocketSession(
    tcp::socket socket)
    : ws(std::move(socket))
{
}
void WebSocketSession::start()
{
    auto self = shared_from_this();

    ws.async_accept(
        [this, self](
            boost::beast::error_code ec)
        {
            onAccept(ec);
        });
}
void WebSocketSession::onAccept(
    boost::beast::error_code ec)
{
   if (ec)
    {
        return;
    }

    send(
    R"({
        "type":"connected",
        "message":"Welcome to AegisX Market Data"
    })");
}
void WebSocketSession::send(
    const std::string& message)
{
    auto self = shared_from_this();
    Logger::info(message);
    ws.async_write(
        boost::asio::buffer(message),
        [this, self]
        (
            boost::beast::error_code ec,
            std::size_t bytesTransferred
        )
        {
            onWrite(ec, bytesTransferred);
        });
}
void WebSocketSession::onWrite(
    boost::beast::error_code ec,
    std::size_t)
{
    if (ec)
    {
        Logger::error(
            "WebSocket write failed: " +
            ec.message());

        return;
    }

    Logger::info("WebSocket write completed");
}