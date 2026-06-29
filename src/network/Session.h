#pragma once

#include <boost/asio.hpp>

#include <array>
#include <memory>

#include "../engine/Exchange.h"
#include "MessageParser.h"

class Session :
    public std::enable_shared_from_this<Session>
{
public:

    Session(
        boost::asio::ip::tcp::socket socket,
        Exchange& exchange);

    void start();

private:

    void doRead();

    void processMessage(
        const std::string& text);

    void doWrite(
        const std::string& response);

private:

    boost::asio::ip::tcp::socket socket;

    Exchange& exchange;

    MessageParser parser;

    std::array<char, 1024> buffer;
};