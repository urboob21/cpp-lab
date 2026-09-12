// -----------------------------------------------------------------------------
// Adapter (structural pattern)
//
// Lets objects with incompatible interfaces work together. An adapter wraps an
// existing class (the adaptee) and exposes the interface the client expects
// (the target).
//
// Use it when:
//   - you want to use an existing or third-party class whose interface does not
//     match the rest of your code
//   - you cannot (or should not) modify that class
//
// Participants:
//   Target   interface the client depends on
//   Adaptee  existing class with an incompatible interface
//   Adapter  implements Target by translating calls to the Adaptee
//
// Two variants: the OBJECT adapter holds the adaptee (composition, preferred);
// the CLASS adapter inherits from it (here: private inheritance).
//
// UML: docs/uml/dp/structural_adapter.drawio.svg
// -----------------------------------------------------------------------------

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace basic {

/// Target: the interface the client code expects.
class Target {
 public:
  virtual ~Target() = default;
  virtual void request() = 0;
};

/// Adaptee: useful, but with a different interface.
class Adaptee {
 public:
  void specificRequest(int code) const {
    LOG_S("    Adaptee::specificRequest(" << code << ")");
  }
};

/// Object adapter: HAS an adaptee.
class ObjectAdapter : public Target {
 public:
  explicit ObjectAdapter(std::unique_ptr<Adaptee> adaptee)
      : adaptee_{std::move(adaptee)} {}
  void request() override { adaptee_->specificRequest(42); }

 private:
  std::unique_ptr<Adaptee> adaptee_;
};

/// Class adapter: IS implemented in terms of the adaptee (private inheritance).
class ClassAdapter : public Target, private Adaptee {
 public:
  void request() override { specificRequest(7); }
};

void clientCode(Target& target) {
  LOG("  client calls request()");
  target.request();
}

void run() {
  LOG_SECTION("Object adapter and class adapter");
  ObjectAdapter object_adapter{std::make_unique<Adaptee>()};
  clientCode(object_adapter);
  ClassAdapter class_adapter;
  clientCode(class_adapter);
}

}  // namespace basic

namespace payments {

/// Target: what the shop checkout already uses.
class PaymentProcessor {
 public:
  virtual ~PaymentProcessor() = default;
  virtual bool pay(const std::string& customer, double amount_eur) = 0;
};

class CardProcessor : public PaymentProcessor {
 public:
  bool pay(const std::string& customer, double amount_eur) override {
    LOG_S("    [card] charged " << amount_eur << " EUR to the card of "
                                << customer);
    return true;
  }
};

/// Adaptee: a third-party SDK we cannot change. Different method name,
/// different parameters, amounts in cents and an error code instead of bool.
class PayPalSdk {
 public:
  int sendPayment(const std::string& email, long amount_cents,
                  const std::string& currency) const {
    LOG_S("    [PayPal SDK] sent " << amount_cents << " cents (" << currency
                                   << ") to " << email);
    return 0;  // 0 = success
  }
};

/// Adapter: translates the checkout's call into the SDK's call.
class PayPalAdapter : public PaymentProcessor {
 public:
  bool pay(const std::string& customer, double amount_eur) override {
    const std::string email =
        customer + "@example.com";  // look up the PayPal account
    const auto cents = static_cast<long>(amount_eur * 100.0 + 0.5);
    return sdk_.sendPayment(email, cents, "EUR") == 0;
  }

 private:
  PayPalSdk sdk_;
};

void checkout(PaymentProcessor& processor, const std::string& customer,
              double amount) {
  const bool ok = processor.pay(customer, amount);  // unchanged client code
  LOG_S("  checkout for " << customer << ": " << (ok ? "paid" : "failed"));
}

void run() {
  LOG_SECTION("Case study: plugging a third-party SDK into existing code");
  CardProcessor card;
  PayPalAdapter paypal;
  checkout(card, "alice", 19.99);
  checkout(paypal, "bob", 42.50);
}

}  // namespace payments

}  // namespace

LAB_EXAMPLE("Adapter",
            "make an incompatible class fit the interface the client expects") {
  basic::run();
  payments::run();
}
