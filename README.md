# OptimumDaq — Low-Latency Electronic Stock Exchange Simulator

> A production-inspired electronic trading engine built in modern C++ that implements a price-time-priority matching engine with TCP, HTTP REST, and WebSocket interfaces.

---

# Overview

OptimumDaq is a low-latency electronic stock exchange simulator that models how modern exchanges receive, match, execute, and broadcast trades.

The project was built to understand the internal architecture of real trading systems rather than simply implementing an order book. It focuses on clean software architecture, efficient data structures, modular design, networking, benchmarking, and interview-level system design.

The exchange currently supports:

* Multiple trading symbols
* Limit Orders
* Market Orders
* Order Cancellation
* Price-Time Priority Matching
* Real-Time Order Book
* TCP Trading Interface
* HTTP REST API
* WebSocket Market Data Streaming
* Trade History
* Performance Benchmarking Framework

The implementation emphasizes correctness, modularity, and extensibility rather than exchange-specific business rules.

---

# Motivation

Most stock exchange tutorials stop after implementing an order book.

Real exchanges involve significantly more engineering:

* Order routing
* Matching engines
* Market data distribution
* Client communication
* Network protocols
* Performance considerations
* Scalability
* Separation of concerns

The objective of this project was to bridge that gap by building a modular exchange from first principles while maintaining clean software engineering practices.

---

# Features

## Matching Engine

* Price-Time Priority Matching
* Partial Fills
* Full Fills
* Market Orders
* Limit Orders
* FIFO execution within each price level

---

## Order Book

* Separate Buy/Sell books
* Automatic creation/removal of price levels
* O(log N) price lookup
* O(1) order insertion inside price level
* O(1) cancellation lookup

---

## Networking

### TCP Server

Interactive trading protocol supporting

PLACE

CANCEL

SNAPSHOT

BEST_BID

BEST_ASK

TRADE_HISTORY

---

### HTTP REST API

Endpoints

GET /

POST /orders

DELETE /orders/{symbol}/{orderId}

JSON request/response handling

---

### WebSocket

Real-time trade broadcasting

Clients automatically receive executed trades as JSON.

Example:

```json
{
    "type":"trade",
    "buyOrderId":1,
    "sellOrderId":2,
    "price":150,
    "quantity":100
}
```

---

## Benchmark

Synthetic workload generator

Measures:

* Orders processed
* Trades executed
* Processing time
* Throughput

---

# System Architecture

```
                    +----------------+
                    |    Clients     |
                    +-------+--------+
                            |
        +-------------------+-------------------+
        |                   |                   |
        |                   |                   |
      TCP                HTTP REST         WebSocket
        |                   |                   |
        |                   |                   |
        +---------+---------+-------------------+
                  |
             Exchange
                  |
          Matching Engine
                  |
             Order Book
                  |
        +---------+---------+
        |                   |
     Buy Book          Sell Book
```

---

# Project Structure

```
src/

api/
    HttpServer
    HttpSession

benchmark/
    Benchmark

client/
    TCP Client

engine/
    Exchange

logging/
    Logger

MatchingEngine/
    MatchingEngine

marketdata/
    TradeDispatcher

network/
    MessageParser
    Session
    TcpServer

orderbook/
    Order
    Limit
    OrderBook
    Trade

tests/

websocket/
    WebSocketServer
    WebSocketSession
```

---

# Core Components

## Exchange

Acts as the central coordinator.

Responsibilities

* Maintains one matching engine per symbol
* Routes incoming orders
* Cancels orders
* Provides market data
* Stores trade history

Exchange does not perform matching itself.

---

## MatchingEngine

Responsible for:

* Matching incoming orders
* Executing trades
* Handling partial fills
* Resting remaining liquidity
* Maintaining execution rules

Each trading symbol owns one MatchingEngine.

---

## OrderBook

Maintains:

Buy Side

Sell Side

Handles:

* Adding orders
* Cancelling orders
* Best Bid
* Best Ask
* Snapshot generation

---

## Order

Represents one user order.

Contains

* Order ID
* Side
* Order Type
* Remaining Quantity
* Price
* Timestamp

Also participates in doubly linked lists for FIFO execution.

---

## Limit

Represents a single price level.

Contains

* Price
* Head Order
* Tail Order
* Total Volume

Supports

O(1)

append

remove

pop front

---

## Trade

Represents one executed trade.

Contains

* Buy Order ID
* Sell Order ID
* Quantity
* Execution Price
* Timestamp

---

# Price-Time Priority

The exchange follows standard price-time priority.

BUY Orders

Highest price executes first.

Within same price

Earliest order executes first.

SELL Orders

Lowest price executes first.

Within same price

Earliest order executes first.

---

# Data Structures

Buy Book

std::map<Price, Limit, std::greater<>>

Reason:

Highest price accessible immediately.

Complexity:

O(log N)

---

Sell Book

