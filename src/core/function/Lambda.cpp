// -----------------------------------------------------------------------------
// Lambda expressions
//
//   [captures](parameters) mutable -> return_type { body }
//
//   Captures:  [x]  copy of x         [&x]    reference to x
//              [=]  copy everything   [&]     reference everything used
//              [y = expr] init capture (C++14), e.g. [p = std::move(ptr)]
//              [this] the current object
//
//   - The compiler turns a lambda into an unnamed class with operator().
//   - Captured copies are const inside the body unless the lambda is `mutable`.
//   - `auto` parameters make a generic lambda (a template operator()).
//   - Pitfall: capturing by reference and using the lambda after the captured
//     variables are gone gives a dangling reference.
//
// Reference: https://en.cppreference.com/w/cpp/language/lambda
// -----------------------------------------------------------------------------

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <memory>
#include <string>
#include <utility>
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

void withAlgorithms() {
  LOG_SECTION("Lambdas with standard algorithms");
  std::vector<int> values{-1, -6, 4, 2, 0, 6, 3, 9, -5};
  LOG_S("input             : " << join(values));

  std::sort(values.begin(), values.end(), [](int a, int b) { return a > b; });
  LOG_S("descending        : " << join(values));

  std::sort(values.begin(), values.end(),
            [](int a, int b) { return std::abs(a) < std::abs(b); });
  LOG_S("by absolute value : " << join(values));

  const auto negatives =
      std::count_if(values.begin(), values.end(), [](int v) { return v < 0; });
  LOG_S("negative values   : " << negatives);
}

void captures() {
  LOG_SECTION("Capture by value, by reference, mutable, init capture");
  int counter = 0;

  auto by_value = [counter]() {
    return counter + 1;
  };  // copies counter NOW
  auto by_reference = [&counter]() {
    ++counter;
  };  // refers to counter
  counter = 10;
  by_reference();
  LOG_S("by_value() = " << by_value()
                        << " (saw counter = 0), counter = " << counter);

  auto generator = [next = 0]() mutable {
    return next++;
  };  // init capture + mutable
  const int first = generator();
  const int second = generator();
  const int third = generator();
  LOG_S("mutable generator: " << first << ' ' << second << ' ' << third);

  auto owned = std::make_unique<std::string>("moved into the lambda");
  auto uses_owned = [text = std::move(owned)]() {
    return *text;
  };  // move-only capture
  LOG_S("init capture with std::move: " << uses_owned());
}

class Button {
 public:
  explicit Button(std::string label) : label_{std::move(label)} {}

  std::function<void()> onClick() const {
    return [this] {
      LOG_S("clicked '" << label_ << "' (captured this)");
    };
  }

 private:
  std::string label_;
};

void thisCapture() {
  LOG_SECTION("Capturing this");
  Button button{"OK"};
  const std::function<void()> handler = button.onClick();
  handler();  // fine: button is still alive
  // Storing `handler` beyond the lifetime of `button` would dangle.
}

std::function<int(int)> makeMultiplier(int factor) {
  return [factor](int value) {
    return value * factor;
  };  // copy: safe to return
  // [&factor] would refer to a parameter that dies when this function returns!
}

void genericAndClosures() {
  LOG_SECTION("Generic lambdas, closures, immediately invoked lambdas");
  auto print_twice = [](const auto& value) {
    LOG_S("  " << value << ' ' << value);
  };
  print_twice(42);
  print_twice(std::string("hi"));

  const auto triple = makeMultiplier(3);
  LOG_S("makeMultiplier(3)(7) = " << triple(7));

  // Immediately invoked lambda: initialize a const variable with complex logic.
  const std::string level = [](int errors) {
    if (errors == 0) {
      return "ok";
    }
    return errors < 5 ? "warning" : "error";
  }(3);
  LOG_S("const level computed by an immediately invoked lambda: " << level);
}

}  // namespace

LAB_EXAMPLE("Lambda",
            "lambda syntax, captures, mutable, generic lambdas and closures") {
  withAlgorithms();
  captures();
  thisCapture();
  genericAndClosures();
}
