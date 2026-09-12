// -----------------------------------------------------------------------------
// Measuring time with <chrono>
//
//   clocks     : steady_clock (never goes backwards - use it to measure),
//                system_clock (wall-clock time, can jump when adjusted)
//   durations  : std::chrono::milliseconds, seconds, ... (type-safe units)
//   literals   : using namespace std::chrono_literals;  -> 100ms, 2s
//
// Measure elapsed time as `steady_clock::now() - start` and convert the
// result with duration_cast or a floating-point duration.
//
// Reference: https://en.cppreference.com/w/cpp/chrono
// -----------------------------------------------------------------------------

#include <chrono>
#include <cstdint>
#include <thread>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

using namespace std::chrono_literals;

void durations() {
  LOG_SECTION("Durations and literals");
  const std::chrono::milliseconds timeout = 1500ms;
  const auto in_seconds =
      std::chrono::duration_cast<std::chrono::seconds>(timeout);
  const std::chrono::duration<double> precise =
      timeout;  // implicit: no precision lost

  LOG_S("1500ms = " << timeout.count() << " ms");
  LOG_S("duration_cast<seconds>(1500ms) = " << in_seconds.count()
                                            << " s (truncated)");
  LOG_S("duration<double>(1500ms)       = " << precise.count() << " s");
  LOG_S("2s + 250ms = " << (2s +
                            250ms));  // mixed units: result in the finer unit
}

void measureSleep() {
  LOG_SECTION("Measuring a sleep with steady_clock");
  const auto start = std::chrono::steady_clock::now();
  std::this_thread::sleep_for(100ms);
  const auto elapsed = std::chrono::steady_clock::now() - start;

  const std::chrono::duration<double, std::milli> elapsed_ms = elapsed;
  LOG_S("sleep_for(100ms) took " << elapsed_ms.count()
                                 << " ms (at least 100 ms, never less)");
}

void measureWork() {
  LOG_SECTION("Measuring a computation");
  const auto start = std::chrono::steady_clock::now();

  std::uint64_t sum = 0;
  for (std::uint64_t i = 0; i < 10'000'000; ++i) {
    sum += i;
  }

  const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::steady_clock::now() - start);
  LOG_S("sum = " << sum << " computed in " << elapsed);  // C++20 operator<<
}

}  // namespace

LAB_EXAMPLE(
    "Timing",
    "std::chrono clocks, durations, literals and measuring elapsed time") {
  durations();
  measureSleep();
  measureWork();
}
