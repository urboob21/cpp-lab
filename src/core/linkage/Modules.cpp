// -----------------------------------------------------------------------------
// Modules (C++20)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - export module math; and import math; instead of a header
//   - what a module interface unit exports, and what stays private
//   - module partitions and implementation units
//   - no macros leak, no include order, faster builds in principle
//   - tooling status: modules need extra CMake and compiler support, so this lab still uses headers
//
// Reference: https://en.cppreference.com/w/cpp/language/modules
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "export module math; and import math; instead of a header",
    "what a module interface unit exports, and what stays private",
    "module partitions and implementation units",
    "no macros leak, no include order, faster builds in principle",
    "tooling status: modules need extra CMake and compiler support, so this "
    "lab still uses headers",
};

void outline() {
  LOG_SECTION("Modules (C++20) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE(
    "Modules",
    "export module, import, partitions, and what changes compared with headers",
    lab::kDraft) {
  outline();
}
