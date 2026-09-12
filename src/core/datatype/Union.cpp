// -----------------------------------------------------------------------------
// Unions and std::variant
//
//   - All members of a union share the same memory; its size is that of the
//     largest member (plus padding).
//   - Only the member written last is "active". Reading another member is
//     allowed in C but UNDEFINED BEHAVIOR in C++.
//   - To reinterpret bytes, use std::memcpy or std::bit_cast (C++20).
//   - A union does not know which member is active. A "tagged union" stores
//     that separately - std::variant (C++17) does it safely for you.
//
// Reference: https://en.cppreference.com/w/cpp/language/union
// -----------------------------------------------------------------------------

#include <bit>
#include <cstdint>
#include <string>
#include <type_traits>
#include <variant>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

union Number {
  std::int32_t integer;
  float real;
  char letter;
};

void rawUnion() {
  LOG_SECTION("A plain union");
  Number number{};
  number.integer = 65;  // `integer` is now the active member
  LOG_S("integer = " << number.integer);

  number.real = 3.14F;  // switches the active member to `real`
  LOG_S("real    = " << number.real);
  // Reading number.integer now would be undefined behavior in C++.

  LOG_S("sizeof(Number) = " << sizeof(Number) << " (largest member)");
}

void typePunning() {
  LOG_SECTION("Reading the bits of a float correctly");
  const float real = 1.0F;
  const auto bits = std::bit_cast<std::uint32_t>(real);  // C++20, well-defined
  LOG_S("std::bit_cast<std::uint32_t>(1.0F) = 0x" << std::hex << bits
                                                  << std::dec << " (IEEE-754)");
}

/// A hand-written tagged union: the tag says which member is active.
struct TaggedNumber {
  enum class Kind { kInteger, kReal } kind;
  union {
    std::int32_t integer;
    float real;
  };
};

void describe(const TaggedNumber& number) {
  if (number.kind == TaggedNumber::Kind::kInteger) {
    LOG_S("  tagged: integer " << number.integer);
  } else {
    LOG_S("  tagged: real " << number.real);
  }
}

using Value = std::variant<int, double, std::string>;

void describe(const Value& value) {
  // std::visit calls the lambda with the currently held alternative.
  std::visit(
      [](const auto& held) {
        using T = std::decay_t<decltype(held)>;
        if (std::is_same_v<T, int>) {
          LOG_S("  variant holds int " << held);
        } else if (std::is_same_v<T, double>) {
          LOG_S("  variant holds double " << held);
        } else {
          LOG_S("  variant holds std::string \"" << held << "\"");
        }
      },
      value);
}

void taggedUnions() {
  LOG_SECTION("Tagged union vs std::variant");
  TaggedNumber tagged{};
  tagged.kind = TaggedNumber::Kind::kInteger;
  tagged.integer = 7;
  describe(tagged);
  tagged.kind =
      TaggedNumber::Kind::kReal;  // forgetting this line would be a bug
  tagged.real = 2.5F;
  describe(tagged);

  Value value = 42;
  describe(value);
  value = 3.5;
  describe(value);
  value =
      std::string("hello");  // std::string would not even fit in a plain union
  describe(value);

  if (const auto* text = std::get_if<std::string>(&value)) {
    LOG_S("  std::get_if<std::string> -> " << *text);
  }
  try {
    [[maybe_unused]] const int wrong = std::get<int>(value);
  } catch (const std::bad_variant_access&) {
    LOG("  std::get<int> on a string alternative throws "
        "std::bad_variant_access");
  }
  LOG_S("  sizeof(Value) = " << sizeof(Value)
                             << " (largest alternative + index)");
}

}  // namespace

LAB_EXAMPLE("Union",
            "unions, the active member rule, std::bit_cast, tagged unions and "
            "std::variant") {
  rawUnion();
  typePunning();
  taggedUnions();
}
