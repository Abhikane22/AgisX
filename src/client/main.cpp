#include "Client.h"

#include <iostream>

int main()
{
    try
    {
        Client client(
            "127.0.0.1",
            9090);

        client.run();
    }
    catch (const std::exception& e)
    {
        std::cerr
            << e.what()
            << '\n';
    }
}