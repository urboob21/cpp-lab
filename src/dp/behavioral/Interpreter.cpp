// -----------------------------------------------------------------------------
// Interpreter (GoF)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - an expression tree: numbers, variables and binary operators
//   - one class per grammar rule, each with interpret(context)
//   - the context: variable bindings passed down the tree
//   - parsing a string into the tree (a tiny recursive descent parser)
//   - when a real parser generator or std::variant visitor is the better answer
//
// Reference: https://refactoring.guru/design-patterns
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "an expression tree: numbers, variables and binary operators",
    "one class per grammar rule, each with interpret(context)",
    "the context: variable bindings passed down the tree",
    "parsing a string into the tree (a tiny recursive descent parser)",
    "when a real parser generator or std::variant visitor is the better answer",
};

void outline() {
  LOG_SECTION("Interpreter (GoF) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE(
    "Interpreter",
    "represent a small grammar as a class per rule and evaluate expressions",
    lab::kDraft) {
  outline();
}
