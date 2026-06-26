cmake_minimum_required(VERSION 3.16)

project(OPTIMUMDAQ)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    OPTIMUMDAQ

    src/main.cpp

    src/orderbook/Order.cpp
)

target_include_directories(
    OPTIMUMDAQ
    PRIVATE
    src
)