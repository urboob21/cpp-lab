// -----------------------------------------------------------------------------
// std::list (C++98)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - insert and erase in the middle in O(1), but no random access
//   - iterators and references stay valid across insertions and erasures
//   - splice: move elements between lists without copying
//   - member sort, merge, unique and remove_if instead of the algorithms
//   - why std::vector still wins in most benchmarks (cache locality)
//
// Reference: https://en.cppreference.com/w/cpp/container/list
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "insert and erase in the middle in O(1), but no random access",
    "iterators and references stay valid across insertions and erasures",
    "splice: move elements between lists without copying",
    "member sort, merge, unique and remove_if instead of the algorithms",
    "why std::vector still wins in most benchmarks (cache locality)",
};

void outline() {
  LOG_SECTION("std::list (C++98) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("List",
            "std::list: doubly linked storage, splice, stable iterators and "
            "when not to use it",
            lab::kDraft) {
  outline();
}
