// -----------------------------------------------------------------------------
// std::span (C++20)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - one function signature for arrays, vectors and std::array
//   - static vs dynamic extent, and what that costs
//   - first, last, subspan without copying
//   - span is a view: the referenced storage must outlive it
//   - as_bytes for a raw view, and span vs string_view
//
// Reference: https://en.cppreference.com/w/cpp/container/span
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "one function signature for arrays, vectors and std::array",
    "static vs dynamic extent, and what that costs",
    "first, last, subspan without copying",
    "span is a view: the referenced storage must outlive it",
    "as_bytes for a raw view, and span vs string_view",
};

void outline() {
  LOG_SECTION("std::span (C++20) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE(
    "Span",
    "a non-owning view over contiguous memory: pointer + size in one type",
    lab::kDraft) {
  outline();
}
