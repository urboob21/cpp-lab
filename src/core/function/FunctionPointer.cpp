// -----------------------------------------------------------------------------
// Function pointers
//
//   int (*operation)(int, int) = &add;   pointer to "function taking two ints
//                                        and returning int"
//   using Operation = int (*)(int, int); an alias makes the syntax readable
//
//   - A function name converts to a pointer automatically (&add == add).
//   - Call through the pointer with operation(1, 2) or (*operation)(1, 2).
//   - Function pointers cannot hold lambdas WITH captures; use std::function
//     or templates for that (see Functional.cpp).
//   - Pointers to member functions need an object: (object.*member)().
//
// Reference: https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_functions
// -----------------------------------------------------------------------------

#include <algorithm>
#include <array>
#include <cstdlib>
#include <string>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

std::string join(const std::vector<int>& values) {
  std::string text;
  for (const int value : values) {
    text += std::to_string(value) + " ";
  }
  return text;
}

bool ascending(int a, int b) {
  return a < b;
}

bool descending(int a, int b) {
  return a > b;
}

bool byAbsoluteValue(int a, int b) {
  return std::abs(a) < std::abs(b);
}

using Comparator = bool (*)(int, int);

void sortWith(std::vector<int> values, Comparator compare, const char* label) {
  std::sort(values.begin(), values.end(), compare);
  LOG_S(label << join(values));
}

void comparators() {
  LOG_SECTION("Passing functions to std::sort");
  const std::vector<int> values{-1, -6, 4, 2, 0, 6, 3, 9, -5};
  LOG_S("input             : " << join(values));

  sortWith(values, ascending, "ascending         : ");
  sortWith(values, &descending, "descending        : ");  // & is optional
  sortWith(values, byAbsoluteValue, "by absolute value : ");

  Comparator chosen = descending;
  LOG_S("chosen(1, 2) = " << std::boolalpha << chosen(1, 2)
                          << ", (*chosen)(2, 1) = " << (*chosen)(2, 1));
}

int add(int a, int b) {
  return a + b;
}
int subtract(int a, int b) {
  return a - b;
}
int multiply(int a, int b) {
  return a * b;
}

void dispatchTable() {
  LOG_SECTION("A dispatch table: array of function pointers");
  struct Entry {
    char symbol;
    int (*function)(int, int);
  };
  constexpr std::array<Entry, 3> kOperations{
      {{'+', add}, {'-', subtract}, {'*', multiply}}};

  for (const Entry& entry : kOperations) {
    LOG_S("8 " << entry.symbol << " 2 = " << entry.function(8, 2));
  }

  // A capture-less lambda converts to a function pointer too.
  int (*modulo)(int, int) = [](int a, int b) {
    return a % b;
  };
  LOG_S("8 % 3 = " << modulo(8, 3)
                   << " (capture-less lambda as a function pointer)");
}

class Counter {
 public:
  void increment() { ++value_; }
  void reset() { value_ = 0; }
  int value() const { return value_; }

 private:
  int value_{0};
};

void memberFunctionPointers() {
  LOG_SECTION("Pointers to member functions");
  void (Counter::*action)() = &Counter::increment;  // & is required here
  Counter counter;
  (counter.*action)();
  (counter.*action)();
  LOG_S("after calling increment twice: " << counter.value());

  action = &Counter::reset;
  Counter* pointer = &counter;
  (pointer->*action)();
  LOG_S("after reset through a pointer: " << counter.value());
}

}  // namespace

LAB_EXAMPLE("FunctionPointer",
            "function pointers, comparators, dispatch tables, member function "
            "pointers") {
  comparators();
  dispatchTable();
  memberFunctionPointers();
}
