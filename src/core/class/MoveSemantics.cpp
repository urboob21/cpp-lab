// -----------------------------------------------------------------------------
// Move semantics (C++11)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - a class with an owning pointer: copy vs move, counted with log lines
//   - std::move is a cast: it only marks an object as movable
//   - what a moved-from object may be used for (valid but unspecified)
//   - noexcept moves and why std::vector needs them to move on reallocation
//   - guaranteed copy elision (C++17): the move that never happens
//
// Reference: https://en.cppreference.com/w/cpp/language/move_constructor
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "a class with an owning pointer: copy vs move, counted with log lines",
    "std::move is a cast: it only marks an object as movable",
    "what a moved-from object may be used for (valid but unspecified)",
    "noexcept moves and why std::vector needs them to move on reallocation",
    "guaranteed copy elision (C++17): the move that never happens",
};

void outline() {
  LOG_SECTION("Move semantics (C++11) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("MoveSemantics",
            "move constructor and assignment, when moves happen, moved-from "
            "state, copy elision",
            lab::kDraft) {
  outline();
}
