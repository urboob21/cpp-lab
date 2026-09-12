// -----------------------------------------------------------------------------
// Shallow copy vs deep copy
//
//   Shallow copy : copies member values as they are. For a pointer member that
//                  means both objects point to the SAME memory.
//                  This is what the compiler-generated copy operations do.
//   Deep copy    : allocates new memory and copies what the pointer points to,
//                  so each object owns its own data.
//
// Pointer members that own memory need a user-written copy constructor and
// copy assignment (see RuleOfThreeFiveZero) - or better, a smart pointer or
// container that already copies correctly.
//
// Reference: https://www.learncpp.com/cpp-tutorial/shallow-vs-deep-copying/
// -----------------------------------------------------------------------------

#include <memory>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace shallow {

class Model {
 public:
  explicit Model(int value) : value_{new int{value}} {}

  // No user-defined copy operations: copying duplicates the pointer only.
  // A destructor with `delete value_;` would free the shared int twice, so it
  // is intentionally left out and run() frees the memory once by hand.

  void set(int value) { *value_ = value; }
  int get() const { return *value_; }
  const int* address() const { return value_; }
  void releaseForDemo() {
    delete value_;
    value_ = nullptr;
  }

 private:
  int* value_;
};

void run() {
  LOG_SECTION("Shallow copy");
  Model original{3};
  Model copy = original;  // compiler-generated copy: pointer copied
  copy.set(30);

  LOG_S("original.get() = " << original.get()
                            << "  <- changed through the copy");
  LOG_S("copy.get()     = " << copy.get());
  LOG_S("same address?    " << std::boolalpha
                            << (original.address() == copy.address()));

  original.releaseForDemo();  // `copy` now holds a dangling pointer
}

}  // namespace shallow

namespace deep {

class Model {
 public:
  explicit Model(int value) : value_{new int{value}} {}
  ~Model() { delete value_; }

  // Copy constructor: allocate our own int and copy the pointed-to value.
  // No self-copy check is needed: an object cannot be constructed from itself.
  Model(const Model& other) : value_{new int{*other.value_}} {}

  // Copy assignment: `a = a` is possible, so guard against self-assignment.
  Model& operator=(const Model& other) {
    if (this != &other) {
      *value_ =
          *other.value_;  // both objects own an int already: copy the value
    }
    return *this;
  }

  void set(int value) { *value_ = value; }
  int get() const { return *value_; }
  const int* address() const { return value_; }

 private:
  int* value_;
};

void run() {
  LOG_SECTION("Deep copy");
  const Model original{3};
  Model copy = original;
  copy.set(30);

  LOG_S("original.get() = " << original.get() << "   <- unchanged");
  LOG_S("copy.get()     = " << copy.get());
  LOG_S("same address?    " << std::boolalpha
                            << (original.address() == copy.address()));

  Model assigned{0};
  assigned = original;
  LOG_S("assigned.get() = " << assigned.get()
                            << " after `assigned = original`");
}

}  // namespace deep

namespace modern {

/// Rule of Zero alternative: std::unique_ptr cannot be copied by accident, and
/// the explicit copy constructor states what a copy means.
class Model {
 public:
  explicit Model(int value) : value_{std::make_unique<int>(value)} {}
  Model(const Model& other) : value_{std::make_unique<int>(*other.value_)} {}
  Model& operator=(const Model& other) {
    *value_ = *other.value_;
    return *this;
  }
  Model(Model&&) noexcept = default;
  Model& operator=(Model&&) noexcept = default;
  ~Model() = default;

  void set(int value) { *value_ = value; }
  int get() const { return *value_; }

 private:
  std::unique_ptr<int> value_;  // frees itself, never double-deletes
};

void run() {
  LOG_SECTION("Deep copy with std::unique_ptr");
  const Model original{3};
  Model copy = original;
  copy.set(30);
  LOG_S("original.get() = " << original.get()
                            << ", copy.get() = " << copy.get());
}

}  // namespace modern

}  // namespace

LAB_EXAMPLE("ShallowDeepCopying",
            "why copying a pointer member is not copying the data") {
  shallow::run();
  deep::run();
  modern::run();
}
