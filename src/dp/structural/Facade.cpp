// -----------------------------------------------------------------------------
// Facade (structural pattern)
//
// Provides a simple interface to a complex subsystem. Clients call the facade
// instead of orchestrating many subsystem classes themselves.
//
// Problem: to send one request, every client must validate, authenticate, log
// and call the backend - in the right order, with the right error handling.
// The same orchestration gets copied everywhere.
// Solution: RequestFacade::send() does it once. Clients that need more control
// can still use the subsystems directly.
//
// Use it when:
//   - you want a limited but straightforward interface to a complex subsystem
//   - you want to structure a system into layers
//
// Injecting the subsystems (instead of creating them inside the facade) also
// makes the facade easy to test with fakes, as shown at the end.
//
// UML: docs/uml/dp/structural_facade.drawio.svg
// -----------------------------------------------------------------------------

#include <memory>
#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// --- Subsystem classes -------------------------------------------------------

class Validator {
 public:
  bool check(const std::string& payload) const {
    const bool ok = !payload.empty();
    LOG_S("    Validator: payload " << (ok ? "is valid" : "is empty"));
    return ok;
  }
};

class Authenticator {
 public:
  bool login(const std::string& user) const {
    LOG_S("    Authenticator: login " << user);
    return user != "guest";
  }
};

class AuditLog {
 public:
  void write(const std::string& entry) const {
    LOG_S("    AuditLog: " << entry);
  }
};

class Backend {
 public:
  virtual ~Backend() = default;
  virtual void send(const std::string& payload) const {
    LOG_S("    Backend: POST " << payload);
  }
};

namespace problem {

void run() {
  LOG_SECTION("Problem: every client orchestrates the subsystem itself");
  const Validator validator;
  const Authenticator authenticator;
  const AuditLog log;
  const Backend backend;

  const std::string payload = R"({"order": 42})";
  if (validator.check(payload) &&
      authenticator.login("alice")) {  // must know the order...
    log.write("alice sends an order");
    backend.send(payload);  // ...and every step
  }
}

}  // namespace problem

// --- Facade ------------------------------------------------------------------

class RequestFacade {
 public:
  explicit RequestFacade(
      std::unique_ptr<Backend> backend = std::make_unique<Backend>())
      : backend_{std::move(backend)} {}

  /// One call hides the whole workflow.
  bool send(const std::string& user, const std::string& payload) const {
    if (!validator_.check(payload)) {
      return false;
    }
    if (!authenticator_.login(user)) {
      log_.write(user + " was rejected");
      return false;
    }
    log_.write(user + " sends a request");
    backend_->send(payload);
    return true;
  }

 private:
  Validator validator_;
  Authenticator authenticator_;
  AuditLog log_;
  std::unique_ptr<Backend> backend_;
};

/// A fake backend for tests: no network access.
class FakeBackend : public Backend {
 public:
  void send(const std::string& payload) const override {
    LOG_S("    FakeBackend: pretended to send " << payload);
  }
};

void run() {
  LOG_SECTION("Facade: clients call one simple method");
  const RequestFacade facade;
  LOG_S("  result: " << std::boolalpha
                     << facade.send("alice", R"({"order": 42})"));
  LOG_S("  result: " << std::boolalpha
                     << facade.send("guest", R"({"order": 43})"));
  LOG_S("  result: " << std::boolalpha << facade.send("alice", ""));

  LOG_SECTION("Injecting a fake subsystem for testing");
  const RequestFacade test_facade{std::make_unique<FakeBackend>()};
  test_facade.send("tester", R"({"order": "test"})");
}

}  // namespace

LAB_EXAMPLE("Facade",
            "one simple entry point in front of a complex subsystem") {
  problem::run();
  run();
}
