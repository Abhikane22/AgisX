cmake_minimum_required(VERSION 3.16)

project(
    optimumDaq
    VERSION 1.0
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED)

set(SOURCES

    src/main.cpp

    src/orderbook/Order.cpp
    src/orderbook/Limit.cpp
    src/orderbook/Trade.cpp
    src/orderbook/OrderBook.cpp

    src/MatchingEngine/MatchingEngine.cpp

    src/engine/Exchange.cpp

    src/network/MessageParser.cpp
    src/network/Session.cpp
    src/network/TcpServer.cpp
    src/logging/Logger.cpp

    src/api/HttpServer.cpp
    src/api/HttpSession.cpp
)

add_executable(
    optimumdaq
    ${SOURCES}
)

target_include_directories(
    optimumdaq
    PRIVATE
        src
)

target_link_libraries(
    optimumdaq
    PRIVATE
        Boost::headers
)
set(TEST_SOURCES

    src/tests/TestRunner.cpp
    src/tests/TestUtils.cpp

    src/tests/OrderTests.cpp
    src/tests/LimitTests.cpp
    src/tests/OrderBookTests.cpp
    src/tests/MatchingEngineTests.cpp
    src/tests/ExchangeTests.cpp
    src/tests/MessageParserTests.cpp

    src/orderbook/Order.cpp
    src/orderbook/Limit.cpp
    src/orderbook/Trade.cpp
    src/orderbook/OrderBook.cpp

    src/MatchingEngine/MatchingEngine.cpp

    src/engine/Exchange.cpp

    src/network/MessageParser.cpp
    src/network/Session.cpp
    src/network/TcpServer.cpp

    src/logging/Logger.cpp
)
set(BENCHMARK_SOURCES

    src/benchmark/main.cpp
    src/benchmark/Benchmark.cpp

    src/orderbook/Order.cpp
    src/orderbook/Limit.cpp
    src/orderbook/Trade.cpp
    src/orderbook/OrderBook.cpp

    src/MatchingEngine/MatchingEngine.cpp

    src/engine/Exchange.cpp

    src/logging/Logger.cpp
)
add_executable(
    optimumdaq_benchmark
    ${BENCHMARK_SOURCES}
    # optimumdaq_client
    # src/client/main.cpp
    # src/client/Client.cpp
)

target_include_directories(
    #optimumdaq_client
    optimumdaq_benchmark
    PRIVATE
        src
)

# target_link_libraries(
#     optimumdaq_client
#     PRIVATE
#         Boost::headers
# )