#include <memory>
#include "Logger.h"

#include "ExampleRegistry.h"

namespace adapter_pattern {

/// @class Adaptee
/// @brief Existing class with an incompatible interface
class Adaptee {
 public:
  void specific_request() {
    ++dummy_;
    LOG("Adaptee::specific_request()");
  }

 private:
  int dummy_{0};
};

/// @class Target
/// @brief Interface expected by the client
class Target {
 public:
  virtual ~Target() = default;

  virtual void request() = 0;
};

/// @class ConcreteTarget
/// @brief A normal implementation of Target
class ConcreteTarget : public Target {
 public:
  void request() override { LOG("request"); }
};

/// @class Adapter
/// @brief Converts the Target interface into the Adaptee interface
class Adapter : public Target {
 public:
  explicit Adapter(std::unique_ptr<Adaptee> adaptee)
      : adaptee_(std::move(adaptee)) {
    LOG("CTR");
  }

  void request() override { adaptee_->specific_request(); }

 private:
  std::unique_ptr<Adaptee> adaptee_;
};

/// @brief Client code only depends on Target.
void client_code(Target& target) {
  LOG("");
  target.request();
}

void run() {
  LOG("Adapter Example");
  {
    ConcreteTarget target;
    client_code(target);
  }
  LOG("");

  {
    auto adaptee = std::make_unique<Adaptee>();
    Adapter adapter(std::move(adaptee));
    client_code(adapter);
  }
}
}  // namespace adapter_pattern

namespace case_study {
// Target interface expected by the existing system
class PaymentSystem {
 public:
  virtual void pay_with_card(const std::string& card_number) {
    LOG_S("Payment using card: " << card_number);
  }

  virtual ~PaymentSystem() = default;
};

// Adaptee: a new payment API with an incompatible interface
class PayPalAPI {
 public:
  void send_payment(const std::string& email) {
    LOG_S("Payment sent via PayPal to " << email);
    dummy_++;
  }

 private:
  int dummy_{};
};

// Adapter: makes PayPalAPI compatible with PaymentSystem
class PayPalAdapter : public PaymentSystem {
 private:
  PayPalAPI paypal_;

 public:
  void pay_with_card(const std::string& cardNumber) override {
    // Treat the cardNumber parameter as a PayPal email
    paypal_.send_payment(cardNumber);
  }
};

// Client code: uses the old interface without modification
void run() {
  LOG("Case Study Example");
  std::string method;
  std::string input;
  method = std::string("card") + std::string("");
  input = "1234-5678-9999";
  // method = std::string("paypal") + std::string("");input =
  // "user@example.com";

  LOG_S("Choose payment method (card/paypal): " << method);

  PaymentSystem* payment_system = nullptr;

  if (method == "card") {
    payment_system = new PaymentSystem();
    payment_system->pay_with_card(input);
  } else if (method == "paypal") {
    payment_system = new PayPalAdapter();
    payment_system->pay_with_card(input);
  } else {
    LOG("Unsupported payment method!");
  }

  delete payment_system;
}
}  // namespace case_study

class AdapterExample : public IExample {
 public:
  std::string group() const override { return "dp/structural"; }
  std::string name() const override { return "Adapter"; }
  std::string description() const override { return "Factory Pattern Example"; }
  void execute() override {
    adapter_pattern::run();
    case_study::run();
  }
};

REGISTER_EXAMPLE(AdapterExample);