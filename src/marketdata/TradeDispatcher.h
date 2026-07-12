#pragma once

#include <memory>
#include <vector>
#include "../orderbook/Trade.h"

class WebSocketSession;

class TradeDispatcher
{
public:

    void subscribe(
        const std::shared_ptr<WebSocketSession>& session);

    void unsubscribeExpired();

    void broadcast(
        const std::string& message);
    
    
    void
    broadcastTrade(
        const Trade& trade);

private:

    std::vector<
    std::shared_ptr<WebSocketSession>
> subscribers;
};