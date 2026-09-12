// -----------------------------------------------------------------------------
// std::any (C++17)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - std::any holds one value of any copyable type, with its type_info
//   - any_cast<T>: the reference form throws, the pointer form returns nullptr
//   - has_value, reset, emplace and small-object optimisation
//   - any vs variant: open set of types vs a closed, checked one
//   - a realistic use: a heterogeneous settings map
//
// Reference: https://en.cppreference.com/w/cpp/utility/any
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "std::any holds one value of any copyable type, with its type_info",
    "any_cast<T>: the reference form throws, the pointer form returns nullptr",
    "has_value, reset, emplace and small-object optimisation",
    "any vs variant: open set of types vs a closed, checked one",
    "a realistic use: a heterogeneous settings map",
};

void outline() {
  LOG_SECTION("std::any (C++17) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Any",
            "type-erased storage for a single value of any type, and when "
            "variant or optional fit better",
            lab::kDraft) {
  outline();
}
