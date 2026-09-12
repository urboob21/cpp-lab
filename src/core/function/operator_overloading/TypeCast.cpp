// -----------------------------------------------------------------------------
// Conversion operators (user-defined type casts)
//
//   operator Target() const;            implicit conversion to Target
//   explicit operator Target() const;   only with static_cast<Target>(x)
//   explicit operator bool() const;     still works in if/while/&&/!
//
//   - Implicit conversions are convenient but can kick in where you do not
//     expect them; prefer `explicit` unless the conversion is obviously safe.
//   - A converting constructor (Target(Source)) is the other way to define a
//     conversion; avoid defining both directions implicitly.
//
// Reference: https://en.cppreference.com/w/cpp/language/cast_operator
// -----------------------------------------------------------------------------

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Cents {
 public:
  explicit Cents(int cents = 0) : cents_{cents} {}

  explicit operator int() const {  // requires static_cast<int>(cents)
    LOG_FUNC();
    return cents_;
  }

  explicit operator bool() const {
    return cents_ != 0;
  }  // "is there any money?"

 private:
  int cents_;
};

class Dollars {
 public:
  explicit Dollars(int dollars = 0) : dollars_{dollars} {}

  operator Cents()
      const {  // NOLINT: implicit on purpose, Dollars -> Cents is lossless
    LOG_FUNC();
    return Cents{dollars_ * 100};
  }

 private:
  int dollars_;
};

void printCents(const Cents& cents) {
  LOG_S("printCents: " << static_cast<int>(cents) << " cents");
}

void run() {
  LOG_SECTION("Implicit conversion operator");
  const Dollars price{3};
  printCents(price);  // Dollars -> Cents happens automatically

  LOG_SECTION("explicit conversion operators");
  const Cents empty{0};
  const Cents some{250};
  // int raw = some;  // error: operator int is explicit
  LOG_S("static_cast<int>(some) = " << static_cast<int>(some));

  if (some && !empty) {  // explicit operator bool is allowed in conditions
    LOG("`some` has money and `empty` has none");
  }
  // bool flag = some;  // error: explicit operator bool needs a cast here
  // int sum = some + 1; // error: explicit prevents surprising arithmetic
}

}  // namespace

LAB_EXAMPLE(
    "TypeCast",
    "implicit and explicit conversion operators, explicit operator bool") {
  run();
}
