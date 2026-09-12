// -----------------------------------------------------------------------------
// Static members (C++98/C++17)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - a static data member is shared by every object, and needs one definition
//   - inline static (C++17) removes the out-of-class definition
//   - static constexpr members as compile-time constants
//   - static member functions: no this, callable without an object
//   - a counter of live instances, incremented in the constructors
//
// Reference: https://en.cppreference.com/w/cpp/language/static
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "a static data member is shared by every object, and needs one definition",
    "inline static (C++17) removes the out-of-class definition",
    "static constexpr members as compile-time constants",
    "static member functions: no this, callable without an object",
    "a counter of live instances, incremented in the constructors",
};

void outline() {
  LOG_SECTION("Static members (C++98/C++17) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("StaticMember",
            "static data members and functions, inline static, constexpr "
            "static and instance counters",
            lab::kDraft) {
  outline();
}
