// -----------------------------------------------------------------------------
// Type erasure (C++ idiom)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - std::function is type erasure: any callable with the right signature
//   - the three parts: a concept struct, a templated model, a handle that owns it
//   - a Drawable that accepts any type with a draw() member
//   - cost: one allocation and one virtual call, like an interface
//   - alternatives: std::variant (closed set) and templates (compile time)
//
// Reference: https://en.cppreference.com/w/cpp/utility/functional/function
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "std::function is type erasure: any callable with the right signature",
    "the three parts: a concept struct, a templated model, a handle that owns "
    "it",
    "a Drawable that accepts any type with a draw() member",
    "cost: one allocation and one virtual call, like an interface",
    "alternatives: std::variant (closed set) and templates (compile time)",
};

void outline() {
  LOG_SECTION("Type erasure (C++ idiom) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE(
    "TypeErasure",
    "store any type that satisfies an interface, without a common base class",
    lab::kDraft) {
  outline();
}
