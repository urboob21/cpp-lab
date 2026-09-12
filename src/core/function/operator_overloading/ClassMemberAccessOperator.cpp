// -----------------------------------------------------------------------------
// Overloading operator-> and operator*
//
//   T& operator*()   dereference: returns the pointed-to object
//   T* operator->()  member access: returns a pointer (or another object with
//                    operator->, which is then applied again - "drill-down")
//
// These are how smart pointers and iterators look like pointers.
// Overloads usually come in const and non-const pairs.
//
// Reference: https://en.cppreference.com/w/cpp/language/operator_member_access
// -----------------------------------------------------------------------------

#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

struct Person {
  std::string name;
  int age;
  void greet() const { LOG_S("  Hi, I am " << name << " (" << age << ")"); }
};

/// A tiny owning pointer, similar in spirit to std::unique_ptr.
template <typename T>
class Owner {
 public:
  explicit Owner(T* pointer) : pointer_{pointer} {}
  ~Owner() { delete pointer_; }
  Owner(const Owner&) = delete;
  Owner& operator=(const Owner&) = delete;

  T& operator*() {
    LOG("  Owner::operator*");
    return *pointer_;
  }
  const T& operator*() const { return *pointer_; }

  T* operator->() {
    LOG("  Owner::operator->");
    return pointer_;  // the compiler then applies the built-in -> to this pointer
  }
  const T* operator->() const { return pointer_; }

 private:
  T* pointer_;
};

/// operator-> may return an object that itself has operator->; the compiler
/// keeps applying -> until it gets a raw pointer. Here that is used to lock
/// around every member call: operator-> returns a temporary "guard".
class Monitor {
 public:
  class Guard {
   public:
    explicit Guard(Person& person) : person_{person} { LOG("  [lock]"); }
    ~Guard() { LOG("  [unlock]"); }
    Guard(const Guard&) = delete;
    Guard& operator=(const Guard&) = delete;
    Person* operator->() { return &person_; }

   private:
    Person& person_;
  };

  explicit Monitor(Person person) : person_{std::move(person)} {}
  Guard operator->() {
    return Guard{person_};
  }  // chained: Guard::operator-> follows

 private:
  Person person_;
};

void run() {
  LOG_SECTION("A smart-pointer-like class");
  Owner<Person> owner{new Person{"Alice", 30}};
  owner->greet();     // owner.operator->()->greet()
  (*owner).age += 1;  // owner.operator*().age
  LOG_S("  age after (*owner).age += 1: " << owner->age);

  LOG_SECTION("Chained operator->: a monitor that locks per call");
  Monitor monitor{Person{"Bob", 40}};
  monitor
      ->greet();  // Monitor::operator-> -> Guard -> Guard::operator-> -> Person*
}

}  // namespace

LAB_EXAMPLE("ClassMemberAccessOperator",
            "operator-> and operator* for smart pointers and proxies") {
  run();
}
