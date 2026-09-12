// -----------------------------------------------------------------------------
// Chain of Responsibility (behavioral pattern)
//
// Passes a request along a chain of handlers. Each handler decides to handle
// the request, to reject it, or to pass it to the next handler. The sender only
// knows the first handler of the chain.
//
// Use it when:
//   - several objects may handle a request and the handler is not known upfront
//   - handlers must run in a specific order that can change at run time
//
// Real world: web-server middleware, GUI event bubbling, support escalation.
//
// Participants:
//   Handler          interface + link to the next handler (Handler)
//   ConcreteHandler  AuthenticationHandler, LoggingHandler, GetHandler, PostHandler
//   Client           builds the chain and sends requests to its head
//
// UML: docs/uml/dp/behavioral_CoR.drawio.svg
// -----------------------------------------------------------------------------

#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

struct Request {
  std::string method;
  std::string path;
  std::string token;
};

/// Handler: stores the next link and forwards by default.
class Handler {
 public:
  virtual ~Handler() = default;

  /// Returns `next` so a chain can be written as a.setNext(b).setNext(c).
  Handler& setNext(Handler& next) {
    next_ = &next;
    return next;
  }

  virtual void handle(const Request& request) const {
    if (next_ != nullptr) {
      next_->handle(request);
    } else {
      LOG_S("  [end of chain] nobody handled " << request.method << ' '
                                               << request.path);
    }
  }

 private:
  const Handler* next_{nullptr};  // non-owning: the client owns all handlers
};

/// Can STOP the chain.
class AuthenticationHandler : public Handler {
 public:
  void handle(const Request& request) const override {
    if (request.token != "secret") {
      LOG_S("  [auth] rejected " << request.path << " (invalid token)");
      return;  // not forwarded
    }
    Handler::handle(request);
  }
};

/// Does some work and always PASSES the request on.
class LoggingHandler : public Handler {
 public:
  void handle(const Request& request) const override {
    LOG_S("  [log]  " << request.method << ' ' << request.path);
    Handler::handle(request);
  }
};

/// HANDLES the requests it is responsible for, forwards the others.
class MethodHandler : public Handler {
 public:
  explicit MethodHandler(std::string method) : method_{std::move(method)} {}

  void handle(const Request& request) const override {
    if (request.method == method_) {
      LOG_S("  [" << method_ << "]  handled " << request.path);
      return;
    }
    Handler::handle(request);
  }

 private:
  std::string method_;
};

void run() {
  LOG_SECTION("Building the chain: auth -> log -> GET -> POST");
  AuthenticationHandler auth;
  LoggingHandler logging;
  MethodHandler get{"GET"};
  MethodHandler post{"POST"};
  auth.setNext(logging).setNext(get).setNext(post);

  const auto send = [&auth](const Request& request) {
    LOG_S("client sends " << request.method << ' ' << request.path);
    auth.handle(request);  // the client only talks to the head of the chain
  };

  send({"GET", "/users/123", "secret"});
  send({"POST", "/login", "secret"});
  send({"DELETE", "/users/123", "secret"});
  send({"GET", "/admin", "wrong"});

  LOG_SECTION("Changing the chain at run time: skip authentication");
  logging.handle({"POST", "/public/feedback", ""});
}

}  // namespace

LAB_EXAMPLE("ChainOfResponsibility",
            "pass a request along handlers that handle, forward or stop it") {
  run();
}
