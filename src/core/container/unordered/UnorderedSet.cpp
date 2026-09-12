// -----------------------------------------------------------------------------
// std::unordered_set (C++11)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - insert returns a pair: was the element new?
//   - specializing std::hash for a user type, or passing a hash functor
//   - the equality functor must agree with the hash
//   - bucket_count, load_factor, max_load_factor and rehash/reserve
//   - unordered_set vs set: order, complexity and iteration cost
//
// Reference: https://en.cppreference.com/w/cpp/container/unordered_set
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "insert returns a pair: was the element new?",
    "specializing std::hash for a user type, or passing a hash functor",
    "the equality functor must agree with the hash",
    "bucket_count, load_factor, max_load_factor and rehash/reserve",
    "unordered_set vs set: order, complexity and iteration cost",
};

void outline() {
  LOG_SECTION("std::unordered_set (C++11) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("UnorderedSet",
            "hash sets: custom hash and equality, load factor, and hashing a "
            "user-defined type",
            lab::kDraft) {
  outline();
}
