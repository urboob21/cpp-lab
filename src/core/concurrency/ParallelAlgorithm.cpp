// -----------------------------------------------------------------------------
// Parallel algorithms (C++17)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - std::sort(std::execution::par, ...) - one argument, many threads
//   - seq, par, par_unseq and unseq (C++20): what each allows
//   - the element operations must be safe to run concurrently
//   - measuring: parallelism only pays above a certain problem size
//   - libstdc++ needs TBB (-ltbb); check before relying on it
//
// Reference: https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "std::sort(std::execution::par, ...) - one argument, many threads",
    "seq, par, par_unseq and unseq (C++20): what each allows",
    "the element operations must be safe to run concurrently",
    "measuring: parallelism only pays above a certain problem size",
    "libstdc++ needs TBB (-ltbb); check before relying on it",
};

void outline() {
  LOG_SECTION("Parallel algorithms (C++17) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("ParallelAlgorithm",
            "execution policies: par, par_unseq, when parallelism pays off and "
            "what it costs",
            lab::kDraft) {
  outline();
}
