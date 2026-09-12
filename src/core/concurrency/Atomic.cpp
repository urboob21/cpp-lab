// -----------------------------------------------------------------------------
// std::atomic (C++11/C++20)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - atomic<int> as a shared counter: no mutex, no data race
//   - load, store, fetch_add and the read-modify-write operations
//   - compare_exchange_weak in a lock-free update loop
//   - memory orders: relaxed, acquire/release, seq_cst - what each guarantees
//   - is_lock_free, atomic_flag and std::atomic_ref (C++20)
//
// Reference: https://en.cppreference.com/w/cpp/atomic/atomic
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "atomic<int> as a shared counter: no mutex, no data race",
    "load, store, fetch_add and the read-modify-write operations",
    "compare_exchange_weak in a lock-free update loop",
    "memory orders: relaxed, acquire/release, seq_cst - what each guarantees",
    "is_lock_free, atomic_flag and std::atomic_ref (C++20)",
};

void outline() {
  LOG_SECTION("std::atomic (C++11/C++20) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Atomic",
            "atomic counters and flags, compare_exchange, memory orders and "
            "std::atomic_ref",
            lab::kDraft) {
  outline();
}