std::map<Price, Limit>

Lowest price accessible immediately.

---

Order Lookup

unordered_map<OrderId, Order*>

Allows

O(1)

cancellation.

---

Price Level

Doubly Linked List

Supports

O(1)

append

remove

FIFO

---

# Matching Algorithm

Incoming Order

↓

Find best opposing price

↓

Check if prices cross

↓

Execute trade

↓

Reduce quantities

↓

Remove filled orders

↓

Continue

↓

Rest remaining quantity

---

# Complexity

Insert Order

O(log N)

Cancel Order

O(1)

Best Bid

O(1)

Best Ask

O(1)

Trade Execution

O(K)

K = number of matched resting orders

---

# TCP Protocol

Example

```
PLACE AAPL BUY LIMIT 100 150
```

```
PLACE AAPL SELL MARKET 100
```

```
CANCEL AAPL 42
```

```
SNAPSHOT AAPL
```

```
BEST_BID AAPL
```

```
BEST_ASK AAPL
```

```
TRADE_HISTORY AAPL
```

---

# HTTP API

## Root

GET /

Response

```
AegisX Exchange Running
```

---

## Place Order

POST

```
/orders
```

Example

```json
{
    "symbol":"AAPL",
    "side":"BUY",
    "type":"LIMIT",
    "price":150,
    "quantity":100
}
```

Response

```json
{
    "orderId":1,
    "status":"accepted",
    "trades":0
}
```

---

Cancel

DELETE

```
/orders/AAPL/1
```

Response

```json
{
    "status":"cancelled"
}
```

---

# WebSocket

Connect

```
ws://localhost:8081
```

Welcome message

```json
{
    "type":"connected",
    "message":"Welcome to AegisX Market Data"
}
```

Trade Broadcast

```json
{
    "type":"trade",
    "buyOrderId":1,
    "sellOrderId":2,
    "price":150,
    "quantity":100
}
```

---

# Logging

Logger provides

INFO

ERROR

Console diagnostics

Useful during

networking

matching

broadcasting

debugging

---

# Testing

Project includes automated unit tests for

* Order
* Limit
* OrderBook
* Matching Engine
* Exchange
* Message Parser

Testing strategy focused on validating correctness before introducing larger architectural changes.

---

# Benchmark

Synthetic orders are generated using random:

* Side
* Price
* Quantity

Metrics collected

* Orders processed
* Trades executed
* Total execution time
* Orders per second

Purpose

Evaluate matching engine throughput under heavy workloads.

---

# Design Decisions

## Why std::map?

Provides sorted ordering.

Allows immediate access to

Best Bid

Best Ask

without scanning.

---

## Why unordered_map?

Required for fast cancellation.

Searching entire order book would be O(N).

unordered_map provides average O(1).

---

## Why Doubly Linked List?

Orders leave the middle of a price level after cancellation.

Singly linked list would require searching.

Doubly linked list removes in O(1).

---

## Why One Matching Engine Per Symbol?

Keeps books isolated.

Allows future parallelization.

---

## Why REST + WebSocket?

REST

Order submission

Request/Response

WebSocket

Streaming

Low latency

Persistent connection

---

## Why Modular Design?

Each class has a single responsibility.

Modules can evolve independently.

Future additions become easier.

---

# Future Improvements

Potential production enhancements

* Thread Pool
* Lock-Free Queues
* Asynchronous Persistence
* Market Data Snapshots
* FIX Protocol
* Authentication
* Risk Checks
* User Accounts
* Matching Engine Sharding
* Connection Pooling
* Recovery from Persistent Storage
* Metrics Dashboard
* Docker Deployment
* CI/CD Pipeline

---

# Lessons Learned

Building this project required understanding several domains simultaneously:

* Modern C++
* Data Structures
* Algorithms
* Network Programming
* Asynchronous Programming
* HTTP
* WebSockets
* System Design
* Performance Optimization
* Software Architecture

The project reinforced the importance of clean modular design, efficient data structures, and separating matching logic from networking concerns.

---

# Current Status

Implemented

✅ Price-Time Priority Matching

✅ Order Book

✅ Limit Orders

✅ Market Orders

✅ Order Cancellation

✅ Multi-Symbol Exchange

✅ TCP Server

✅ TCP Client

✅ HTTP REST API

✅ WebSocket Market Data

✅ Live Trade Broadcasting

✅ Trade History (In-Memory)

✅ Benchmark Framework

Persistence to a relational database was explored but intentionally deferred to keep the project focused on the exchange engine itself and avoid platform-specific dependency complexity.

---

# Technologies

* C++17
* Boost.Asio
* Boost.Beast
* Modern CMake
* JSON (nlohmann/json)
* TCP/IP
* HTTP
* WebSocket
* Git

---

# License

This project was built for educational purposes to explore the architecture and implementation of electronic trading systems. It is not intended for use in real financial markets.
