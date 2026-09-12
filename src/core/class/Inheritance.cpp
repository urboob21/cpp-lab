// -----------------------------------------------------------------------------
// Inheritance (C++98/C++11)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - what public, protected and private inheritance mean for the interface
//   - override and final: let the compiler catch a wrong signature
//   - inheriting constructors with using Base::Base (C++11)
//   - name hiding: a derived overload hides every base overload
//   - when to prefer composition over inheritance
//
// Reference: https://en.cppreference.com/w/cpp/language/derived_class
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "what public, protected and private inheritance mean for the interface",
    "override and final: let the compiler catch a wrong signature",
    "inheriting constructors with using Base::Base (C++11)",
    "name hiding: a derived overload hides every base overload",
    "when to prefer composition over inheritance",
};

void outline() {
  LOG_SECTION("Inheritance (C++98/C++11) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Inheritance",
            "public, protected and private inheritance, override, final and "
            "inheriting constructors",
            lab::kDraft) {
  outline();
}
