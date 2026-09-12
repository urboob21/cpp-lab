// -----------------------------------------------------------------------------
// Destructors
//
//   - Run automatically when an object's lifetime ends (end of scope, delete,
//     destruction of the owning object, stack unwinding after a throw).
//   - Destruction order is the REVERSE of construction order:
//     derived body -> members (reverse declaration order) -> base classes.
//   - A class meant to be deleted through a base pointer needs a virtual
//     destructor. Without it, `delete base_pointer` is undefined behavior.
//   - RAII: acquire a resource in the constructor and release it in the
//     destructor, so it can never be forgotten - not even when an exception
//     is thrown.
//
// Reference: https://en.cppreference.com/w/cpp/language/destructor
// -----------------------------------------------------------------------------

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Member {
 public:
  explicit Member(std::string name) : name_{std::move(name)} {
    LOG_S("  Member(" << name_ << ")");
  }
  ~Member() { LOG_S("  ~Member(" << name_ << ")"); }
  Member(const Member&) = delete;
  Member& operator=(const Member&) = delete;

 private:
  std::string name_;
};

namespace order {

class Base {
 public:
  Base() { LOG("  Base()"); }
  ~Base() { LOG("  ~Base()"); }
  Base(const Base&) = delete;
  Base& operator=(const Base&) = delete;
};

class Derived : public Base {
 public:
  Derived() { LOG("  Derived()"); }
  ~Derived() { LOG("  ~Derived()"); }
  Derived(const Derived&) = delete;
  Derived& operator=(const Derived&) = delete;

 private:
  Member first_{"first"};
  Member second_{"second"};
};

void run() {
  LOG_SECTION("Construction and destruction order");
  {
    const Derived derived;
    LOG("-- leaving the scope --");
  }
}

}  // namespace order

namespace virtual_destructor {

class Base {
 public:
  Base() = default;
  virtual ~Base() { LOG("  ~Base()"); }
  Base(const Base&) = delete;
  Base& operator=(const Base&) = delete;
};

class Derived : public Base {
 public:
  ~Derived() override { LOG("  ~Derived()  <- runs because ~Base is virtual"); }

 private:
  Member resource_{"resource owned by Derived"};
};

void run() {
  LOG_SECTION("Virtual destructor");
  LOG("std::unique_ptr<Base> ptr = std::make_unique<Derived>(); ptr.reset();");
  std::unique_ptr<Base> ptr = std::make_unique<Derived>();
  ptr.reset();
  // If ~Base() were not virtual, deleting a Derived through a Base pointer
  // would be undefined behavior: typically ~Derived and its members never run
  // and their resources leak.
}

}  // namespace virtual_destructor

namespace raii {

void run() {
  LOG_SECTION("RAII and stack unwinding");
  try {
    const Member lock{"lock"};
    const Member connection{"connection"};
    LOG("throwing while both resources are held...");
    throw std::runtime_error("something failed");
  } catch (const std::exception& e) {
    LOG_S("caught '" << e.what() << "' - both destructors already ran");
  }
}

}  // namespace raii

}  // namespace

LAB_EXAMPLE(
    "Destructor",
    "destruction order, virtual destructors, RAII and stack unwinding") {
  order::run();
  virtual_destructor::run();
  raii::run();
}
