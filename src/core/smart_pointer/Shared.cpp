// -----------------------------------------------------------------------------
// std::shared_ptr - shared ownership
//
//   auto p = std::make_shared<T>(args...);
//
//   - Several shared_ptrs own the same object; a reference count in a
//     "control block" tracks them. The object is deleted with the last owner.
//   - Copying increments the count (atomically - thread-safe count, NOT a
//     thread-safe object). Moving transfers without touching the count.
//   - make_shared allocates the object and control block together.
//   - shared_ptr<const T> shares read-only access.
//   - Pitfall: two objects owning each other form a cycle and are never freed
//     (see Weak.cpp). Prefer unique_ptr unless ownership is really shared.
//
// Reference: https://en.cppreference.com/w/cpp/memory/shared_ptr
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
      : server{std::move(server_name)}, port{port_number} {
    LOG("  AppConfig loaded");
  }
  ~AppConfig() { LOG("  AppConfig destroyed (last owner gone)"); }
  AppConfig(const AppConfig&) = delete;
  AppConfig& operator=(const AppConfig&) = delete;
};

/// Module A: shares the configuration read-only.
class NetworkManager {
 public:
  explicit NetworkManager(std::shared_ptr<const AppConfig> config)
      : config_{std::move(config)} {}
  void connect() const {
    LOG_S("  connecting to " << config_->server << ':' << config_->port);
  }

 private:
  std::shared_ptr<const AppConfig>
      config_;  // cannot modify the config through this
};

/// Module B: shares the same configuration.
class RequestLogger {
 public:
  explicit RequestLogger(std::shared_ptr<const AppConfig> config)
      : config_{std::move(config)} {}
  void start() const { LOG_S("  logging requests for " << config_->server); }

 private:
  std::shared_ptr<const AppConfig> config_;
};

void sharedOwnership() {
  LOG_SECTION("Several owners, one object");
  auto config = std::make_shared<AppConfig>("test.server.com", 80);
  LOG_S("  use_count after make_shared: " << config.use_count());
  {
    const NetworkManager network{config};
    {
      const RequestLogger logger{config};
      LOG_S("  use_count with two modules:  " << config.use_count());
      network.connect();
      logger.start();

      config->port = 8080;  // the owner with non-const access changes it...
      network.connect();    // ...and every module sees the change
    }
    LOG_S("  logger gone, use_count:     " << config.use_count());
  }
  LOG_S("  network gone, use_count:    " << config.use_count());

  const std::shared_ptr<AppConfig> moved =
      std::move(config);  // no count change
  LOG_S("  after std::move: config is "
        << (config ? "set" : "empty")
        << ", moved.use_count() = " << moved.use_count());
  LOG("  -- leaving the function --");
}

class Session : public std::enable_shared_from_this<Session> {
 public:
  /// Hands out another owner of *this. Never do std::shared_ptr<Session>(this):
  /// that would create a second, independent control block (double delete).
  std::shared_ptr<Session> self() { return shared_from_this(); }
  ~Session() { LOG("  Session destroyed"); }
};

void sharedFromThis() {
  LOG_SECTION("enable_shared_from_this");
  const auto session = std::make_shared<Session>();
  [[maybe_unused]] const std::shared_ptr<Session> another = session->self();
  LOG_S("  use_count = " << session.use_count() << " (same control block)");
}

void customDeleter() {
  LOG_SECTION("Custom deleter");
  const std::shared_ptr<int> value(new int{42}, [](const int* pointer) {
    LOG_S("  custom deleter frees " << *pointer);
    delete pointer;
  });
  LOG_S("  *value = " << *value);
}

}  // namespace

LAB_EXAMPLE(
    "Shared",
    "std::shared_ptr: reference counting, const sharing, shared_from_this") {
  sharedOwnership();
  sharedFromThis();
  customDeleter();
}
