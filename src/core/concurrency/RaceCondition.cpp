// -----------------------------------------------------------------------------
// Race conditions, std::mutex and std::atomic
//
// `++counter` is not one step: the CPU reads the value, adds one and writes it
// back. When two threads do this at the same time, increments get lost.
// Unsynchronized access to shared data is a DATA RACE, which is undefined
// behavior - the wrong total below is just one possible symptom.
//
// Fixes:
//   - std::mutex + std::lock_guard : only one thread at a time in the section
//   - std::atomic<int>             : the increment itself is indivisible
//
// Tip: build with -fsanitize=thread (ThreadSanitizer) to detect data races.
// Reference: https://en.cppreference.com/w/cpp/thread/mutex
// -----------------------------------------------------------------------------

#include <atomic>
#include <mutex>
#include <string_view>
#include <thread>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

constexpr int kIncrementsPerThread = 1'000'000;
constexpr int kExpected = 2 * kIncrementsPerThread;

/// Runs `work` on two threads and reports the final counter.
template <typename Work, typename Counter>
void runCase(std::string_view title, Work work, const Counter& counter) {
  LOG_SECTION(title);
  std::thread first(work);
  std::thread second(work);
  first.join();
  second.join();

  const int total = counter;
  LOG_S("expected " << kExpected << ", got " << total
                    << (total == kExpected ? "  (correct)"
                                           : "  <- lost updates!"));
}

namespace problem {

int counter = 0;

void run() {
  auto work = [] {
    for (int i = 0; i < kIncrementsPerThread; ++i) {
      ++counter;  // DATA RACE: read-modify-write without synchronization
    }
  };
  runCase("Problem: unsynchronized ++counter", work, counter);
}

}  // namespace problem

namespace with_mutex {

int counter = 0;
std::mutex counter_mutex;

void run() {
  auto work = [] {
    for (int i = 0; i < kIncrementsPerThread; ++i) {
      // Anti-pattern: counter_mutex.lock(); ++counter; counter_mutex.unlock();
      // If anything between lock and unlock throws, the mutex stays locked.
      const std::lock_guard<std::mutex> lock(
          counter_mutex);  // RAII: always unlocks
      ++counter;
    }
  };
  runCase("Fix 1: std::mutex + std::lock_guard", work, counter);
}

}  // namespace with_mutex

namespace with_atomic {

std::atomic<int> counter{0};

void run() {
  auto work = [] {
    for (int i = 0; i < kIncrementsPerThread; ++i) {
      ++counter;  // atomic increment, no lock needed
    }
  };
  runCase("Fix 2: std::atomic<int>", work, counter);
  LOG("atomic is faster than a mutex for a single variable; use a mutex when");
  LOG("several variables must change together.");
}

}  // namespace with_atomic

}  // namespace

LAB_EXAMPLE(
    "RaceCondition",
    "lost updates from a data race, fixed with std::mutex or std::atomic") {
  problem::counter = 0;
  with_mutex::counter = 0;
  with_atomic::counter = 0;

  problem::run();
  with_mutex::run();
  with_atomic::run();
}
