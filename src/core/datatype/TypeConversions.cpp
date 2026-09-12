// -----------------------------------------------------------------------------
// Type conversions, casts, aliases and type deduction
//
// Implicit conversions happen automatically:
//   - promotions (char -> int, float -> double) never lose information
//   - conversions (double -> int, int -> unsigned) may lose or change values
//
// Explicit casts, from safest to most dangerous:
//   static_cast       related types, checked at compile time
//   dynamic_cast      down-casts in polymorphic hierarchies, checked at run time
//   const_cast        adds/removes const (modifying a truly const object is UB)
//   reinterpret_cast  treat the bits as another type (pointers, addresses)
//   (T)value          C-style cast: tries all of the above - avoid it
//
// Reference: https://en.cppreference.com/w/cpp/language/expressions#Conversions
// -----------------------------------------------------------------------------

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Base {
 public:
  virtual ~Base() = default;
  virtual std::string name() const { return "Base"; }
};

class Derived : public Base {
 public:
  std::string name() const override { return "Derived"; }
  std::string onlyInDerived() const { return "Derived::onlyInDerived"; }
};

class Other : public Base {
 public:
  std::string name() const override { return "Other"; }
};

void implicitConversions() {
  LOG_SECTION("Implicit conversions");
  const char letter = 'A';
  const int promoted = letter;  // promotion: char -> int
  const float f = 3.5F;
  const double widened = f;  // promotion: float -> double
  const bool flag = true;
  // NOLINTNEXTLINE(readability-implicit-bool-conversion): the conversion is the point
  const int from_bool = flag;  // true -> 1
  LOG_S("'A' -> " << promoted << ", 3.5F -> " << widened << ", true -> "
                  << from_bool);

  const double pi = 3.14159;
  const int truncated =
      static_cast<int>(pi);  // implicitly `int t = pi;` compiles too
  LOG_S("double 3.14159 -> int " << truncated << " (fraction lost)");

  // Signed/unsigned mixing: -1 is converted to a huge unsigned value first.
  const int minus_one = -1;
  const unsigned int one = 1;
  const bool surprising = static_cast<unsigned int>(minus_one) < one;
  LOG_S("-1 < 1U evaluates as " << std::boolalpha << surprising
                                << " (-1 becomes "
                                << static_cast<unsigned int>(minus_one)
                                << "); -Wsign-compare warns about it");
}

void staticAndConstCast() {
  LOG_SECTION("static_cast and const_cast");
  const double pi = 3.14159;
  LOG_S("static_cast<int>(3.14159) = " << static_cast<int>(pi));

  const Derived derived;
  const Base& as_base =
      static_cast<const Base&>(derived);  // up-cast: always safe
  LOG_S("static_cast to Base& keeps the dynamic type: " << as_base.name());

  const Base sliced = derived;  // OBJECT SLICING: only the Base part is copied
  LOG_S("copying a Derived into a Base object slices it: " << sliced.name());

  int mutable_value = 1;
  const int* read_only = &mutable_value;
  int* writable =
      const_cast<int*>(read_only);  // OK: the object itself is not const
  *writable = 2;
  LOG_S("const_cast removed const from a pointer to a non-const int: "
        << mutable_value);
  // Doing the same with `const int x = 1;` and writing through it is UB.
}

void reinterpretCast() {
  LOG_SECTION("reinterpret_cast");
  const std::uint32_t value = 0x11223344;
  const auto* bytes = reinterpret_cast<const unsigned char*>(
      &value);  // char may alias anything
  LOG_S("0x11223344 as bytes: "
        << std::hex << static_cast<int>(bytes[0]) << ' '
        << static_cast<int>(bytes[1]) << ' ' << static_cast<int>(bytes[2])
        << ' ' << static_cast<int>(bytes[3]) << std::dec
        << (bytes[0] == 0x44 ? "  (little-endian)" : "  (big-endian)"));

  // Embedded use case: memory-mapped I/O (only valid on real hardware)
  //   constexpr std::uintptr_t kRegisterAddress = 0x4001'3800;
  //   auto* const reg = reinterpret_cast<volatile std::uint32_t*>(kRegisterAddress);
  //   *reg = 0xF;
}

void dynamicCast() {
  LOG_SECTION("dynamic_cast (needs a polymorphic base)");
  const std::unique_ptr<Base> derived = std::make_unique<Derived>();
  const std::unique_ptr<Base> other = std::make_unique<Other>();

  if (const auto* as_derived = dynamic_cast<const Derived*>(derived.get())) {
    LOG_S("Base* -> Derived* succeeded: " << as_derived->onlyInDerived());
  }
  if (dynamic_cast<const Derived*>(other.get()) == nullptr) {
    LOG("Base* -> Derived* on an Other returns nullptr");
  }

  try {
    const Base& reference = *other;
    [[maybe_unused]] const Derived& wrong =
        dynamic_cast<const Derived&>(reference);
  } catch (const std::bad_cast& e) {
    LOG_S("failed reference cast throws std::bad_cast: " << e.what());
  }
}

int add(int x, int y) {
  return x + y;
}

auto multiply(int x, int y) -> int {  // trailing return type
  return x * y;
}

void aliasesAndDeduction() {
  LOG_SECTION("Type aliases and type deduction");
  using Celsius = double;     // modern alias
  typedef double Fahrenheit;  // C-style alias, same meaning
  using BinaryOperation = int (*)(int, int);

  const Celsius room = 21.5;
  const Fahrenheit converted = room * 9 / 5 + 32;
  BinaryOperation operation = add;
  LOG_S("21.5 C = " << converted
                    << " F, operation(3, 4) = " << operation(3, 4));
  operation = multiply;
  LOG_S("after operation = multiply: operation(3, 4) = " << operation(3, 4));

  auto integer = 42;                           // int
  auto real = 3.14;                            // double
  const auto sum = integer + real;             // double
  decltype(integer) same_type_as_integer = 7;  // int, taken from an expression
  std::common_type_t<int, double> common = integer;  // double
  LOG_S("auto sum = int + double -> double: " << sum);
  LOG_S("decltype(integer) is int: "
        << std::boolalpha
        << std::is_same_v<decltype(same_type_as_integer), int>);
  LOG_S("std::common_type_t<int, double> is double: "
        << std::boolalpha << std::is_same_v<decltype(common), double>);
}

}  // namespace

LAB_EXAMPLE("TypeConversions",
            "implicit conversions, the four C++ casts, slicing, aliases, "
            "auto/decltype") {
  implicitConversions();
  staticAndConstCast();
  reinterpretCast();
  dynamicCast();
  aliasesAndDeduction();
}
