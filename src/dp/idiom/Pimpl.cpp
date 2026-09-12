// -----------------------------------------------------------------------------
// The pImpl idiom (C++ idiom)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - class Widget { struct Impl; std::unique_ptr<Impl> impl_; };
//   - the destructor must be defined where Impl is complete
//   - what it buys: no private members in the header, fewer rebuilds, stable ABI
//   - what it costs: an allocation and an indirection per object
//   - copy semantics: they have to be written by hand
//
// Reference: https://en.cppreference.com/w/cpp/language/pimpl
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "class Widget { struct Impl; std::unique_ptr<Impl> impl_; };",
    "the destructor must be defined where Impl is complete",
    "what it buys: no private members in the header, fewer rebuilds, stable "
    "ABI",
    "what it costs: an allocation and an indirection per object",
    "copy semantics: they have to be written by hand",
};

void outline() {
  LOG_SECTION("The pImpl idiom (C++ idiom) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE(
    "Pimpl",
    "hide the implementation behind a pointer: stable ABI and faster rebuilds",
    lab::kDraft) {
  outline();
}
