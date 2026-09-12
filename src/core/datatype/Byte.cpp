// -----------------------------------------------------------------------------
// std::byte (C++17)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - std::byte is not a number: no arithmetic, only bitwise operators
//   - std::to_integer<int>(b) and constructing from an integer
//   - reading and writing a buffer of std::byte
//   - why char has three flavours and which one to use for text
//   - std::as_bytes / std::as_writable_bytes over a std::span (C++20)
//
// Reference: https://en.cppreference.com/w/cpp/types/byte
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "std::byte is not a number: no arithmetic, only bitwise operators",
    "std::to_integer<int>(b) and constructing from an integer",
    "reading and writing a buffer of std::byte",
    "why char has three flavours and which one to use for text",
    "std::as_bytes / std::as_writable_bytes over a std::span (C++20)",
};

void outline() {
  LOG_SECTION("std::byte (C++17) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Byte", "std::byte as raw memory, versus char and unsigned char",
            lab::kDraft) {
  outline();
}
