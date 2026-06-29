#include <boost/asio.hpp>
#include <iostream>

#include "engine/Exchange.h"
#include "network/TcpServer.h"

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

        std::cout
            << "=====================================\n"
            << "        OPTIMUMDAQ EXCHANGE\n"
            << "=====================================\n"
            << "Listening on port "
            << PORT
            << "...\n\n";

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