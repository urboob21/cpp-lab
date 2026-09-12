// -----------------------------------------------------------------------------
// Ranges and views (C++20/C++23)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - std::ranges::sort(v) instead of sort(v.begin(), v.end())
//   - projections: sort people by .age without a comparator
//   - views are lazy and cheap: filter, transform, take, drop, reverse
//   - composing a pipeline with operator| and where the work actually happens
//   - materialising with std::ranges::to<std::vector> (C++23); zip, chunk, adjacent
//
// Reference: https://en.cppreference.com/w/cpp/ranges
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "std::ranges::sort(v) instead of sort(v.begin(), v.end())",
    "projections: sort people by .age without a comparator",
    "views are lazy and cheap: filter, transform, take, drop, reverse",
    "composing a pipeline with operator| and where the work actually happens",
    "materialising with std::ranges::to<std::vector> (C++23); zip, chunk, "
    "adjacent",
};

void outline() {
  LOG_SECTION("Ranges and views (C++20/C++23) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE(
    "Ranges",
    "range algorithms, lazy views, pipelines, projections and ranges::to",
    lab::kDraft) {
  outline();
}
