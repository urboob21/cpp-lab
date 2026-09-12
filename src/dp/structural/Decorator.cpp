// -----------------------------------------------------------------------------
// Decorator (structural pattern)
//
// Attaches new behavior to an object by wrapping it in decorator objects that
// implement the same interface. Decorators can be stacked in any combination
// at run time.
//
// Problem: a notifier that can also send SMS, Slack or both, with or without
// email... Subclasses for every combination explode: 3 optional features
// already need 2^3 = 8 classes.
// Solution: one decorator per feature; combine them by wrapping:
//   SlackDecorator(SmsDecorator(EmailNotifier))
//
// Use it when:
//   - responsibilities should be added to individual objects at run time
//   - extending by subclassing is impractical or impossible (final classes)
//
// Participants:
//   Component           Notifier interface
//   ConcreteComponent   EmailNotifier
//   Decorator           NotifierDecorator - holds and forwards to a Notifier
//   ConcreteDecorator   SmsDecorator, SlackDecorator
//
// UML: docs/uml/dp/structural_decorator.drawio.svg
// -----------------------------------------------------------------------------

#include <memory>
#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// Component.
class Notifier {
 public:
  virtual ~Notifier() = default;
  virtual void send(const std::string& message) const = 0;
};

namespace problem {

class EmailNotifier : public Notifier {
 public:
  void send(const std::string& message) const override {
    LOG_S("    email: " << message);
  }
};

class EmailAndSmsNotifier : public EmailNotifier {
 public:
  void send(const std::string& message) const override {
    EmailNotifier::send(message);
    LOG_S("    sms  : " << message);
  }
};

class EmailAndSlackNotifier : public EmailNotifier {
 public:
  void send(const std::string& message) const override {
    EmailNotifier::send(message);
    LOG_S("    slack: " << message);
  }
};

class EmailAndSmsAndSlackNotifier : public EmailAndSmsNotifier {
 public:
  void send(const std::string& message) const override {
    EmailAndSmsNotifier::send(message);
    LOG_S("    slack: " << message);
  }
};
// ...plus SmsOnly, SlackOnly, SmsAndSlack... one class per combination.

void run() {
  LOG_SECTION("Problem: one subclass per combination");
  const EmailAndSmsAndSlackNotifier notifier;
  notifier.send("server is down");
}

}  // namespace problem

namespace decorator {

/// Concrete component.
class EmailNotifier : public Notifier {
 public:
  void send(const std::string& message) const override {
    LOG_S("    email: " << message);
  }
};

/// Base decorator: same interface, forwards to the wrapped object.
class NotifierDecorator : public Notifier {
 public:
  explicit NotifierDecorator(std::unique_ptr<Notifier> wrapped)
      : wrapped_{std::move(wrapped)} {}
  void send(const std::string& message) const override {
    wrapped_->send(message);
  }

 private:
  std::unique_ptr<Notifier> wrapped_;
};

/// Concrete decorators add behavior before/after forwarding.
class SmsDecorator : public NotifierDecorator {
 public:
  using NotifierDecorator::NotifierDecorator;
  void send(const std::string& message) const override {
    NotifierDecorator::send(message);
    LOG_S("    sms  : " << message);
  }
};

class SlackDecorator : public NotifierDecorator {
 public:
  using NotifierDecorator::NotifierDecorator;
  void send(const std::string& message) const override {
    NotifierDecorator::send(message);
    LOG_S("    slack: " << message);
  }
};

class UrgentDecorator
    : public NotifierDecorator {  // changes the message itself
 public:
  using NotifierDecorator::NotifierDecorator;
  void send(const std::string& message) const override {
    NotifierDecorator::send("[URGENT] " + message);
  }
};

void run() {
  LOG_SECTION("Decorator: combine features by wrapping");
  std::unique_ptr<Notifier> notifier = std::make_unique<EmailNotifier>();
  LOG("  email only:");
  notifier->send("build finished");

  notifier = std::make_unique<SmsDecorator>(std::move(notifier));
  notifier = std::make_unique<SlackDecorator>(std::move(notifier));
  LOG("  email + sms + slack:");
  notifier->send("deploy started");

  notifier = std::make_unique<UrgentDecorator>(std::move(notifier));
  LOG("  wrapped once more with UrgentDecorator:");
  notifier->send("server is down");
}

}  // namespace decorator

}  // namespace

LAB_EXAMPLE(
    "Decorator",
    "stack wrappers at run time instead of subclassing every combination") {
  problem::run();
  decorator::run();
}
