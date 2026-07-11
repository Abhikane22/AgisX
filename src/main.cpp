#include <boost/asio.hpp>
#include <iostream>

#include "engine/Exchange.h"
#include "network/TcpServer.h"
#include "logging/Logger.h"
#include "api/HttpServer.h"

int main()
{
    try
    {
        boost::asio::io_context ioContext;

        Exchange exchange;

        constexpr unsigned short PORT = 9090;

        TcpServer server(
            ioContext,
            PORT,
            exchange);
        constexpr unsigned short HTTP_PORT = 8080;

        HttpServer httpServer(
            ioContext,
            HTTP_PORT,
            exchange);
        std::cout
            << "=====================================\n"
            << "        OPTIMUMDAQ EXCHANGE\n"
            << "=====================================\n"
            << "Listening on port "
            << PORT
            << "...\n\n";
        Logger::info(
    "Exchange server started on port " +
    std::to_string(PORT));
            Logger::info(
    "HTTP server started on port " +
    std::to_string(HTTP_PORT));

        ioContext.run();
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Fatal Error: "
            << e.what()
            << '\n';

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}