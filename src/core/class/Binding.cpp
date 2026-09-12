// -----------------------------------------------------------------------------
// Early binding vs late binding
//
//   Early (static) binding : the function is chosen at compile time from the
//                            static type (non-virtual members, overloads).
//   Late (dynamic) binding : a virtual function is chosen at run time from the
//                            dynamic type of the object, through a vtable.
//
// Pitfalls:
//   - A non-virtual function with the same name in a derived class HIDES the
//     base version; calls through a base reference still reach the base.
//   - Virtual calls inside a constructor do not reach the derived class: that
//     part of the object has not been constructed yet.
//
// Reference: https://www.learncpp.com/cpp-tutorial/early-binding-and-late-binding/
// -----------------------------------------------------------------------------

#include <memory>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace early_binding {

class Animal {
 public:
  void speak() const { LOG("Animal::speak"); }  // NOT virtual
};

class Dog : public Animal {
 public:
  void speak() const { LOG("Dog::speak"); }  // hides Animal::speak
};

void print(int value) {
  LOG_S("print(int)    called with " << value);
}

void print(double value) {
  LOG_S("print(double) called with " << value);
}

void run() {
  LOG_SECTION("Early binding: decided by the static type");

  const Dog dog;
  const Animal& as_animal = dog;
  LOG("dog.speak():");
  dog.speak();  // static type Dog
  LOG("as_animal.speak() where as_animal refers to the same Dog:");
  as_animal.speak();  // static type Animal -> Animal::speak

  LOG("Overload resolution also happens at compile time:");
  print(5);     // exact match: int
  print(0.5F);  // float -> double (promotion) is preferred over float -> int
}

}  // namespace early_binding

namespace late_binding {

class Animal {
 public:
  Animal() {
    LOG("Animal() calls describe():");
    describe();  // intentional: shows that dynamic binding is not used yet
  }
  virtual ~Animal() = default;
  Animal(const Animal&) = delete;
  Animal& operator=(const Animal&) = delete;

  virtual void speak() const { LOG("Animal::speak"); }
  virtual void describe() const { LOG("  Animal::describe"); }
};

class Dog final : public Animal {  // final: no class may derive from Dog
 public:
  void speak() const override { LOG("Dog::speak"); }
  void describe() const override { LOG("  Dog::describe"); }
};

void run() {
  LOG_SECTION("Late binding: decided by the dynamic type (virtual)");

  const std::unique_ptr<Animal> animal = std::make_unique<Dog>();
  LOG("  -> Animal::describe ran, because the Dog part did not exist yet");

  LOG("animal->speak() through a pointer to Animal:");
  animal->speak();
  LOG("animal->describe() after construction:");
  animal->describe();

  LOG_S("sizeof(early_binding::Animal) = " << sizeof(early_binding::Animal)
                                           << " (no virtual functions)");
  LOG_S("sizeof(late_binding::Animal)  = " << sizeof(Animal)
                                           << " (hidden vtable pointer)");
}

}  // namespace late_binding

}  // namespace

LAB_EXAMPLE("Binding",
            "early (static) vs late (virtual) binding, hiding vs overriding") {
  early_binding::run();
  late_binding::run();
}
