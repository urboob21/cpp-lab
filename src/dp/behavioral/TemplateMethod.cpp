// -----------------------------------------------------------------------------
// Template Method (behavioral pattern)
//
// Defines the skeleton of an algorithm in a base class and lets subclasses
// override specific steps without changing the overall structure.
//
// Kinds of steps:
//   - fixed steps     : implemented in the base class, not virtual
//   - abstract steps  : pure virtual, every subclass MUST implement them
//   - hooks           : virtual with a default, subclasses MAY override them
//
// Use it when:
//   - several classes contain almost identical algorithms with small differences
//   - clients should extend only particular steps, not the whole algorithm
//
// C++ note: the public template method is non-virtual and the customizable
// steps are private virtual functions - the Non-Virtual Interface (NVI) idiom.
//
// UML: docs/uml/dp/behavioral_templatemethod.drawio.svg
// -----------------------------------------------------------------------------

#include <memory>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// Abstract class with the template method.
class Beverage {
 public:
  virtual ~Beverage() = default;

  /// The template method: fixed order, cannot be overridden.
  void prepare() const {
    boilWater();
    brew();
    pourInCup();
    if (wantsCondiments()) {
      addCondiments();
    }
    LOG("    ready!");
  }

 private:
  // Fixed steps.
  void boilWater() const { LOG("    boil water"); }
  void pourInCup() const { LOG("    pour into cup"); }

  // Abstract steps: subclasses must provide them.
  virtual void brew() const = 0;
  virtual void addCondiments() const = 0;

  // Hook: optional customization point with a default.
  virtual bool wantsCondiments() const { return true; }
};

class Coffee : public Beverage {
 private:
  void brew() const override { LOG("    drip coffee through the filter"); }
  void addCondiments() const override { LOG("    add sugar and milk"); }
};

class Tea : public Beverage {
 private:
  void brew() const override { LOG("    steep the tea bag"); }
  void addCondiments() const override { LOG("    add lemon"); }
};

class BlackCoffee : public Coffee {
 private:
  bool wantsCondiments() const override {
    return false;
  }  // overrides only the hook
};

void run() {
  LOG_SECTION("Same algorithm, different steps");
  std::vector<std::pair<const char*, std::unique_ptr<Beverage>>> orders;
  orders.emplace_back("Coffee", std::make_unique<Coffee>());
  orders.emplace_back("Tea", std::make_unique<Tea>());
  orders.emplace_back("Black coffee (hook overridden)",
                      std::make_unique<BlackCoffee>());

  for (const auto& [name, beverage] : orders) {
    LOG_S("  " << name << ':');
    beverage->prepare();  // client code only calls the template method
  }
}

}  // namespace

LAB_EXAMPLE(
    "TemplateMethod",
    "algorithm skeleton in a base class, steps and hooks in subclasses") {
  run();
}
