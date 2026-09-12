// -----------------------------------------------------------------------------
// std::weak_ptr - observe without owning
//
//   std::weak_ptr<T> observer = shared;   does NOT increase the owner count
//   if (auto locked = observer.lock())    temporary shared_ptr, empty if expired
//
// Uses:
//   - Caches and observers that must not keep objects alive.
//   - Breaking reference cycles: if A owns B and B owns A through shared_ptrs,
//     neither count ever reaches zero and both leak. Make one side weak.
//
// In multithreaded code use lock() rather than expired(): the object can be
// destroyed right after expired() returned false.
//
// Reference: https://en.cppreference.com/w/cpp/memory/weak_ptr
// -----------------------------------------------------------------------------

#include <memory>
#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

struct AppConfig {
  std::string server;
  int port;

  AppConfig(std::string server_name, int port_number)
      : server{std::move(server_name)}, port{port_number} {}
  ~AppConfig() { LOG("  AppConfig destroyed"); }
  AppConfig(const AppConfig&) = delete;
  AppConfig& operator=(const AppConfig&) = delete;
};

void useIfAlive(const std::weak_ptr<AppConfig>& observer) {
  if (const std::shared_ptr<AppConfig> config = observer.lock()) {
    LOG_S("  lock() succeeded: " << config->server << ':' << config->port);
  } else {
    LOG("  lock() returned empty: the config is gone");
  }
}

void observing() {
  LOG_SECTION("lock() and expired()");
  std::weak_ptr<AppConfig> observer;
  {
    const auto config = std::make_shared<AppConfig>("test.server.com", 80);
    observer = config;
    LOG_S("  use_count = " << config.use_count()
                           << " (the weak_ptr does not count)");
    LOG_S("  expired() = " << std::boolalpha << observer.expired());
    useIfAlive(observer);
    LOG("  -- last shared_ptr goes out of scope --");
  }
  LOG_S("  expired() = " << std::boolalpha << observer.expired());
  useIfAlive(observer);
}

namespace cycle {

struct Child;

struct Parent {
  std::shared_ptr<Child> child;
  ~Parent() { LOG("  ~Parent"); }
};

struct StrongChild {
  std::shared_ptr<struct StrongParent> parent;
  ~StrongChild() { LOG("  ~StrongChild"); }
};

struct StrongParent {
  std::shared_ptr<StrongChild> child;
  ~StrongParent() { LOG("  ~StrongParent"); }
};

struct Child {
  std::weak_ptr<Parent> parent;  // weak back-reference breaks the cycle
  ~Child() { LOG("  ~Child"); }
};

void run() {
  LOG_SECTION("Reference cycle with shared_ptr on both sides");
  {
    auto parent = std::make_shared<StrongParent>();
    parent->child = std::make_shared<StrongChild>();
    parent->child->parent = parent;  // cycle: parent <-> child
    LOG_S("  parent.use_count() = "
          << parent.use_count() << " (local variable + the child's pointer)");
    const std::weak_ptr<StrongParent> watch = parent;
    parent.reset();  // drop our own reference
    LOG_S("  after parent.reset() the parent is still alive: "
          << std::boolalpha << !watch.expired());
    LOG("  -> nothing will ever free these objects: a memory leak");

    // Cut the loop by hand so this lab stays leak-free. Both destructors run:
    watch.lock()->child->parent.reset();
  }

  LOG_SECTION("The same with a weak_ptr back-reference");
  {
    auto parent = std::make_shared<Parent>();
    parent->child = std::make_shared<Child>();
    parent->child->parent = parent;  // weak: does not count
    LOG_S("  parent.use_count() = " << parent.use_count());
    if (const auto locked = parent->child->parent.lock()) {
      LOG("  the child can still reach its parent through lock()");
    }
    LOG("  -- leaving scope: both destructors run --");
  }
}

}  // namespace cycle

}  // namespace

LAB_EXAMPLE(
    "Weak",
    "std::weak_ptr: lock/expired, observers and breaking shared_ptr cycles") {
  observing();
  cycle::run();
}
