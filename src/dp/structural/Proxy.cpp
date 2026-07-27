// Composition - lazy
// Aggregation - x

#include <memory>
#include <utility>

#include "ExampleRegistry.h"
#include "Logger.h"

namespace {
/// @class Server Interface
class IServer {
 public:
  virtual ~IServer() = default;

  /// @brief api
  virtual void request() = 0;
};

/// @brief User code
void clientCode(IServer* s) {
  if (s != nullptr) {
    s->request();
  }
}

namespace problem {
const std::string kAdmin = "admin";
class Server : public IServer {
 private:
  std::string id_;

 public:
  explicit Server(std::string id) : id_{std::move(id)} {
    // [P1] Heavy or complex construction, so ideally should be lazy-loaded
    LOG_S("CTR: " << id_);
  }

  // [P2] Need access control
  // [P3] Need to log requests without modifying the Server itself
  void request() override {
    if (id_ != kAdmin) {
      LOG_S("Invalid ID: " << id_);
      return;
    }
    LOG_S("Handling request for: " << id_);
  }
};

void run() {
  {
    std::string connection_id = "admin";
    // [P4] The Server is constructed immediately even if we do not call any
    // requests
    auto server = std::make_unique<Server>(connection_id);
    LOG("User request");
    clientCode(server.get());
  }

  {
    // [P4] Server is constructed even for invalid ID, wasting resources
    std::string invalid_id = "xxx";
    auto server = std::make_unique<Server>(invalid_id);
    LOG("User request");
    clientCode(server.get());
  }
}
}  // namespace problem

namespace proxy_pattern {
const std::string kAdmin = "admin";
class Server : public IServer {
 private:
  std::string id_;

 public:
  explicit Server(std::string id) : id_{std::move(id)} {
    LOG_S("CTR: " << id_);
  }

  void request() override { LOG_S("Handling request for: " << id_); }
};

class ServerProxy : public IServer {
 private:
  std::string id_;
  std::unique_ptr<Server> server_;

  bool checkAccess() {
    LOG("Checking access before forwarding request.");
    if (id_ != kAdmin) {
      LOG("Invalid id. Return");
      return false;
    }

    // Lazy initialization: construct Server only on first access
    if (server_ == nullptr) {
      server_ = std::make_unique<Server>(id_);
    }
    return true;
  }

  void logAccess() const { LOG_S("Logging request time: " << id_); }

 public:
  explicit ServerProxy(std::string id) : id_{std::move(id)} {
    LOG_S("CTR: " << id_);
  }

  void request() override {
    if (checkAccess()) {
      server_->request();
      logAccess();
    }
  }
};

void run() {
  {
    std::string connection_id = "admin";
    // Server is not constructed until first request is made
    auto server_proxy = std::make_unique<ServerProxy>(connection_id);
    LOG("User request");
    clientCode(server_proxy.get());
  }

  {
    // Server is not constructed if id is invalid
    std::string invalid_id = "xxx";
    auto server_proxy = std::make_unique<ServerProxy>(invalid_id);
    LOG("User request");
    clientCode(server_proxy.get());
  }
}
}  // namespace proxy_pattern

class ProxyExample : public IExample {
 public:
  std::string group() const override { return "dp/structural"; }
  std::string name() const override { return "Proxy"; }
  std::string description() const override { return "Proxy Pattern Example"; }
  void execute() override {
    problem::run();
    proxy_pattern::run();
  }
};

REGISTER_EXAMPLE(ProxyExample);
}  // namespace
