// -----------------------------------------------------------------------------
// std::function, std::bind and std::invoke (<functional>)
//
//   std::function<int(int, int)> f;   holds ANY callable with that signature:
//                                     free functions, lambdas (with captures),
//                                     function objects, bound member functions
//
//   - Calling an empty std::function throws std::bad_function_call.
//   - It has a small run-time cost (type erasure, possible allocation); inside
//     hot code or generic code prefer templates / auto parameters.
//   - std::bind is mostly replaced by lambdas, which are easier to read.
//   - std::invoke calls anything callable, including member pointers.
//
// Reference: https://en.cppreference.com/w/cpp/utility/functional/function
// -----------------------------------------------------------------------------

#include <functional>
#include <map>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

int add(int a, int b) {
  return a + b;
}

/// A function object (functor): a class with operator().
struct Multiply {
  int operator()(int a, int b) const { return a * b; }
};

class Calculator {
 public:
  int power(int base, int exponent) const {
    int result = 1;
    for (int i = 0; i < exponent; ++i) {
      result *= base;
    }
    return result;
  }
};

void apply(int a, int b, const std::function<int(int, int)>& operation,
           const char* name) {
  if (!operation) {  // std::function can be empty
    LOG_S(name << ": no function provided");
    return;
  }
  LOG_S(name << "(" << a << ", " << b << ") = " << operation(a, b));
}

void holdingCallables() {
  LOG_SECTION("One type for many kinds of callables");
  int offset = 100;
  const Calculator calculator;

  apply(8, 2, add, "free function");
  apply(8, 2, Multiply{}, "function object");
  apply(8, 2, [](int a, int b) { return a % b; }, "lambda");
  apply(
      8, 2, [offset](int a, int b) { return a - b + offset; },
      "capturing lambda");
  apply(
      8, 2, [&calculator](int a, int b) { return calculator.power(a, b); },
      "member function via lambda");
  apply(8, 2, nullptr, "empty");

  const std::function<int(int, int)> empty;
  try {
    empty(1, 2);
  } catch (const std::bad_function_call& e) {
    LOG_S("calling an empty std::function throws: " << e.what());
  }
}

void bindAndInvoke() {
  LOG_SECTION("std::bind vs lambda, std::invoke");
  using std::placeholders::_1;

  const std::function<int(int)> add_ten_bind = std::bind(add, _1, 10);
  const std::function<int(int)> add_ten_lambda = [](int value) {
    return add(value, 10);
  };
  LOG_S("std::bind(add, _1, 10)(5) = " << add_ten_bind(5) << ", lambda(5) = "
                                       << add_ten_lambda(5));

  const Calculator calculator;
  LOG_S("std::invoke(&Calculator::power, calculator, 2, 10) = " << std::invoke(
            &Calculator::power, calculator, 2, 10));
}

void callbackRegistry() {
  LOG_SECTION("Use case: a command registry");
  std::map<std::string, std::function<void()>> commands;
  int counter = 0;
  commands["increment"] = [&counter] {
    ++counter;
  };
  commands["reset"] = [&counter] {
    counter = 0;
  };
  commands["show"] = [&counter] {
    LOG_S("  counter = " << counter);
  };

  for (const std::string name :
       {"increment", "increment", "show", "reset", "show", "unknown"}) {
    if (const auto it = commands.find(name); it != commands.end()) {
      it->second();
    } else {
      LOG_S("  unknown command '" << name << "'");
    }
  }
}

}  // namespace

LAB_EXAMPLE(
    "Functional",
    "std::function, function objects, std::bind, std::invoke, callbacks") {
  holdingCallables();
  bindAndInvoke();
  callbackRegistry();
}
