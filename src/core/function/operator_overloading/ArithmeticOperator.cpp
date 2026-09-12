// -----------------------------------------------------------------------------
// Overloading arithmetic operators: + - * / and += -= *= /=
//
// Canonical pattern:
//   - Implement the compound operator (+=) as a MEMBER returning *this.
//   - Implement the binary operator (+) as a NON-MEMBER in terms of +=.
//     A non-member works when the left operand is not our class: 5 + cents.
//   - Take operands by const reference (or by value when cheap) and return a
//     new object by value.
//
// Reference: https://en.cppreference.com/w/cpp/language/operators#Binary_arithmetic_operators
// -----------------------------------------------------------------------------

#include <stdexcept>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Cents {
 public:
  explicit Cents(int cents) : cents_{cents} {}
  int value() const { return cents_; }

  // Compound assignment operators modify *this and return it by reference,
  // so they can be chained like built-in types.
  Cents& operator+=(const Cents& other) {
    cents_ += other.cents_;
    return *this;
  }
  Cents& operator-=(const Cents& other) {
    cents_ -= other.cents_;
    return *this;
  }
  Cents& operator*=(int factor) {
    cents_ *= factor;
    return *this;
  }
  Cents& operator/=(int divisor) {
    if (divisor == 0) {
      throw std::invalid_argument("division of Cents by zero");
    }
    cents_ /= divisor;
    return *this;
  }

 private:
  int cents_;
};

// Non-members built on the compound operators. `lhs` is taken by value: it is
// the copy we modify and return.
Cents operator+(Cents lhs, const Cents& rhs) {
  return lhs += rhs;
}
Cents operator-(Cents lhs, const Cents& rhs) {
  return lhs -= rhs;
}
Cents operator*(Cents lhs, int factor) {
  return lhs *= factor;
}
Cents operator*(int factor, Cents rhs) {  // allows 3 * cents
  return rhs *= factor;
}
Cents operator/(Cents lhs, int divisor) {
  return lhs /= divisor;
}

// Mixed operands: with a member operator+ only `cents + 5` could work,
// never `5 + cents`, because the left operand of a member is always *this.
Cents operator+(const Cents& lhs, int rhs) {
  return Cents{lhs.value() + rhs};
}
Cents operator+(int lhs, const Cents& rhs) {
  return Cents{lhs + rhs.value()};
}

void run() {
  LOG_SECTION("Binary operators");
  const Cents a{25};
  const Cents b{75};
  LOG_S("a + b    = " << (a + b).value());
  LOG_S("a - b    = " << (a - b).value());
  LOG_S("a * 3    = " << (a * 3).value() << ", 3 * a = " << (3 * a).value());
  LOG_S("b / 2    = " << (b / 2).value());
  LOG_S("a + 5    = " << (a + 5).value() << ", 5 + a = " << (5 + a).value());

  LOG_SECTION("Compound assignment and chaining");
  Cents total{0};
  total += a;
  (total += b) += Cents{100};  // returns Cents&, so it can be chained
  LOG_S("total    = " << total.value());

  try {
    total /= 0;
  } catch (const std::invalid_argument& e) {
    LOG_S("total /= 0 throws: " << e.what());
  }
}

}  // namespace

LAB_EXAMPLE("ArithmeticOperator",
            "overload + - * / via += -= *= /=, mixed operand types") {
  run();
}
