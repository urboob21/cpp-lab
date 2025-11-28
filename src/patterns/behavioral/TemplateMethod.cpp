// cppcheck-suppress-file [functionStatic]

//  is a behavioral design pattern that defines the skeleton of an algorithm in
//  the `superclass`
// but lets `subclasses` override specific steps of the algorithm without
// changing its structure. Appicability:
// (*)    when you want to let clients extend only particular steps of an
// algorithm, but not the whole algorithm or its structure.
// (**)   when you have several classes that contain almost identical algorithms
// with some minor differences.
//        As a result, you might need to modify all classes when the algorithm
//        changes.
// (**)  when a behavior makes sense only in some classes of a class hierarchy,
// but not in others. UML:
// docs/uml/patterns_behavioral_templatemethod.drawio.svg

#include <iostream>

namespace {
namespace TemplateMethod {
class AbstractClass {
 public:
  virtual ~AbstractClass() = default;
  // Template Method (non-virtual => cannot be overridden)
  // Defines the algorithm's skeleton and ensures subclasses cannot change the
  // flow.
  void templateMethod() const {
    baseOperation1();
    abstractMethod1();
    hookOperation1();

    baseOperation2();
    abstractMethod2();
    hookOperation2();
  }

 protected:
  // 1. Base operations: These have full implementations and cannot be
  // overridden.
  void baseOperation1() const {
    // Common logic step 1
    std::cout << "[AbstractClass]\t Executed base operation - 1\n";
  }

  void baseOperation2() const {
    // Common logic step 2
    std::cout << "[AbstractClass]\t Executed base operation - 2\n";
  }

  // 2. Hook methods: Subclasses may override them to extend behavior, but
  // overriding is optional.
  virtual void hookOperation1() const {}
  virtual void hookOperation2() const {}

  // 3. Abstract methods: Subclasses MUST provide implementations.
  virtual void abstractMethod1() const = 0;
  virtual void abstractMethod2() const = 0;
};

class ConcreteClass1 : public AbstractClass {
 public:
  void abstractMethod1() const override {
    std::cout << "[ConcreteClass1]\t Implemented Operation - 2\n";
  }

  void abstractMethod2() const override {
    std::cout << "[ConcreteClass1]\t Implemented Operation - 2\n";
  }

  void hookOperation1() const override {
    std::cout << "[ConcreteClass1]\t Overridden Hook - 1\n";
  }
};

class ConcreteClass2 : public AbstractClass {
 public:
  void abstractMethod1() const override {
    std::cout << "[ConcreteClass2]\t Implemented Operation - 2\n";
  }

  void abstractMethod2() const override {
    std::cout << "[ConcreteClass2]\t Implemented Operation - 2\n";
  }
};

namespace Client {
void clientCode(const AbstractClass* clazz) {
  clazz->templateMethod();
}
}  // namespace Client
void run() {
  std::cout << "\t[ConcreteClass1] Executed templateMethod\n";
  AbstractClass* clazz1 = new ConcreteClass1();
  Client::clientCode(clazz1);

  std::cout << "\t[ConcreteClass1] Executed templateMethod\n";
  AbstractClass* clazz2 = new ConcreteClass2();
  Client::clientCode(clazz2);

  delete clazz1;
  delete clazz2;
}
}  // namespace TemplateMethod
}  // namespace

struct TemplateMethodAutoRunner {
  TemplateMethodAutoRunner() {
    std::cout << "\n--- TemplateMethod Pattern Example ---\n";
    TemplateMethod::run();
  }
};

static TemplateMethodAutoRunner instance;