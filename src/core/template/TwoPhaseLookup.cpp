// -----------------------------------------------------------------------------
// Two-phase lookup and dependent names (C++98)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - what the compiler checks at definition time and what waits for instantiation
//   - typename T::value_type: why the keyword is needed (optional since C++20 in some places)
//   - obj.template get<0>() and Base<T>::template rebind<U>
//   - why members of a dependent base need this-> or a using declaration
//   - declaration in a header, definition in a .cpp plus explicit instantiation
//
// Reference: https://en.cppreference.com/w/cpp/language/dependent_name
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "what the compiler checks at definition time and what waits for "
    "instantiation",
    "typename T::value_type: why the keyword is needed (optional since C++20 "
    "in some places)",
    "obj.template get<0>() and Base<T>::template rebind<U>",
    "why members of a dependent base need this-> or a using declaration",
    "declaration in a header, definition in a .cpp plus explicit instantiation",
};

void outline() {
  LOG_SECTION("Two-phase lookup and dependent names (C++98) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("TwoPhaseLookup",
            "typename and template disambiguators, dependent names, explicit "
            "instantiation",
            lab::kDraft) {
  outline();
}
