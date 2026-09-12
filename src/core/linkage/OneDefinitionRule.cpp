// -----------------------------------------------------------------------------
// The One Definition Rule (C++98/C++17)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - one definition per program for functions and variables, one per translation unit for types
//   - inline functions and inline variables (C++17): identical definitions allowed
//   - templates and class member functions defined in the header
//   - typical ODR violations: differing struct layouts behind an #ifdef
//   - what the linker reports, and what silently goes wrong
//
// Reference: https://en.cppreference.com/w/cpp/language/definition
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "one definition per program for functions and variables, one per "
    "translation unit for types",
    "inline functions and inline variables (C++17): identical definitions "
    "allowed",
    "templates and class member functions defined in the header",
    "typical ODR violations: differing struct layouts behind an #ifdef",
    "what the linker reports, and what silently goes wrong",
};

void outline() {
  LOG_SECTION("The One Definition Rule (C++98/C++17) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("OneDefinitionRule",
            "what may appear once, what may be repeated, and how inline and "
            "templates fit in",
            lab::kDraft) {
  outline();
}
