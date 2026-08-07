#include <memory>
#include <string>
#include "Logger.h"

#include "ExampleRegistry.h"

namespace {

class IComponent {
 public:
  virtual ~IComponent() = default;
  virtual std::string operation() const = 0;
};

auto client_code = [](const IComponent* comp) {
  LOG(comp->operation());
};

namespace problem {
class ConcreteComponent : public IComponent {
 public:
  std::string operation() const override { return "ConcreteComponent"; }
};

class ComponentWithA : public ConcreteComponent {
 public:
  std::string operation() const override {
    return ConcreteComponent::operation() + " + FeatureA";
  }
};

class ComponentWithB : public ConcreteComponent {
 public:
  std::string operation() const override {
    return ConcreteComponent::operation() + " + FeatureB";
  }
};

class ComponentWithAandB : public ConcreteComponent {
 public:
  std::string operation() const override {
    return ConcreteComponent::operation() + " + FeatureA + FeatureB";
  }
};

void run() {
  // [P1]
  // If you have 3 features , e.g FeatureC -> many combinations
  // If you have 5 features -> 32 subclasses
  LOG("Problem");
  std::unique_ptr<IComponent> simple = std::make_unique<ConcreteComponent>();
  client_code(simple.get());

  std::unique_ptr<IComponent> with_a = std::make_unique<ComponentWithA>();
  client_code(with_a.get());

  std::unique_ptr<IComponent> with_b = std::make_unique<ComponentWithB>();
  client_code(with_b.get());

  std::unique_ptr<IComponent> with_ab = std::make_unique<ComponentWithAandB>();
  client_code(with_ab.get());
}
}  // namespace problem

namespace decorator_pattern {
/// @class Concrete Component
class ConcreteComponent : public IComponent {
 public:
  std::string operation() const override { return "ConcreteComponent"; }
};

/// @class Base Decorator
class BaseDecorator : public IComponent {
 protected:
  std::unique_ptr<IComponent> component_;

 public:
  explicit BaseDecorator(std::unique_ptr<IComponent> component)
      : component_(std::move(component)) {}

  /// the Decorator delegates all work to the wrapped componentx
  std::string operation() const override { return component_->operation(); }
};

/// @class Concrete Decorator
class ConcreteDecoratorA : public BaseDecorator {
 public:
  explicit ConcreteDecoratorA(std::unique_ptr<IComponent> component)
      : BaseDecorator(std::move(component)) {}

  std::string operation() const override {
    return BaseDecorator::operation() + " + FeatureA";
  }
};

class ConcreteDecoratorB : public BaseDecorator {
 public:
  explicit ConcreteDecoratorB(std::unique_ptr<IComponent> component)
      : BaseDecorator(std::move(component)) {}

  std::string operation() const override {
    return BaseDecorator::operation() + " + FeatureB";
  }
};

class ConcreteDecoratorC : public BaseDecorator {
 public:
  explicit ConcreteDecoratorC(std::unique_ptr<IComponent> component)
      : BaseDecorator(std::move(component)) {}

  std::string operation() const override {
    return BaseDecorator::operation() + " + FeatureC";
  }
};

void run() {
  LOG("Decorator");
  std::unique_ptr<IComponent> simple = std::make_unique<ConcreteComponent>();
  client_code(simple.get());

  std::unique_ptr<IComponent> with_a =
      std::make_unique<ConcreteDecoratorA>(std::move(simple));
  std::unique_ptr<IComponent> with_ab =
      std::make_unique<ConcreteDecoratorB>(std::move(with_a));
  std::unique_ptr<IComponent> with_abc =
      std::make_unique<ConcreteDecoratorC>(std::move(with_ab));
  client_code(with_abc.get());
}
}  // namespace decorator_pattern

}  // namespace

class DecoratorExample : public IExample {
 public:
  std::string group() const override { return "dp/structural"; }
  std::string name() const override { return "Decorator"; }
  std::string description() const override {
    return "Decorator Pattern Example";
  }
  void execute() override {
    problem::run();
    decorator_pattern::run();
  }
};

REGISTER_EXAMPLE(DecoratorExample);