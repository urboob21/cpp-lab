// -----------------------------------------------------------------------------
// Locks beyond std::mutex (C++14/C++17)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - shared_lock for many readers, unique_lock for one writer
//   - std::scoped_lock locks several mutexes without deadlock (C++17)
//   - unique_lock: defer_lock, try_to_lock, adopt_lock and unlocking early
//   - std::once_flag and std::call_once for one-time initialisation
//   - recursive_mutex and timed mutexes: what they are for, and the cost
//
// Reference: https://en.cppreference.com/w/cpp/thread/shared_mutex
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "shared_lock for many readers, unique_lock for one writer",
    "std::scoped_lock locks several mutexes without deadlock (C++17)",
    "unique_lock: defer_lock, try_to_lock, adopt_lock and unlocking early",
    "std::once_flag and std::call_once for one-time initialisation",
    "recursive_mutex and timed mutexes: what they are for, and the cost",
};

void outline() {
  LOG_SECTION("Locks beyond std::mutex (C++14/C++17) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("SharedMutex",
            "shared_mutex, scoped_lock, unique_lock options, recursive locks "
            "and std::call_once",
            lab::kDraft) {
  outline();
}
