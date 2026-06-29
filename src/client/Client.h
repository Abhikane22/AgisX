#pragma once

#include <boost/asio.hpp>

#include <string>

class Client
{
public:

    Client(
        const std::string& host,
        unsigned short port);

    void run();

private:

    void connect();

    void sendCommand(
        const std::string& command);

    std::string receiveResponse();

private:

    boost::asio::io_context ioContext;

    boost::asio::ip::tcp::socket socket;

    boost::asio::ip::tcp::resolver resolver;

    std::string host;

    unsigned short port;
};