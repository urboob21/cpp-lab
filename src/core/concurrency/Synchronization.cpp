// -----------------------------------------------------------------------------
// Latches, barriers and semaphores (C++20)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - std::latch: a one-shot countdown that threads wait on
//   - std::barrier: a reusable rendezvous with a completion function
//   - std::counting_semaphore and binary_semaphore for permits
//   - comparison with condition_variable: less code, fewer mistakes
//   - a worker pool that starts together and reports when all are done
//
// Reference: https://en.cppreference.com/w/cpp/thread/latch
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "std::latch: a one-shot countdown that threads wait on",
    "std::barrier: a reusable rendezvous with a completion function",
    "std::counting_semaphore and binary_semaphore for permits",
    "comparison with condition_variable: less code, fewer mistakes",
    "a worker pool that starts together and reports when all are done",
};

void outline() {
  LOG_SECTION("Latches, barriers and semaphores (C++20) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Synchronization",
            "std::latch, std::barrier and std::counting_semaphore as "
            "coordination primitives",
            lab::kDraft) {
  outline();
}
