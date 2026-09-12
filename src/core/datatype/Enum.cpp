// -----------------------------------------------------------------------------
// Enumerations
//
//   enum Color { kRed, kGreen };          unscoped: names leak into the
//                                         enclosing scope, converts to int
//   enum class Fruit { kApple, kPear };   scoped (C++11): names stay inside,
//                                         no implicit conversion - prefer it
//   enum class Status : std::uint8_t {}   fixed underlying type (size, protocols)
//   using enum Fruit;                     (C++20) bring enumerators into scope
//
// There is no built-in enum -> string conversion; a switch is the usual way.
//
// Reference: https://en.cppreference.com/w/cpp/language/enum
// -----------------------------------------------------------------------------

#include <cstdint>
#include <string_view>
#include <type_traits>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

enum Color {
  kRed,
  kGreen,
  kBlue
};  // unscoped: kRed is visible everywhere here

enum class Fruit { kApple, kPear, kCherry };  // scoped

enum class Status : std::uint8_t {  // one byte, explicit values
  kOk = 0,
  kWarning = 1,
  kError = 2,
};

/// Bit flags with a scoped enum need their own operators.
enum class Permission : unsigned {
  kNone = 0,
  kRead = 1U << 0U,
  kWrite = 1U << 1U,
  kExecute = 1U << 2U,
};

constexpr Permission operator|(Permission a, Permission b) {
  return static_cast<Permission>(static_cast<unsigned>(a) |
                                 static_cast<unsigned>(b));
}

constexpr bool has(Permission set, Permission flag) {
  return (static_cast<unsigned>(set) & static_cast<unsigned>(flag)) != 0U;
}

std::string_view toString(Fruit fruit) {
  switch (fruit) {  // -Wswitch warns if an enumerator is not handled
    case Fruit::kApple:
      return "apple";
    case Fruit::kPear:
      return "pear";
    case Fruit::kCherry:
      return "cherry";
  }
  return "unknown";
}

/// Converts any enum to its underlying integer (std::to_underlying in C++23).
template <typename Enum>
constexpr auto toUnderlying(Enum value) {
  return static_cast<std::underlying_type_t<Enum>>(value);
}

void unscoped() {
  LOG_SECTION("Unscoped enum");
  const Color color = kGreen;
  const int as_int = color;  // implicit conversion to int
  LOG_S("kGreen converts to " << as_int << ", and kBlue + 1 = " << kBlue + 1
                              << " (compiles, but is rarely meaningful)");
}

void scoped() {
  LOG_SECTION("Scoped enum (enum class)");
  const Fruit fruit = Fruit::kPear;
  // const int bad = fruit;  // error: no implicit conversion
  LOG_S("Fruit::kPear -> \"" << toString(fruit) << "\", underlying value "
                             << toUnderlying(fruit));

  using enum Fruit;  // C++20: kCherry can be used without the Fruit:: prefix
  LOG_S("using enum Fruit; toString(kCherry) = " << toString(kCherry));
}

void underlyingType() {
  LOG_SECTION("Underlying type");
  LOG_S("sizeof(Color)  = " << sizeof(Color)
                            << " (implementation-defined for unscoped)");
  LOG_S("sizeof(Fruit)  = " << sizeof(Fruit)
                            << " (enum class defaults to int)");
  LOG_S("sizeof(Status) = " << sizeof(Status) << " (: std::uint8_t)");
  LOG_S("Status::kError = "
        << static_cast<int>(Status::kError)
        << "  (cast uint8_t to int, or it prints as a char)");
}

void flags() {
  LOG_SECTION("Bit flags");
  const Permission permissions = Permission::kRead | Permission::kWrite;
  LOG_S(std::boolalpha << "read: " << has(permissions, Permission::kRead)
                       << ", write: " << has(permissions, Permission::kWrite)
                       << ", execute: "
                       << has(permissions, Permission::kExecute));
}

}  // namespace

LAB_EXAMPLE(
    "Enum",
    "unscoped vs scoped enums, underlying types, using enum, bit flags") {
  unscoped();
  scoped();
  underlyingType();
  flags();
}
