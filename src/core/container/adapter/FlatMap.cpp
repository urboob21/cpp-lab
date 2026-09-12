// -----------------------------------------------------------------------------
// std::flat_map and std::flat_set (C++23)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - a flat_map is two vectors: keys and values, kept sorted
//   - lookup is a binary search, iteration is cache friendly
//   - insertion and erasure move elements: O(n), unlike std::map
//   - constructing from sorted input with std::sorted_unique
//   - check compiler support before using it; the sorted-vector idiom by hand
//
// Reference: https://en.cppreference.com/w/cpp/container/flat_map
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "a flat_map is two vectors: keys and values, kept sorted",
    "lookup is a binary search, iteration is cache friendly",
    "insertion and erasure move elements: O(n), unlike std::map",
    "constructing from sorted input with std::sorted_unique",
    "check compiler support before using it; the sorted-vector idiom by hand",
};

void outline() {
  LOG_SECTION("std::flat_map and std::flat_set (C++23) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("FlatMap",
            "sorted vectors behind a map interface: fast lookup and iteration, "
            "slow insertion",
            lab::kDraft) {
  outline();
}
