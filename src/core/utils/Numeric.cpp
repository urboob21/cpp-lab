// -----------------------------------------------------------------------------
// The numeric algorithms (C++11/C++17)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - std::iota to fill a range with increasing values
//   - accumulate is sequential and ordered; reduce may reorder (C++17)
//   - transform_reduce: the map-reduce pair in one call
//   - partial_sum, adjacent_difference and inclusive/exclusive_scan
//   - std::gcd, std::lcm and std::midpoint (why (a+b)/2 can overflow)
//
// Reference: https://en.cppreference.com/w/cpp/header/numeric
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "std::iota to fill a range with increasing values",
    "accumulate is sequential and ordered; reduce may reorder (C++17)",
    "transform_reduce: the map-reduce pair in one call",
    "partial_sum, adjacent_difference and inclusive/exclusive_scan",
    "std::gcd, std::lcm and std::midpoint (why (a+b)/2 can overflow)",
};

void outline() {
  LOG_SECTION("The numeric algorithms (C++11/C++17) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Numeric",
            "<numeric>: iota, accumulate vs reduce, inner_product, "
            "partial_sum, gcd, lcm, midpoint",
            lab::kDraft) {
  outline();
}
