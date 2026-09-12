// -----------------------------------------------------------------------------
// SFINAE and if constexpr (C++11/C++17)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - substitution failure is not an error: the compiler drops the overload
//   - std::enable_if_t in the return type, in a parameter and as a template argument
//   - the void_t detection idiom: does T have a .size() member?
//   - if constexpr (C++17) instead of two overloads
//   - the same check written as a C++20 concept, side by side
//
// Reference: https://en.cppreference.com/w/cpp/language/sfinae
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "substitution failure is not an error: the compiler drops the overload",
    "std::enable_if_t in the return type, in a parameter and as a template "
    "argument",
    "the void_t detection idiom: does T have a .size() member?",
    "if constexpr (C++17) instead of two overloads",
    "the same check written as a C++20 concept, side by side",
};

void outline() {
  LOG_SECTION("SFINAE and if constexpr (C++11/C++17) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Sfinae",
            "SFINAE with enable_if and void_t, and how if constexpr and "
            "concepts replace it",
            lab::kDraft) {
  outline();
}
