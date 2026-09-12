// -----------------------------------------------------------------------------
// Variable templates (C++14/C++17)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - template<typename T> constexpr T pi = T(3.14159265358979);
//   - the _v trait shorthands: std::is_integral_v<T> is a variable template
//   - specializing a variable template
//   - inline variables (C++17): one definition shared by every translation unit
//   - how this differs from a constexpr function
//
// Reference: https://en.cppreference.com/w/cpp/language/variable_template
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "template<typename T> constexpr T pi = T(3.14159265358979);",
    "the _v trait shorthands: std::is_integral_v<T> is a variable template",
    "specializing a variable template",
    "inline variables (C++17): one definition shared by every translation unit",
    "how this differs from a constexpr function",
};

void outline() {
  LOG_SECTION("Variable templates (C++14/C++17) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE(
    "VariableTemplate",
    "variable templates, constexpr and inline variables at namespace scope",
    lab::kDraft) {
  outline();
}
