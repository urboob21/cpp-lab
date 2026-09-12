// -----------------------------------------------------------------------------
// std::mdspan (C++23)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - a flat vector viewed as a 2D matrix: m[row, col] (C++23 subscript)
//   - static and dynamic extents
//   - layout_right vs layout_left: row major and column major over the same data
//   - submdspan and slicing
//   - compiler support, and the hand-written index arithmetic it replaces
//
// Reference: https://en.cppreference.com/w/cpp/container/mdspan
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "a flat vector viewed as a 2D matrix: m[row, col] (C++23 subscript)",
    "static and dynamic extents",
    "layout_right vs layout_left: row major and column major over the same "
    "data",
    "submdspan and slicing",
    "compiler support, and the hand-written index arithmetic it replaces",
};

void outline() {
  LOG_SECTION("std::mdspan (C++23) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE(
    "Mdspan",
    "multidimensional views over flat storage: extents, layouts and accessors",
    lab::kDraft) {
  outline();
}
