// -----------------------------------------------------------------------------
// std::map (C++98/C++17)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - operator[] default-constructs a missing value - at() throws instead
//   - insert vs insert_or_assign vs emplace vs try_emplace (C++17)
//   - structured bindings over the pairs, and extract/merge (C++17)
//   - lower_bound / upper_bound / equal_range on sorted keys
//   - a custom comparator, and heterogeneous lookup with is_transparent
//
// Reference: https://en.cppreference.com/w/cpp/container/map
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "operator[] default-constructs a missing value - at() throws instead",
    "insert vs insert_or_assign vs emplace vs try_emplace (C++17)",
    "structured bindings over the pairs, and extract/merge (C++17)",
    "lower_bound / upper_bound / equal_range on sorted keys",
    "a custom comparator, and heterogeneous lookup with is_transparent",
};

void outline() {
  LOG_SECTION("std::map (C++98/C++17) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Map",
            "std::map: sorted keys, operator[] vs at vs insert vs try_emplace, "
            "lower_bound, custom comparator",
            lab::kDraft) {
  outline();
}
