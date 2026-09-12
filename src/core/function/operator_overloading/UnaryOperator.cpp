// -----------------------------------------------------------------------------
// Overloading unary operators: - + ! ~
//
//   T    operator-() const;   negation, returns a new object
//   T    operator+() const;   unary plus, usually returns a copy
//   bool operator!() const;   logical not
//   T    operator~() const;   bitwise not
//
// Unary operators take no parameters as members (or one as non-members) and
// should not modify the operand.
//
// Reference: https://en.cppreference.com/w/cpp/language/operators
// -----------------------------------------------------------------------------

#include <bitset>
#include <cstdint>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Vector2 {
 public:
  Vector2(double x, double y) : x_{x}, y_{y} {}

  Vector2 operator-() const { return {-x_, -y_}; }
  Vector2 operator+() const { return *this; }
  bool operator!() const {
    return x_ == 0.0 && y_ == 0.0;
  }  // "is it the zero vector?"

  double x() const { return x_; }
  double y() const { return y_; }

 private:
  double x_;
  double y_;
};

class Flags {
 public:
  explicit Flags(std::uint8_t bits) : bits_{bits} {}
  Flags operator~() const { return Flags{static_cast<std::uint8_t>(~bits_)}; }
  std::uint8_t bits() const { return bits_; }

 private:
  std::uint8_t bits_;
};

void run() {
  LOG_SECTION("- + ! on a 2D vector");
  const Vector2 velocity{3.0, -1.5};
  const Vector2 reversed = -velocity;
  const Vector2 copy = +velocity;
  LOG_S("velocity  = (" << velocity.x() << ", " << velocity.y() << ")");
  LOG_S("-velocity = (" << reversed.x() << ", " << reversed.y() << ")");
  LOG_S("+velocity = (" << copy.x() << ", " << copy.y() << ")");
  LOG_S(std::boolalpha << "!velocity = " << !velocity
                       << ", !Vector2{0, 0} = " << !Vector2{0.0, 0.0});

  LOG_SECTION("~ on a set of flags");
  const Flags flags{0b0000'1111};
  LOG_S(" flags = " << std::bitset<8>(flags.bits()));
  LOG_S("~flags = " << std::bitset<8>((~flags).bits()));
}

}  // namespace

LAB_EXAMPLE("UnaryOperator", "overload unary -, +, ! and ~") {
  run();
}
