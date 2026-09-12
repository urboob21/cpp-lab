// -----------------------------------------------------------------------------
// Coroutines (C++20/C++23)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - what makes a function a coroutine, and where its frame lives
//   - a lazy generator with co_yield, consumed by a range-for
//   - the promise_type: initial_suspend, final_suspend, yield_value, return_value
//   - co_await and a trivial awaitable
//   - std::generator (C++23) instead of hand-writing the plumbing
//
// Reference: https://en.cppreference.com/w/cpp/language/coroutines
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "what makes a function a coroutine, and where its frame lives",
    "a lazy generator with co_yield, consumed by a range-for",
    "the promise_type: initial_suspend, final_suspend, yield_value, "
    "return_value",
    "co_await and a trivial awaitable",
    "std::generator (C++23) instead of hand-writing the plumbing",
};

void outline() {
  LOG_SECTION("Coroutines (C++20/C++23) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE(
    "Coroutine",
    "co_await, co_yield, co_return, the promise type, and std::generator",
    lab::kDraft) {
  outline();
}
