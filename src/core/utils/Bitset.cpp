// -----------------------------------------------------------------------------
// std::bitset (C++98)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - set, reset, flip, test and operator[]
//   - count, any, all, none
//   - conversion: to_string, to_ulong, to_ullong and constructing from both
//   - bitwise operators between bitsets, and shifting
//   - bitset vs vector<bool> vs an enum of bit flags (see core/datatype/Enum)
//
// Reference: https://en.cppreference.com/w/cpp/utility/bitset
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "set, reset, flip, test and operator[]",
    "count, any, all, none",
    "conversion: to_string, to_ulong, to_ullong and constructing from both",
    "bitwise operators between bitsets, and shifting",
    "bitset vs vector<bool> vs an enum of bit flags (see core/datatype/Enum)",
};

void outline() {
  LOG_SECTION("std::bitset (C++98) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Bitset",
            "a fixed-size sequence of bits: flags, counting, and conversion to "
            "strings and integers",
            lab::kDraft) {
  outline();
}
