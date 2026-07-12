#include "TradeDispatcher.h"
#include <algorithm>
#include "../websocket/WebSocketSession.h"
#include "../logging/Logger.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;
void TradeDispatcher::subscribe(
    const std::shared_ptr<WebSocketSession>& session)
{
    Logger::info("WebSocket client subscribed.");
    subscribers.push_back(session);
}
// void TradeDispatcher::unsubscribeExpired()
// {
//     subscribers.erase(
//         std::remove_if(
//             subscribers.begin(),
//             subscribers.end(),
//             [](const auto& session)
//             {
//                 return session.expired();
//             }),
//         subscribers.end());
// }
void TradeDispatcher::broadcast(
    const std::string& message)
{
    for (auto& session : subscribers)
    {
        session->send(message);
    }
}
void TradeDispatcher::broadcastTrade(
    const Trade& trade)
{
    json message;
    Logger::info("broadcastTrade() called");

    message["type"] = "trade";

    message["buyOrderId"] =
        trade.buyOrderId;

    message["sellOrderId"] =
        trade.sellOrderId;

    message["price"] =
        trade.executionPrice;

    message["quantity"] =
        trade.quantity;

    broadcast(
        message.dump());
}