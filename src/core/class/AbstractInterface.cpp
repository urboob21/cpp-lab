// -----------------------------------------------------------------------------
// Abstract classes and interfaces (C++98)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - = 0 makes a class abstract: it cannot be instantiated
//   - a pure virtual function may still have a definition, and why that is useful
//   - an interface: only pure virtual functions plus a virtual destructor
//   - the non-virtual interface idiom: public non-virtual, private virtual
//   - programming against an interface: swap the implementation in one line
//
// Reference: https://en.cppreference.com/w/cpp/language/abstract_class
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "= 0 makes a class abstract: it cannot be instantiated",
    "a pure virtual function may still have a definition, and why that is "
    "useful",
    "an interface: only pure virtual functions plus a virtual destructor",
    "the non-virtual interface idiom: public non-virtual, private virtual",
    "programming against an interface: swap the implementation in one line",
};

void outline() {
  LOG_SECTION("Abstract classes and interfaces (C++98) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE(
    "AbstractInterface",
    "pure virtual functions, abstract bases, the non-virtual interface idiom",
    lab::kDraft) {
  outline();
}
