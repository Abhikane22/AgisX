#include "Client.h"

#include <iostream>

Client::Client(
    const std::string& host,
    unsigned short port)
    : socket(ioContext),
      resolver(ioContext),
      host(host),
      port(port)
{
}
void
Client::connect()
{
    auto endpoints =
        resolver.resolve(
            host,
            std::to_string(port));

    boost::asio::connect(
        socket,
        endpoints);

    std::cout
        << "Connected to "
        << host
        << ":"
        << port
        << '\n';
}
void
Client::sendCommand(
    const std::string& command)
{
    boost::asio::write(
        socket,
        boost::asio::buffer(command));
}
std::string
Client::receiveResponse()
{
    char buffer[1024];

    std::size_t length =
        socket.read_some(
            boost::asio::buffer(buffer));

    return std::string(
        buffer,
        length);
}
void
Client::run()
{
    connect();

    while (true)
    {
        std::cout << "> ";

        std::string command;

        std::getline(
            std::cin,
            command);

        if (command == "exit")
        {
            break;
        }

        sendCommand(command);

        std::cout
            << receiveResponse()
            << '\n';
    }
}