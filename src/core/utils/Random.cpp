// -----------------------------------------------------------------------------
// Random numbers (C++11)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - engine + distribution: std::mt19937 and std::uniform_int_distribution
//   - seeding from std::random_device, and seeding fixed for reproducible runs
//   - uniform_real, normal, bernoulli and discrete distributions
//   - why rand() % n is biased, and what modulo does to the distribution
//   - std::shuffle and std::sample instead of hand-rolled picking
//
// Reference: https://en.cppreference.com/w/cpp/numeric/random
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "engine + distribution: std::mt19937 and std::uniform_int_distribution",
    "seeding from std::random_device, and seeding fixed for reproducible runs",
    "uniform_real, normal, bernoulli and discrete distributions",
    "why rand() % n is biased, and what modulo does to the distribution",
    "std::shuffle and std::sample instead of hand-rolled picking",
};

void outline() {
  LOG_SECTION("Random numbers (C++11) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Random",
            "<random>: engines, seeding, distributions, and why std::rand is "
            "not enough",
            lab::kDraft) {
  outline();
}
