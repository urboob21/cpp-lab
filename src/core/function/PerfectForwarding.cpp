// -----------------------------------------------------------------------------
// Perfect forwarding (C++11)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - T&& in a deduced context is a forwarding reference, not an rvalue reference
//   - reference collapsing: & + && = &
//   - std::forward<T> keeps the value category, std::move always moves
//   - a make_unique-style factory that forwards any argument list
//   - the classic mistakes: forwarding twice, std::forward without a template
//
// Reference: https://en.cppreference.com/w/cpp/utility/forward
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "T&& in a deduced context is a forwarding reference, not an rvalue "
    "reference",
    "reference collapsing: & + && = &",
    "std::forward<T> keeps the value category, std::move always moves",
    "a make_unique-style factory that forwards any argument list",
    "the classic mistakes: forwarding twice, std::forward without a template",
};

void outline() {
  LOG_SECTION("Perfect forwarding (C++11) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("PerfectForwarding",
            "forwarding references, std::forward, reference collapsing and "
            "forwarding factories",
            lab::kDraft) {
  outline();
}
