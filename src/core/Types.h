#pragma once

#include <cstdint>
#include <limits>
/*
 * Common type aliases used throughout the exchange engine.
 * Keeping them here improves readability and makes future changes easier.
 */

using OrderId  = std::uint64_t;
using Price    = int;
using Quantity = int;
using Volume   = int;
// constexpr Price INVALID_PRICE = std::numeric_limits<Price>::max();