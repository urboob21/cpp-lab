// -----------------------------------------------------------------------------
// Bit manipulation (C++20)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - std::popcount, std::countl_zero, std::countr_zero, std::bit_width
//   - std::has_single_bit, std::bit_ceil, std::bit_floor for power-of-two sizes
//   - std::rotl / std::rotr instead of hand-written shift pairs
//   - std::endian::native and byte swapping (std::byteswap, C++23)
//   - the classic hand-written equivalents and how the compiler optimises them
//
// Reference: https://en.cppreference.com/w/cpp/header/bit
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "std::popcount, std::countl_zero, std::countr_zero, std::bit_width",
    "std::has_single_bit, std::bit_ceil, std::bit_floor for power-of-two sizes",
    "std::rotl / std::rotr instead of hand-written shift pairs",
    "std::endian::native and byte swapping (std::byteswap, C++23)",
    "the classic hand-written equivalents and how the compiler optimises them",
};

void outline() {
  LOG_SECTION("Bit manipulation (C++20) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("BitOperations",
            "the <bit> header: popcount, countl_zero, bit_width, rotl, endian "
            "and bit_cast",
            lab::kDraft) {
  outline();
}
