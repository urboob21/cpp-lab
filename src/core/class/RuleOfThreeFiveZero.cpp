// -----------------------------------------------------------------------------
// Rule of Three / Five / Zero
//
// A class that manages a resource by hand (new/delete, file handles, ...)
// must decide what copying, moving and destroying mean:
//
//   Rule of Three : destructor + copy constructor + copy assignment
//   Rule of Five  : ... + move constructor + move assignment (C++11)
//   Rule of Zero  : use members that already manage themselves (std::string,
//                   std::vector, std::unique_ptr) and write none of them.
//
// Prefer the Rule of Zero. Write the five only for low-level resource wrappers.
//
// Reference: https://en.cppreference.com/w/cpp/language/rule_of_three
// -----------------------------------------------------------------------------

#include <cstring>
#include <memory>
#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

char* duplicate(const char* text) {
  const std::size_t size = std::strlen(text) + 1;
  auto* copy = new char[size];
  std::memcpy(copy, text, size);
  return copy;
}

namespace problem {

/// Owns a heap buffer but relies on the compiler-generated copy operations.
class Text {
 public:
  explicit Text(const char* text) : data_{duplicate(text)} {}

  // The destructor SHOULD release the buffer with `delete[] data_;`. Because
  // the implicit copy constructor copies only the pointer (a shallow copy),
  // two objects would then delete the same buffer: a double free. To keep the
  // lab running, the destructor does nothing and run() frees the buffer once.
  ~Text() = default;

  const char* c_str() const { return data_; }
  void setFirstChar(char c) { data_[0] = c; }
  void releaseForDemo() {
    delete[] data_;
    data_ = nullptr;
  }

 private:
  char* data_;
};

void run() {
  LOG_SECTION("Problem: owning raw pointer + implicit copy");
  Text first{"hello"};
  Text second = first;  // copies the POINTER, not the characters

  second.setFirstChar('J');
  LOG_S("first  = " << first.c_str() << "   <- changed through `second`!");
  LOG_S("second = " << second.c_str());
  LOG_S("both point to " << static_cast<const void*>(first.c_str()));

  first
      .releaseForDemo();  // manual cleanup; `second` now dangles - never use it
}

}  // namespace problem

namespace rule_of_three {

class Text {
 public:
  explicit Text(const char* text) : data_{duplicate(text)} {}

  // 1. Destructor: releases the resource.
  ~Text() { delete[] data_; }

  // 2. Copy constructor: deep copy.
  Text(const Text& other) : data_{duplicate(other.data_)} {
    LOG_S("  Text(const Text&)   deep copy of \"" << other.data_ << "\"");
  }

  // 3. Copy assignment, written with the copy-and-swap idiom: it is safe for
  //    self-assignment and leaves *this untouched if the copy throws.
  Text& operator=(const Text& other) {
    LOG("  operator=(const Text&)");
    Text copy(other);
    std::swap(data_, copy.data_);
    return *this;  // `copy` now holds the old buffer and deletes it
  }

  const char* c_str() const { return data_; }
  void setFirstChar(char c) { data_[0] = c; }

 private:
  char* data_;
};

void run() {
  LOG_SECTION("Rule of Three");
  Text first{"hello"};
  Text second = first;
  second.setFirstChar('J');
  LOG_S("first = " << first.c_str() << ", second = " << second.c_str()
                   << "   <- independent copies");

  LOG("Text moved = std::move(first);   <- no move constructor: falls back to "
      "COPY");
  const Text moved = std::move(first);
  LOG_S("first still holds \"" << first.c_str()
                               << "\" (the move was really a copy)");
  LOG_S("moved = " << moved.c_str());
}

}  // namespace rule_of_three

namespace rule_of_five {

class Text {
 public:
  explicit Text(const char* text) : data_{duplicate(text)} {}
  ~Text() { delete[] data_; }

  Text(const Text& other) : data_{duplicate(other.data_)} {
    LOG("  Text(const Text&)");
  }

  Text& operator=(const Text& other) {
    LOG("  operator=(const Text&)");
    Text copy(other);
    std::swap(data_, copy.data_);
    return *this;
  }

  // 4. Move constructor: steal the pointer, leave the source empty.
  //    noexcept matters: std::vector only moves elements when it cannot throw.
  Text(Text&& other) noexcept : data_{std::exchange(other.data_, nullptr)} {
    LOG("  Text(Text&&)        steals the buffer");
  }

  // 5. Move assignment.
  Text& operator=(Text&& other) noexcept {
    LOG("  operator=(Text&&)");
    std::swap(data_, other.data_);  // `other` deletes our old buffer later
    return *this;
  }

  const char* c_str() const { return data_ != nullptr ? data_ : "(null)"; }

 private:
  char* data_;
};

void run() {
  LOG_SECTION("Rule of Five");
  Text first{"hello"};
  LOG("Text moved = std::move(first);");
  const Text moved = std::move(first);
  LOG_S("first = " << first.c_str() << ", moved = " << moved.c_str());

  Text target{"old"};
  LOG("target = Text{\"temporary\"};");
  target = Text{"temporary"};
  LOG_S("target = " << target.c_str());
}

}  // namespace rule_of_five

namespace rule_of_zero {

/// std::string manages the memory: the compiler-generated destructor, copy
/// and move operations are all correct. Nothing to write!
class Text {
 public:
  explicit Text(std::string text) : data_{std::move(text)} {}
  const std::string& str() const { return data_; }
  void setFirstChar(char c) { data_.at(0) = c; }

 private:
  std::string data_;
};

/// Polymorphic base classes are the exception: they need a virtual destructor,
/// which suppresses the implicit move operations, so all five are defaulted.
class Shape {
 public:
  Shape() = default;
  virtual ~Shape() = default;
  Shape(const Shape&) = default;
  Shape& operator=(const Shape&) = default;
  Shape(Shape&&) = default;
  Shape& operator=(Shape&&) = default;

  virtual double area() const = 0;
};

class Square final : public Shape {  // Rule of Zero again
 public:
  explicit Square(double side) : side_{side} {}
  double area() const override { return side_ * side_; }

 private:
  double side_;
};

void run() {
  LOG_SECTION("Rule of Zero");
  Text first{"hello"};
  Text second = first;
  second.setFirstChar('J');
  LOG_S("first = " << first.str() << ", second = " << second.str());

  Text moved = std::move(first);
  LOG_S("moved = " << moved.str() << ", first after move = \"" << first.str()
                   << "\" (valid but unspecified, here empty)");

  const std::unique_ptr<Shape> shape = std::make_unique<Square>(3.0);
  LOG_S("Square(3).area() = " << shape->area());
}

}  // namespace rule_of_zero

}  // namespace

LAB_EXAMPLE("RuleOfThreeFiveZero",
            "which special member functions to write, and when to write none") {
  problem::run();
  rule_of_three::run();
  rule_of_five::run();
  rule_of_zero::run();
}
