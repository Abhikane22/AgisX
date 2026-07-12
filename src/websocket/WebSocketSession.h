 #pragma once

#include <boost/beast/websocket.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <memory>

namespace websocket = boost::beast::websocket;
using tcp = boost::asio::ip::tcp;

class WebSocketSession
    : public std::enable_shared_from_this<WebSocketSession>
{
public:

    explicit WebSocketSession(
        tcp::socket socket);

    void start();
    void send(
    const std::string& message);

private:

    void onAccept(
        boost::beast::error_code ec);
    void onWrite(
        boost::beast::error_code ec,
        std::size_t bytesTransferred);
    websocket::stream<tcp::socket> ws;

    boost::beast::flat_buffer buffer;
};