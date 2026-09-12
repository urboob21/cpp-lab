// -----------------------------------------------------------------------------
// Literals (C++11/C++14)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - R"(raw \n string)" and the R"delim( ... )delim" form
//   - standard suffixes: 42s, 1.5f, 10ms, "text"s, "view"sv
//   - writing operator""_km and operator""_deg for a unit type
//   - cooked vs raw literal operators, and the template form for digits
//   - why user-defined suffixes must start with an underscore
//
// Reference: https://en.cppreference.com/w/cpp/language/user_literal
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "R\"(raw \\n string)\" and the R\"delim( ... )delim\" form",
    R"(standard suffixes: 42s, 1.5f, 10ms, "text"s, "view"sv)",
    R"(writing operator""_km and operator""_deg for a unit type)",
    "cooked vs raw literal operators, and the template form for digits",
    "why user-defined suffixes must start with an underscore",
};

void outline() {
  LOG_SECTION("Literals (C++11/C++14) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE(
    "UserDefinedLiteral",
    "raw string literals, standard literal suffixes and user-defined literals",
    lab::kDraft) {
  outline();
}
