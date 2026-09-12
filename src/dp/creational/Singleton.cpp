// -----------------------------------------------------------------------------
// Singleton (creational pattern)
//
// Ensures a class has only one instance and provides a global access point.
//
// Implementation in modern C++ ("Meyers' singleton"):
//   static Config& instance() { static Config config; return config; }
//   - The local static is created on first use.
//   - Since C++11 that initialization is thread-safe.
//   - Constructor private, copy and move deleted.
//
// Caution - a singleton is global state in disguise:
//   - hidden dependencies (functions use it without saying so)
//   - hard to test (tests share and modify the same instance)
//   - destruction order problems when other statics use it at shutdown
// Prefer passing dependencies explicitly; use a singleton only for things that
// really are unique (e.g. the lab's own Registry and Logger).
//
// UML: docs/uml/dp/creational_singleton.drawio.svg
// -----------------------------------------------------------------------------

#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Config {
 public:
  static Config& instance() {
    static Config config;  // created once, on the first call, thread-safe
    return config;
  }

  Config(const Config&) = delete;
  Config& operator=(const Config&) = delete;
  Config(Config&&) = delete;
  Config& operator=(Config&&) = delete;

  void set(const std::string& value) {
    const std::lock_guard<std::mutex> lock(
        mutex_);  // the instance is shared: protect it
    value_ = value;
  }

  std::string get() const {
    const std::lock_guard<std::mutex> lock(mutex_);
    return value_;
  }

 private:
  Config() { LOG("  Config constructed (happens exactly once)"); }
  ~Config() = default;

  mutable std::mutex mutex_;
  std::string value_{"default"};
};

void readSomewhereElse() {
  // A far-away function reaches the same object without any parameter.
  LOG_S("  readSomewhereElse() sees value = " << Config::instance().get());
}

void run() {
  LOG_SECTION("One instance, global access");
  LOG("  first call to Config::instance():");
  Config& config = Config::instance();
  config.set("0x001");
  readSomewhereElse();
  // Config copy = Config::instance();  // error: copy constructor is deleted
  // Config other;                     // error: constructor is private

  LOG_SECTION("Every thread gets the same object");
  std::vector<const Config*> seen(4, nullptr);
  std::vector<std::thread> threads;
  for (std::size_t i = 0; i < seen.size(); ++i) {
    threads.emplace_back([&seen, i] { seen[i] = &Config::instance(); });
  }
  for (std::thread& thread : threads) {
    thread.join();
  }
  bool all_same = true;
  for (const Config* address : seen) {
    all_same = all_same && address == &config;
  }
  LOG_S("  4 threads saw the same address: " << std::boolalpha << all_same);
}

}  // namespace

LAB_EXAMPLE("Singleton",
            "Meyers' singleton: one lazily created, thread-safe instance") {
  run();
}
