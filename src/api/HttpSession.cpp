#include "HttpSession.h"

#include <boost/beast/version.hpp>
#include <nlohmann/json.hpp>
#include <sstream>

using json = nlohmann::json;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;
std::vector<std::string>
HttpSession::splitPath(
    const std::string& path) const
{
    std::vector<std::string> parts;

    std::stringstream stream(path);

    std::string part;

    while (std::getline(stream, part, '/'))
    {
        if (!part.empty())
        {
            parts.push_back(part);
        }
    }

    return parts;
}
HttpSession::HttpSession(
    tcp::socket socket,
    Exchange& exchange)
    : stream(std::move(socket)),
      exchange(exchange)
{
}

void HttpSession::start()
{
    doRead();
}

void HttpSession::doRead()
{
    auto self = shared_from_this();

    request = {};

    http::async_read(
        stream,
        buffer,
        request,
        [this, self](
            beast::error_code ec,
            std::size_t)
        {
            if (ec)
            {
                return;
            }

            handleRequest();
        });
}

void HttpSession::handleRequest()
{
    response = {};

    auto parts =
        splitPath(
            std::string(request.target()));

    response.version(request.version());
    response.set(http::field::server, "AegisX");
    response.set(http::field::content_type, "text/plain");

    if (request.method() == http::verb::get &&
        request.target() == "/")
    {
        handleGetRoot();
    }
    else if (request.method() == http::verb::post &&
             request.target() == "/orders")
    {
        handlePostOrders();
    }
    else if (request.method() == http::verb::delete_)
    {
        handleDeleteOrder(parts);
    }
    else
    {
        response.result(http::status::not_found);
        response.body() = "404 - Not Found";
    }

    response.prepare_payload();

    doWrite();
}
void HttpSession::handleDeleteOrder(
        const std::vector<std::string>& parts)
    {
        response.set(
            http::field::content_type,
            "application/json");

        if (parts.size() != 3 ||
            parts[0] != "orders")
        {
            response.result(http::status::bad_request);
            response.body() = R"({
        "status":"error",
        "message":"Invalid URL"
    })";
            return;
        }

        const std::string symbol = parts[1];

        OrderId orderId =
            std::stoll(parts[2]);

        bool cancelled =
            exchange.cancelOrder(
                symbol,
                orderId);

        json result;

        if (cancelled)
        {
            response.result(http::status::ok);
            result["status"] = "cancelled";
        }
        else
        {
            response.result(http::status::not_found);
            result["status"] = "not_found";
        }

        response.body() =
                result.dump(4);
}
void HttpSession::doWrite()
{
    auto self = shared_from_this();

    http::async_write(
        stream,
        response,
        [this, self](
            beast::error_code ec,
            std::size_t)
        {
            beast::error_code ignored;

            stream.socket().shutdown(
                tcp::socket::shutdown_send,
                ignored);
        });
}
void HttpSession::handleGetRoot()
{
    response.result(http::status::ok);
    response.body() = "AegisX Exchange Running";
}

void HttpSession::handlePostOrders()
{
    response.set(
        http::field::content_type,
        "application/json");

    try
    {
        json body = json::parse(request.body());

        const std::string symbol = body.at("symbol");

        Side side =
            body.at("side") == "BUY"
                ? Side::BUY
                : Side::SELL;

        OrderType type =
            body.at("type") == "LIMIT"
                ? OrderType::LIMIT
                : OrderType::MARKET;

        Quantity quantity =
            body.at("quantity");

        std::optional<Price> price;

        if (type == OrderType::LIMIT)
        {
            price = body.at("price");
        }

        auto order =
            std::make_unique<Order>(
                0,
                side,
                type,
                quantity,
                price);

        auto result =
            exchange.placeOrder(
                symbol,
                std::move(order));

        json responseJson;

        responseJson["status"] = "accepted";
        responseJson["orderId"] = result.orderId;
        responseJson["trades"] = result.trades.size();

        response.result(http::status::ok);
        response.body() = responseJson.dump(4);

    }
    catch (const std::exception& e)
    {
        response.result(
            http::status::bad_request);

        json error;

        error["status"] = "error";
        error["message"] = e.what();

        response.body() = error.dump(4);
    }
}