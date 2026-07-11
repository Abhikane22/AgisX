#pragma once

#include <memory>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include "engine/Exchange.h"
class HttpSession :
    public std::enable_shared_from_this<HttpSession>
{
public:
    explicit HttpSession(
        boost::asio::ip::tcp::socket socket,Exchange& exchange);

    void start();

private:
    void doRead();

    void handleRequest();

    void doWrite();

    void handleGetRoot();

    void handlePostOrders();

private:
    boost::beast::tcp_stream stream;
    Exchange& exchange;

    boost::beast::flat_buffer buffer;

    boost::beast::http::request<
        boost::beast::http::string_body> request;

    boost::beast::http::response<
        boost::beast::http::string_body> response;
};