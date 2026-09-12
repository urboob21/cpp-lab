// -----------------------------------------------------------------------------
// Operators
//
//   - arithmetic : + - * / %  ++ --  (integer division truncates toward zero)
//   - logical    : && || !           (&& and || short-circuit)
//   - bitwise    : & | ^ ~ << >>     (work on the bits of integers)
//
// Pitfalls:
//   - Unsigned arithmetic wraps around; signed overflow is undefined behavior.
//   - The comma operator evaluates both sides and yields the right one.
//
// Reference: https://en.cppreference.com/w/cpp/language/expressions#Operators
// -----------------------------------------------------------------------------

#include <bitset>
#include <cstdint>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void arithmetic() {
  LOG_SECTION("Arithmetic operators");

  int a{100};
  const int b{200};
  LOG_S("a = " << a << ", b = " << b);
  LOG_S("a + b = " << a + b);
  LOG_S("a - b = " << a - b);
  LOG_S("a * b = " << a * b);
  LOG_S("a / b = " << a / b << "     <- integer division drops the fraction");
  LOG_S("a % b = " << a % b);
  LOG_S("-7 / 2 = " << -7 / 2 << ", -7 % 2 = " << -7 % 2
                    << "  <- truncates toward zero, % keeps the sign of -7");
  LOG_S("7.0 / 2 = " << 7.0 / 2
                     << "  <- a double operand gives floating-point division");

  // Converting a negative value to an unsigned type wraps around modulo 2^32.
  const auto wrapped = static_cast<std::uint32_t>(a - b);
  LOG_S("uint32_t(a - b) = " << wrapped << "  <- -100 wrapped around");

  LOG_SECTION("Increment and decrement");
  const int pre = ++a;  // increments first, then yields the new value
  LOG_S("++a yields " << pre << ", a is now " << a);
  const int post = a++;  // yields a copy of the old value, then increments
  LOG_S("a++ yields " << post << ", a is now " << a);

  // Comma operator: evaluate the left side, discard it, yield the right side.
  const int comma = (++a, b);
  LOG_S("(++a, b) yields " << comma << ", a is now " << a);
}

/// Returns `value` and logs that it was evaluated.
bool traced(bool value, const char* label) {
  LOG_S("  evaluated " << label);
  return value;
}

void logical() {
  LOG_SECTION("Logical operators");

  const bool yes = true;
  const bool no = false;
  LOG_S(std::boolalpha << "true && false = " << (yes && no));
  LOG_S(std::boolalpha << "true || false = " << (yes || no));
  LOG_S(std::boolalpha << "!true         = " << !yes);

  LOG("false && rhs -> rhs is never evaluated (short-circuit):");
  const bool and_result = traced(false, "lhs") && traced(true, "rhs");
  LOG("true || rhs  -> rhs is never evaluated either:");
  const bool or_result = traced(true, "lhs") || traced(false, "rhs");
  LOG_S(std::boolalpha << "results: " << and_result << ", " << or_result);
}

void bitwise() {
  LOG_SECTION("Bitwise operators");

  const std::bitset<8> a{0b1111'0000};  // ' separates digits (C++14)
  const std::bitset<8> b{0b1010'1010};
  LOG_S("a      = " << a);
  LOG_S("b      = " << b);
  LOG_S("a & b  = " << (a & b) << "  (1 where both are 1)");
  LOG_S("a | b  = " << (a | b) << "  (1 where either is 1)");
  LOG_S("a ^ b  = " << (a ^ b) << "  (1 where they differ)");
  LOG_S("~a     = " << ~a);
  LOG_S("a << 1 = " << (a << 1));
  LOG_S("a >> 1 = " << (a >> 1));

  LOG_SECTION(
      "Bit manipulation on an integer (typical for hardware registers)");
  std::uint8_t reg = 0b0100'0001;
  LOG_S("start        : " << std::bitset<8>(reg));
  reg = static_cast<std::uint8_t>(reg | (1U << 3U));  // set bit 3
  LOG_S("set bit 3    : " << std::bitset<8>(reg));
  reg = static_cast<std::uint8_t>(reg ^ (1U << 0U));  // toggle bit 0
  LOG_S("toggle bit 0 : " << std::bitset<8>(reg));
  reg = static_cast<std::uint8_t>(reg & ~(1U << 3U));  // clear bit 3
  LOG_S("clear bit 3  : " << std::bitset<8>(reg));
  const bool bit0 = ((reg >> 0U) & 1U) != 0U;  // test bit 0
  LOG_S("bit 0 is set : " << std::boolalpha << bit0);
}

}  // namespace

LAB_EXAMPLE("Operations",
            "arithmetic, logical (short-circuit) and bitwise operators") {
  arithmetic();
  logical();
  bitwise();
}
