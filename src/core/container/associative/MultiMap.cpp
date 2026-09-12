// -----------------------------------------------------------------------------
// std::multimap and std::multiset (C++98)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - insert never fails: a multimap keeps every duplicate
//   - equal_range to walk all values of one key
//   - erase(key) removes every match - erase(iterator) removes one
//   - a multiset as a sorted bag, and count() as a frequency table
//   - when a map<K, vector<V>> is the better model
//
// Reference: https://en.cppreference.com/w/cpp/container/multimap
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "insert never fails: a multimap keeps every duplicate",
    "equal_range to walk all values of one key",
    "erase(key) removes every match - erase(iterator) removes one",
    "a multiset as a sorted bag, and count() as a frequency table",
    "when a map<K, vector<V>> is the better model",
};

void outline() {
  LOG_SECTION("std::multimap and std::multiset (C++98) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("MultiMap",
            "duplicate keys: equal_range, count and erasing one of several "
            "equal entries",
            lab::kDraft) {
  outline();
}
