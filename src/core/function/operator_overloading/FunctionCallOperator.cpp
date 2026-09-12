// -----------------------------------------------------------------------------
// Overloading operator() - function objects (functors)
//
//   struct Linear { double operator()(double x) const { return a * x + b; } };
//
//   - An object with operator() can be called like a function, but it can
//     also carry STATE (parameters, counters, caches).
//   - Standard algorithms accept functors, e.g. std::sort comparators.
//   - A lambda is a compiler-generated functor; write a class when the object
//     needs a name, several overloads or to be reused in many places.
//
// Reference: https://en.cppreference.com/w/cpp/language/operators#Function_call_operator
// -----------------------------------------------------------------------------

#include <algorithm>
#include <string>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// Represents the function f(x) = a * x + b.
struct Linear {
  double a;
  double b;
  double operator()(double x) const { return a * x + b; }
};

/// A stateful functor: counts how often it is called.
class CallCounter {
 public:
  void operator()(int value) {
    ++calls_;
    sum_ += value;
  }
  int calls() const { return calls_; }
  int sum() const { return sum_; }

 private:
  int calls_{0};
  int sum_{0};
};

/// Comparator functor with a parameter: sort by distance to a target.
class CloserTo {
 public:
  explicit CloserTo(int target) : target_{target} {}
  bool operator()(int lhs, int rhs) const {
    return distance(lhs) < distance(rhs);
  }

 private:
  int distance(int value) const {
    return value > target_ ? value - target_ : target_ - value;
  }
  int target_;
};

/// Several overloads in one object.
struct Describe {
  std::string operator()(int value) const {
    return "int " + std::to_string(value);
  }
  std::string operator()(const std::string& value) const {
    return "string \"" + value + "\"";
  }
};

void run() {
  LOG_SECTION("A function with parameters");
  const Linear f{2.0, 1.0};  // f(x) = 2x + 1
  LOG_S("f(0) = " << f(0) << ", f(3) = " << f(3));

  LOG_SECTION("A functor that keeps state");
  const std::vector<int> values{4, 8, 15, 16, 23, 42};
  // std::for_each returns (a copy of) the functor, including its state.
  const CallCounter counter =
      std::for_each(values.begin(), values.end(), CallCounter{});
  LOG_S("called " << counter.calls() << " times, sum = " << counter.sum());

  LOG_SECTION("A functor as a comparator");
  std::vector<int> numbers{1, 9, 14, 20, 11, 7};
  std::sort(numbers.begin(), numbers.end(), CloserTo{10});
  std::string sorted;
  for (const int n : numbers) {
    sorted += std::to_string(n) + " ";
  }
  LOG_S("sorted by distance to 10: " << sorted);

  LOG_SECTION("Overloaded operator()");
  const Describe describe;
  LOG_S(describe(42) << ", " << describe(std::string("hello")));
}

}  // namespace

LAB_EXAMPLE(
    "FunctionCallOperator",
    "operator(): function objects with parameters, state and overloads") {
  run();
}
