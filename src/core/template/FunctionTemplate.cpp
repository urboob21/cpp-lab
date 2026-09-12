// -----------------------------------------------------------------------------
// Function templates (C++98/C++20)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - template<typename T> and the difference between deduction and explicit T
//   - how deduction strips references and const, and when it fails
//   - overloading a template with a non-template function: which one wins
//   - abbreviated function templates: void print(auto value) (C++20)
//   - constraints in one line: void print(std::integral auto value) (C++20)
//
// Reference: https://en.cppreference.com/w/cpp/language/function_template
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "template<typename T> and the difference between deduction and explicit T",
    "how deduction strips references and const, and when it fails",
    "overloading a template with a non-template function: which one wins",
    "abbreviated function templates: void print(auto value) (C++20)",
    "constraints in one line: void print(std::integral auto value) (C++20)",
};

void outline() {
  LOG_SECTION("Function templates (C++98/C++20) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("FunctionTemplate",
            "function templates: argument deduction, explicit arguments, "
            "overloading, auto parameters",
            lab::kDraft) {
  outline();
}
