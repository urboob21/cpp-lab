// -----------------------------------------------------------------------------
// Overloading ++ and -- (prefix and postfix)
//
//   T& operator++();     prefix  ++x : increment, return the object itself
//   T  operator++(int);  postfix x++ : copy, increment, return the OLD copy
//
// The unused `int` parameter only tells the compiler which one is postfix.
// Postfix must create a copy, so prefer ++it in loops over iterators and other
// objects that are expensive to copy.
//
// Reference: https://en.cppreference.com/w/cpp/language/operator_incdec
// -----------------------------------------------------------------------------

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Counter {
 public:
  explicit Counter(int value) : value_{value} {}
  int value() const { return value_; }

  Counter& operator++() {  // prefix
    LOG_FUNC();
    ++value_;
    return *this;
  }

  Counter& operator--() {  // prefix
    LOG_FUNC();
    --value_;
    return *this;
  }

  Counter operator++(int) {  // postfix
    LOG_FUNC();
    Counter old{*this};  // 1. remember the old value
    ++(*this);           // 2. reuse the prefix version
    return old;          // 3. return the old value
  }

  Counter operator--(int) {  // postfix
    LOG_FUNC();
    Counter old{*this};
    --(*this);
    return old;
  }

 private:
  int value_;
};

void run() {
  LOG_SECTION("Prefix vs postfix");
  Counter counter{25};

  const int prefix_result = (++counter).value();
  LOG_S("++counter returned " << prefix_result << ", counter is "
                              << counter.value());

  const int postfix_result = (counter++).value();
  LOG_S("counter++ returned " << postfix_result << ", counter is "
                              << counter.value());

  --counter;
  counter--;
  LOG_S("after --counter and counter--: " << counter.value());

  LOG_SECTION("Prefix can be chained, postfix returns a temporary");
  ++ ++counter;  // both increments apply to `counter`
  LOG_S("++ ++counter -> " << counter.value());
}

}  // namespace

LAB_EXAMPLE("InDecOperator",
            "prefix and postfix ++/--, why ++it is preferred") {
  run();
}
