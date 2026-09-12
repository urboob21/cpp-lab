// -----------------------------------------------------------------------------
// C-style arrays
//
//   - `int values[5]` has a size fixed at compile time; elements are contiguous.
//   - Array-to-pointer decay: in most expressions (and when passed to a
//     function) an array turns into a pointer to its first element and the
//     size is lost.
//   - No bounds checking: values[5] on a 5-element array is undefined behavior.
//   - std::size(values) (C++17) gives the element count while no decay happened.
//   - In C++ prefer std::array (fixed) or std::vector (dynamic); use std::span
//     (C++20) to pass "pointer + size" safely.
//
// Reference: https://en.cppreference.com/w/cpp/language/array
// -----------------------------------------------------------------------------

#include <iterator>
#include <span>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void initialization() {
  LOG_SECTION("Initialization");
  const int numbers[5] = {1, 2, 3};  // remaining elements become 0
  const int deduced[] = {4, 5, 6};   // size deduced: 3
  const char text[] = "hi";          // 3 chars: 'h', 'i', '\0'

  std::string listed;
  for (const int value : numbers) {
    listed += std::to_string(value) + " ";
  }
  LOG_S("int numbers[5] = {1, 2, 3}  -> " << listed);
  LOG_S("std::size(deduced) = " << std::size(deduced));
  LOG_S("sizeof(\"hi\") = " << sizeof(text)
                            << " (includes the terminating '\\0')");
}

// Looks like it takes an array, but the parameter really is `const int*`.
// (GCC even warns when you write sizeof(values) here: -Wsizeof-array-argument.)
void decayed(const int values[]) {
  LOG_S("inside the function the parameter type has sizeof "
        << sizeof(decltype(values))
        << " -> the size of a POINTER, first element " << values[0]);
}

// std::span keeps pointer and size together (C++20).
int sum(std::span<const int> values) {
  int total = 0;
  for (const int value : values) {
    total += value;
  }
  return total;
}

void decay() {
  LOG_SECTION("Array-to-pointer decay");
  const int values[5] = {10, 20, 30, 40, 50};
  LOG_S("sizeof(values) = " << sizeof(values) << ", element count = "
                            << sizeof(values) / sizeof(values[0]));
  decayed(values);

  const int* first = values;  // decay: pointer to values[0]
  LOG_S("*(first + 2) = " << *(first + 2) << " (same as values[2])");
  LOG_S("sum(std::span) = " << sum(values) << " (span knows the size is "
                            << std::span<const int>(values).size() << ")");
}

void multidimensional() {
  LOG_SECTION("Multidimensional array");
  const double matrix[2][3] = {{1.1, 2.2, 3.3}, {4.4, 5.5, 6.6}};
  for (const auto& row : matrix) {  // each row is a `const double[3]`
    std::string line;
    for (const double cell : row) {
      line += std::to_string(cell).substr(0, 3) + " ";
    }
    LOG(line);
  }
  LOG_S("sizeof(matrix) = " << sizeof(matrix) << " = 2 * 3 * sizeof(double)");
}

}  // namespace

LAB_EXAMPLE(
    "Array",
    "C arrays: initialization, decay to pointers, std::size, std::span") {
  initialization();
  decay();
  multidimensional();
}
