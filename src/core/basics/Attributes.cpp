// -----------------------------------------------------------------------------
// Standard attributes (C++11 - C++23)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - [[nodiscard]] on a function and on a type, with a reason string (C++20)
//   - [[maybe_unused]] for parameters and variables that only exist in one build
//   - [[fallthrough]] to document an intentional switch fall-through
//   - [[deprecated("use X instead")]] and what the compiler prints
//   - [[likely]] / [[unlikely]] (C++20) and [[assume]] (C++23): hints, not guarantees
//
// Reference: https://en.cppreference.com/w/cpp/language/attributes
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "[[nodiscard]] on a function and on a type, with a reason string (C++20)",
    "[[maybe_unused]] for parameters and variables that only exist in one "
    "build",
    "[[fallthrough]] to document an intentional switch fall-through",
    "[[deprecated(\"use X instead\")]] and what the compiler prints",
    "[[likely]] / [[unlikely]] (C++20) and [[assume]] (C++23): hints, not "
    "guarantees",
};

void outline() {
  LOG_SECTION("Standard attributes (C++11 - C++23) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Attributes",
            "[[nodiscard]], [[maybe_unused]], [[fallthrough]], [[deprecated]], "
            "[[likely]], [[assume]]",
            lab::kDraft) {
  outline();
}
