// -----------------------------------------------------------------------------
// std::optional (C++17) - a value that may be absent
//
// Older ways to say "no value" all have drawbacks:
//   - magic values (-1, "")   : ambiguous, the caller may forget to check
//   - nullptr                 : needs somewhere to point, unclear ownership
//   - exceptions              : heavy for an expected situation
//   - bool + out parameter    : clumsy call sites
//
// std::optional<T> stores the T inline (no allocation) plus a flag.
//   if (opt) / opt.has_value()   check
//   *opt / opt->member           access (undefined if empty!)
//   opt.value()                  access, throws std::bad_optional_access if empty
//   opt.value_or(fallback)       access with a default
//
// Reference: https://en.cppreference.com/w/cpp/utility/optional
// -----------------------------------------------------------------------------

#include <charconv>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// --- the old ways ------------------------------------------------------------

/// Returns -1 when the input is invalid... but what if -1 is a valid number?
int parseWithMagicValue(std::string_view text) {
  int value = 0;
  const auto [end, error] =
      std::from_chars(text.data(), text.data() + text.size(), value);
  return error == std::errc{} && end == text.data() + text.size() ? value : -1;
}

/// Throws for invalid input - exceptions for an everyday situation.
int parseOrThrow(std::string_view text) {
  const int value = parseWithMagicValue(text);
  if (value == -1 && text != "-1") {
    throw std::invalid_argument("not a number: " + std::string(text));
  }
  return value;
}

// --- the optional way -------------------------------------------------------

std::optional<int> parse(std::string_view text) {
  int value = 0;
  const auto [end, error] =
      std::from_chars(text.data(), text.data() + text.size(), value);
  if (error != std::errc{} || end != text.data() + text.size()) {
    return std::nullopt;  // clearly "no value"
  }
  return value;
}

void oldWays() {
  LOG_SECTION("Before std::optional");
  LOG_S("parseWithMagicValue(\"42\")  = " << parseWithMagicValue("42"));
  LOG_S("parseWithMagicValue(\"abc\") = " << parseWithMagicValue("abc")
                                          << "  <- error?");
  LOG_S("parseWithMagicValue(\"-1\")  = " << parseWithMagicValue("-1")
                                          << "  <- or a real -1?");
  try {
    parseOrThrow("abc");
  } catch (const std::invalid_argument& e) {
    LOG_S("parseOrThrow(\"abc\") threw: " << e.what());
  }
}

void usingOptional() {
  LOG_SECTION("With std::optional");
  for (const std::string_view input : {"42", "-1", "abc"}) {
    if (const std::optional<int> number = parse(input)) {  // explicit check
      LOG_S("parse(\"" << input << "\") -> " << *number);
    } else {
      LOG_S("parse(\"" << input << "\") -> no value");
    }
  }

  const std::optional<int> missing = parse("oops");
  LOG_S("value_or(0)  -> " << missing.value_or(0));
  try {
    LOG_S("value()      -> " << missing.value());
  } catch (const std::bad_optional_access& e) {
    LOG_S("value() on an empty optional throws: " << e.what());
  }
  // *missing would be undefined behavior - always check first.
}

/// Optional parameter with a default of "not provided".
std::string greet(const std::string& name,
                  std::optional<std::string> title = std::nullopt) {
  return "Hello, " + (title ? *title + " " : std::string{}) + name;
}

void modifying() {
  LOG_SECTION("Optional parameters, emplace and reset");
  LOG(greet("Ada"));
  LOG(greet("Lovelace", "Countess"));

  std::optional<std::string> cache;
  LOG_S("has_value() = " << std::boolalpha << cache.has_value());
  cache.emplace(3, 'z');  // constructs "zzz" in place
  LOG_S("after emplace(3, 'z'): " << *cache << ", size via -> "
                                  << cache->size());
  cache.reset();
  LOG_S("after reset(): has_value() = " << std::boolalpha << cache.has_value());
  LOG_S("sizeof(std::optional<int>) = " << sizeof(std::optional<int>)
                                        << " (int + flag)");
}

}  // namespace

LAB_EXAMPLE(
    "Optional",
    "std::optional instead of magic values, null pointers or exceptions") {
  oldWays();
  usingOptional();
  modifying();
}
