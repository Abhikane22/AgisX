#pragma once

#include <cstdint>

/*
 * Common type aliases used throughout the exchange engine.
 * Keeping them here improves readability and makes future changes easier.
 */

using OrderId  = std::uint64_t;
using Price    = int;
using Quantity = int;
using Volume   = int;