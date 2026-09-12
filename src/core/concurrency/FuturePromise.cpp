// -----------------------------------------------------------------------------
// std::future, std::async, std::promise, std::packaged_task
//
// A future is a handle to a value (or exception) that will be available later.
//
//   std::async          : "run this function and give me the result later".
//                         The simplest option - prefer it.
//   std::promise        : manually set the value from any thread.
//   std::packaged_task  : wraps a callable so its result goes into a future.
//
// Notes:
//   - future.get() blocks until the result is ready and can be called once.
//   - An exception thrown by the task is re-thrown by get().
//   - std::launch::async forces a new thread; the default policy may defer
//     the call until get() is invoked.
//
// Reference: https://en.cppreference.com/w/cpp/thread/future
// -----------------------------------------------------------------------------

#include <chrono>
#include <future>
#include <stdexcept>
#include <thread>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

using std::chrono::milliseconds;
using std::chrono::steady_clock;

/// Simulates a slow computation.
int heavyWork() {
  LOG("heavyWork: started (300 ms)");
  std::this_thread::sleep_for(milliseconds(300));
  LOG("heavyWork: done");
  return 1000;
}

/// Simulates the caller doing something useful while it waits.
void doOtherWork(steady_clock::time_point start) {
  for (int step = 1; step <= 3; ++step) {
    std::this_thread::sleep_for(milliseconds(80));
    const auto elapsed =
        std::chrono::duration_cast<milliseconds>(steady_clock::now() - start);
    LOG_S("caller: other work, step " << step << " at " << elapsed.count()
                                      << " ms");
  }
}

void asyncExample() {
  LOG_SECTION("std::async");
  const auto start = steady_clock::now();

  std::future<int> future = std::async(std::launch::async, heavyWork);
  doOtherWork(start);

  const int result = future.get();  // waits for heavyWork to finish
  const auto total =
      std::chrono::duration_cast<milliseconds>(steady_clock::now() - start);
  LOG_S("result = " << result << ", total " << total.count()
                    << " ms (work ran in parallel, not 300 + 240 ms)");
}

void promiseExample() {
  LOG_SECTION("std::promise + std::thread");

  std::promise<int> promise;
  std::future<int> future = promise.get_future();

  // The promise is moved into the thread; the future stays with the caller.
  std::thread worker(
      [p = std::move(promise)]() mutable { p.set_value(heavyWork()); });

  LOG_S("main: waiting, result = " << future.get());
  worker.join();
}

void packagedTaskExample() {
  LOG_SECTION("std::packaged_task");

  std::packaged_task<int(int, int)> task([](int a, int b) { return a * b; });
  std::future<int> future = task.get_future();

  std::thread worker(std::move(task), 6, 7);
  LOG_S("6 * 7 = " << future.get());
  worker.join();
}

void exceptionExample() {
  LOG_SECTION("Exceptions travel through the future");

  std::future<int> future = std::async(std::launch::async, []() -> int {
    throw std::runtime_error("failure inside the task");
  });

  try {
    const int value = future.get();  // re-throws the task's exception here
    LOG_S("unexpected value " << value);
  } catch (const std::exception& e) {
    LOG_S("caught in the caller: " << e.what());
  }
}

}  // namespace

LAB_EXAMPLE("FuturePromise",
            "std::async, std::promise, std::packaged_task and exceptions") {
  asyncExample();
  promiseExample();
  packagedTaskExample();
  exceptionExample();
}
