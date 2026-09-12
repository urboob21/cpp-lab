// -----------------------------------------------------------------------------
// Small utilities of <utility> (C++11 - C++23)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - std::swap, and why a member swap can be faster
//   - std::exchange: assign and return the old value in one step
//   - std::as_const to force the const overload
//   - std::cmp_less and friends (C++20): comparing signed with unsigned safely
//   - std::to_underlying (C++23) instead of static_cast on a scoped enum
//
// Reference: https://en.cppreference.com/w/cpp/header/utility
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "std::swap, and why a member swap can be faster",
    "std::exchange: assign and return the old value in one step",
    "std::as_const to force the const overload",
    "std::cmp_less and friends (C++20): comparing signed with unsigned safely",
    "std::to_underlying (C++23) instead of static_cast on a scoped enum",
};

void outline() {
  LOG_SECTION("Small utilities of <utility> (C++11 - C++23) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Utility",
            "std::swap, std::exchange, std::as_const, integer comparisons and "
            "std::to_underlying",
            lab::kDraft) {
  outline();
}
