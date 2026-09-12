// -----------------------------------------------------------------------------
// Overloading operator[] (and operator() for 2D access)
//
//   T&       operator[](std::size_t index);        read/write for non-const objects
//   const T& operator[](std::size_t index) const;  read-only for const objects
//
//   - Provide both overloads; the compiler picks one by the constness of the
//     object.
//   - Like std::vector, operator[] is usually unchecked (fast) and a separate
//     at() checks the bounds and throws.
//   - Before C++23 operator[] takes exactly one argument, so matrices often
//     use operator()(row, column) instead.
//
// Reference: https://en.cppreference.com/w/cpp/language/operators#Array_subscript_operator
// -----------------------------------------------------------------------------

#include <array>
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class IntList {
 public:
  int& operator[](std::size_t index) {
    assert(index < values_.size() &&
           "index out of range");  // checked in Debug only
    return values_[index];
  }

  const int& operator[](std::size_t index) const {
    assert(index < values_.size() && "index out of range");
    return values_[index];
  }

  int& at(std::size_t index) {
    if (index >= values_.size()) {
      throw std::out_of_range("IntList::at: index " + std::to_string(index));
    }
    return values_[index];
  }

  std::size_t size() const { return values_.size(); }

 private:
  std::array<int, 10> values_{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
};

class Matrix {
 public:
  double& operator()(std::size_t row, std::size_t column) {
    return cells_[row * kColumns + column];
  }
  double operator()(std::size_t row, std::size_t column) const {
    return cells_[row * kColumns + column];
  }

 private:
  static constexpr std::size_t kColumns = 3;
  std::array<double, 2 * kColumns> cells_{};
};

void run() {
  LOG_SECTION("const and non-const operator[]");
  IntList list;
  list[2] = 30;  // non-const overload returns int&
  LOG_S("list[2] = " << list[2]);

  const IntList read_only;
  // read_only[2] = 3;  // error: const overload returns const int&
  LOG_S("read_only[2] = " << read_only[2]);

  try {
    list.at(10) = 1;
  } catch (const std::out_of_range& e) {
    LOG_S("list.at(10) throws: " << e.what());
  }

  LOG_SECTION("operator() for two indices");
  Matrix matrix;
  matrix(1, 2) = 4.5;
  const Matrix& view = matrix;
  LOG_S("matrix(1, 2) = " << view(1, 2) << ", matrix(0, 0) = " << view(0, 0));
}

}  // namespace

LAB_EXAMPLE(
    "SubscriptOperator",
    "operator[] const/non-const overloads, bounds-checked at(), operator()") {
  run();
}
