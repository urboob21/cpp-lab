// -----------------------------------------------------------------------------
// Concepts (C++20)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - concept Addable = requires(T a, T b) { a + b; };
//   - the four ways to constrain: requires clause, trailing requires, auto, shorthand
//   - standard concepts: std::integral, std::ranges::range, std::invocable
//   - subsumption: the most constrained overload wins
//   - compare the error message of a failed concept with a raw template error
//
// Reference: https://en.cppreference.com/w/cpp/language/constraints
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "concept Addable = requires(T a, T b) { a + b; };",
    "the four ways to constrain: requires clause, trailing requires, auto, "
    "shorthand",
    "standard concepts: std::integral, std::ranges::range, std::invocable",
    "subsumption: the most constrained overload wins",
    "compare the error message of a failed concept with a raw template error",
};

void outline() {
  LOG_SECTION("Concepts (C++20) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Concepts",
            "concepts: requires clauses, requires expressions, standard "
            "concepts and better errors",
            lab::kDraft) {
  outline();
}
