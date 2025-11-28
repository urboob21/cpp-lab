// Decorator is a structural design pattern that lets you attach new behaviors
// to objects by placing these objects inside special wrapper objects that
// contain the behaviors. Appicability:
// (*)   when you need to be able to assign extra behaviors to objects at
// runtime without breaking the code that uses these objects.
// (**)  when it’s awkward or not possible to extend an object’s behavior using
// inheritance. UML: docs/uml/patterns_structural_decorator.drawio.svg

#include <algorithm>
#include <iostream>
#include <list>
#include <string>

namespace {
namespace Problem {

class IComponent {
 public:
  virtual ~IComponent() {}
  virtual std::string operation() const = 0;
};

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

// [P1]
// If you have 3 features , e.g FeatureC -> many combinations
// If you have 5 features -> 32 subclasses

namespace Client {
void clientCode(const IComponent& comp) {
  std::cout << comp.operation() << "\n";
}
}  // namespace Client

void run() {
  std::cout << "\n\nProblem\n";
  IComponent* simple = new ConcreteComponent;
  Client::clientCode(*simple);

  IComponent* withA = new ComponentWithA;
  Client::clientCode(*withA);

  IComponent* withB = new ComponentWithB;
  Client::clientCode(*withB);

  IComponent* withAB = new ComponentWithAandB;
  Client::clientCode(*withAB);

  delete simple;
  delete withA;
  delete withB;
  delete withAB;
}

}  // namespace Problem

namespace DecoratorPattern {
class IComponent {
 public:
  virtual ~IComponent() {}
  virtual std::string operation() const = 0;
};

/**
 * Concrete Component
 * - is a class of objects being wrapped.
 * - defines the basic behavior, which can be altered by decorators.
 */
class ConcreteComponent : public IComponent {
 public:
  std::string operation() const override { return "ConcreteComponent"; }
};

/**
 * The base Decorator class follows the same interface as the other components.
 * - has a field for referencing a wrapped object.
 * - the field’s type should be declared as the component interface so it can
 * contain both concrete components and decorators.
 * - the base decorator delegates all operations to the wrapped object.
 */
class BaseDecorator : public IComponent {
 protected:
  IComponent* m_component;

 public:
  explicit BaseDecorator(IComponent* component) : m_component{component} {}

  /**
   * The Decorator delegates all work to the wrapped component.
   */
  std::string operation() const override { return m_component->operation(); }
};

/**
 * Concrete Decorators :
 * - call the wrapped object and alter its result in some way.
 * - define extra behaviors that can be added to components dynamically.
 * - override methods of the base decorator and execute their behavior either
 * before or after calling the parent method.
 */
class ConcreteDecoratorA : public BaseDecorator {
 public:
  explicit ConcreteDecoratorA(IComponent* component)
      : BaseDecorator{component} {}

  std::string operation() const override {
    return BaseDecorator::operation() + " + FeatureA";
  }
};

class ConcreteDecoratorB : public BaseDecorator {
 public:
  explicit ConcreteDecoratorB(IComponent* component)
      : BaseDecorator{component} {}

  std::string operation() const override {
    return BaseDecorator::operation() + " + FeatureB";
  }
};

class ConcreteDecoratorC : public BaseDecorator {
 public:
  explicit ConcreteDecoratorC(IComponent* component)
      : BaseDecorator{component} {}

  std::string operation() const override {
    return BaseDecorator::operation() + " + FeatureC";
  }
};

namespace Client {
void clientCode(const IComponent& comp) {
  std::cout << comp.operation() << "\n";
}
}  // namespace Client

void run() {
  std::cout << "\n\nDecorator\n";
  IComponent* simple = new ConcreteComponent;
  Client::clientCode(*simple);

  IComponent* withA = new ConcreteDecoratorA(simple);
  Client::clientCode(*withA);

  IComponent* withB = new ConcreteDecoratorB(simple);
  Client::clientCode(*withB);

  IComponent* withAB = new ConcreteDecoratorB(withA);
  Client::clientCode(*withAB);

  IComponent* withABC = new ConcreteDecoratorC(withAB);
  Client::clientCode(*withABC);

  delete simple;
  delete withA;
  delete withB;
  delete withAB;
  delete withABC;
}
}  // namespace DecoratorPattern

}  // namespace

struct DecoratorAutoRuner {
  DecoratorAutoRuner() {
    std::cout << "\n--- Decorator Pattern Example ---\n";
    Problem::run();
    DecoratorPattern::run();
  }
};

static DecoratorAutoRuner instance;