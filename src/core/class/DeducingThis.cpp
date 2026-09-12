// -----------------------------------------------------------------------------
// Deducing this (C++23)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - auto&& self replaces the four const/ref overloads of a getter
//   - how the deduced type propagates const and value category
//   - recursive lambdas without the Y-combinator trick
//   - CRTP-free static polymorphism with a deduced self type
//   - compiler support: this example needs GCC 14 / Clang 18 or newer
//
// Reference: https://en.cppreference.com/w/cpp/language/member_functions
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "auto&& self replaces the four const/ref overloads of a getter",
    "how the deduced type propagates const and value category",
    "recursive lambdas without the Y-combinator trick",
    "CRTP-free static polymorphism with a deduced self type",
    "compiler support: this example needs GCC 14 / Clang 18 or newer",
};

void outline() {
  LOG_SECTION("Deducing this (C++23) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("DeducingThis",
            "explicit object parameters: one member function instead of "
            "const/non-const pairs",
            lab::kDraft) {
  outline();
}
