// -----------------------------------------------------------------------------
// Managing std::thread
//
//   join()       : wait for the thread to finish
//   detach()     : let it run on its own; you can no longer wait for it
//   joinable()   : true while the std::thread still owns a thread of execution
//
// A std::thread that is still joinable when destroyed calls std::terminate().
// Protect against early returns and exceptions with RAII:
//   - a small guard class that joins in its destructor, or
//   - std::jthread (C++20), which joins automatically and supports stop requests.
//
// Reference: https://en.cppreference.com/w/cpp/thread/thread
// -----------------------------------------------------------------------------

#include <chrono>
#include <future>
#include <stdexcept>
#include <stop_token>
#include <thread>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

using std::chrono::milliseconds;

void logJoinable(const std::thread& thread) {
  LOG_S("  joinable() = " << std::boolalpha << thread.joinable());
}

namespace join {

void run() {
  LOG_SECTION("join()");
  LOG_S("hardware threads available: " << std::thread::hardware_concurrency());

  std::thread worker;  // default-constructed: no thread yet
  logJoinable(worker);

  worker = std::thread([] {
    for (int i = 0; i < 3; ++i) {
      LOG_S("  worker step " << i << " on thread "
                             << std::this_thread::get_id());
      std::this_thread::sleep_for(milliseconds(5));
    }
  });
  logJoinable(worker);

  for (int i = 0; i < 3; ++i) {
    LOG_S("  main step " << i << " on thread " << std::this_thread::get_id());
    std::this_thread::sleep_for(milliseconds(5));
  }

  worker.join();  // blocks until the worker has finished
  logJoinable(worker);
}

}  // namespace join

namespace detach {

void run() {
  LOG_SECTION("detach()");
  std::promise<void> finished;
  std::future<void> finished_future = finished.get_future();

  std::thread background([done = std::move(finished)]() mutable {
    std::this_thread::sleep_for(milliseconds(20));
    LOG("  detached thread: finished its work");
    done.set_value();
  });
  background.detach();
  logJoinable(background);

  // After detach() nobody can join the thread. Something else must tell us
  // when it is done - here a promise. A detached thread still running when
  // main() returns is killed abruptly, so detach rarely is the right tool.
  finished_future.wait();
  LOG("  main: got the 'finished' signal");
}

}  // namespace detach

namespace exception_safety {

/// Joins the thread when the guard goes out of scope - also during stack
/// unwinding after an exception.
class ThreadGuard {
 public:
  explicit ThreadGuard(std::thread& thread) : thread_{thread} {}
  ~ThreadGuard() {
    if (thread_.joinable()) {
      LOG("  ThreadGuard: joining in the destructor");
      thread_.join();
    }
  }
  ThreadGuard(const ThreadGuard&) = delete;
  ThreadGuard& operator=(const ThreadGuard&) = delete;

 private:
  std::thread& thread_;
};

void mayThrow() {
  throw std::runtime_error("error while the worker is running");
}

void run() {
  LOG_SECTION("Exceptions and RAII (ThreadGuard)");
  try {
    std::thread worker([] {
      std::this_thread::sleep_for(milliseconds(50));
      LOG("  worker: done");
    });
    const ThreadGuard guard(worker);
    // Without the guard, this throw would destroy a joinable std::thread and
    // call std::terminate().
    mayThrow();
  } catch (const std::exception& e) {
    LOG_S("  caught: " << e.what());
  }
}

}  // namespace exception_safety

namespace jthread {

void run() {
  LOG_SECTION("std::jthread (C++20): auto-join and cooperative stop");
  {
    std::jthread ticker([](const std::stop_token& token) {
      int tick = 0;
      while (
          !token.stop_requested()) {  // the thread checks the stop flag itself
        LOG_S("  tick " << ++tick);
        std::this_thread::sleep_for(milliseconds(10));
      }
      LOG("  stop requested, ticker exits");
    });
    std::this_thread::sleep_for(milliseconds(35));
    LOG("  leaving scope: ~jthread calls request_stop() and join()");
  }
}

}  // namespace jthread

}  // namespace

LAB_EXAMPLE("ThreadManagement",
            "join, detach, joinable, RAII thread guards and std::jthread") {
  join::run();
  detach::run();
  exception_safety::run();
  jthread::run();
}
