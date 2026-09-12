// -----------------------------------------------------------------------------
// Proxy (structural pattern)
//
// Provides a substitute for another object. The proxy implements the same
// interface and controls access to the real object, doing something before or
// after forwarding the request.
//
// Common kinds of proxies:
//   - virtual proxy    : creates an expensive object only when first needed
//   - protection proxy : checks permissions before forwarding
//   - logging proxy    : records requests
//   - remote proxy     : hides that the object lives in another process
//   (std::shared_ptr is a "smart reference" proxy for the pointee.)
//
// Problem: a heavy Server is constructed even if nobody ever calls it, and
// access checks and logging would have to be added inside the Server itself.
// Solution: ServerProxy adds lazy creation, access control and logging without
// changing Server.
//
// Participants:
//   Subject      Service interface
//   RealSubject  Server
//   Proxy        ServerProxy (same interface, owns the Server lazily)
//
// UML: docs/uml/dp/structural_proxy.drawio.svg
// -----------------------------------------------------------------------------

#include <memory>
#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// Subject.
class Service {
 public:
  virtual ~Service() = default;
  virtual void request(const std::string& user) = 0;
};

/// Real subject: expensive to create.
class Server : public Service {
 public:
  Server() {
    LOG("    Server: expensive construction (connect, load data...)");
  }
  void request(const std::string& user) override {
    LOG_S("    Server: handling request of " << user);
  }
};

void clientCode(Service& service, const std::string& user) {
  LOG_S("  client: request as " << user);
  service.request(user);
}

namespace problem {

void run() {
  LOG_SECTION("Problem: the real object is created up front");
  Server server;  // paid for even if nobody uses it
  clientCode(server, "admin");
  clientCode(server, "guest");  // no access control, no logging
}

}  // namespace problem

/// Proxy: lazy creation + access control + logging.
class ServerProxy : public Service {
 public:
  void request(const std::string& user) override {
    if (!hasAccess(user)) {
      LOG_S("    Proxy: access denied for " << user);
      return;
    }
    if (!server_) {
      LOG("    Proxy: first authorized request, creating the Server now");
      server_ = std::make_unique<Server>();
    }
    server_->request(user);
    ++requests_;
    LOG_S("    Proxy: logged request #" << requests_);
  }

 private:
  static bool hasAccess(const std::string& user) { return user == "admin"; }

  std::unique_ptr<Server> server_;  // created on demand
  int requests_{0};
};

void run() {
  LOG_SECTION("Proxy: same interface, controlled access");
  ServerProxy proxy;
  LOG("  proxy constructed - no Server exists yet");
  clientCode(proxy, "guest");  // denied, the Server is still not created
  clientCode(proxy, "admin");  // creates the Server lazily
  clientCode(proxy, "admin");  // reuses it
}

}  // namespace

LAB_EXAMPLE("Proxy",
            "a stand-in that adds lazy creation, access control and logging") {
  problem::run();
  run();
}
