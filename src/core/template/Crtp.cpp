// -----------------------------------------------------------------------------
// The CRTP idiom (C++98)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - class Derived : public Base<Derived> - what that buys
//   - static dispatch: static_cast<Derived*>(this)->impl()
//   - mixins: adding comparison or printing to any class
//   - cost comparison with a virtual interface (no vtable, but no runtime dispatch)
//   - C++23 deducing this as the modern alternative (see core/class/DeducingThis)
//
// Reference: https://en.cppreference.com/w/cpp/language/crtp
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "class Derived : public Base<Derived> - what that buys",
    "static dispatch: static_cast<Derived*>(this)->impl()",
    "mixins: adding comparison or printing to any class",
    "cost comparison with a virtual interface (no vtable, but no runtime "
    "dispatch)",
    "C++23 deducing this as the modern alternative (see "
    "core/class/DeducingThis)",
};

void outline() {
  LOG_SECTION("The CRTP idiom (C++98) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Crtp",
            "curiously recurring template pattern: static polymorphism and "
            "mixins without virtual calls",
            lab::kDraft) {
  outline();
}
