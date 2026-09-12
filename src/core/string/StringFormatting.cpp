// -----------------------------------------------------------------------------
// Formatting text: four ways to build "User Phong has 100 points, Pi: 3.14"
//
//   1. std::format (C++20)   type-safe, Python-like "{:.2f}" syntax  <- preferred
//   2. operator+             simple, but std::to_string has fixed precision
//   3. std::ostringstream    flexible, verbose, sticky manipulators
//   4. std::snprintf         C style, fast, not type-safe
//
// std::format specifiers: {[index]:[fill][align][width][.precision][type]}
//   {:>8}  right-align in 8 columns     {:*^9}  center, fill with '*'
//   {:.2f} two decimals                 {:#x}   hex with 0x prefix
//
// Reference: https://en.cppreference.com/w/cpp/utility/format/format
// -----------------------------------------------------------------------------

#include <cstdio>
#include <format>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

const std::string kName = "Phong";
constexpr int kScore = 100;
constexpr double kPi = 3.14159;

void stdFormat() {
  LOG_SECTION("1. std::format");
  LOG(std::format("User {} has {} points, Pi: {:.2f}", kName, kScore, kPi));
  LOG(std::format("|{:>8}|{:<8}|{:*^9}|", "right", "left", "center"));
  LOG(std::format("hex {:#x}, binary {:#b}, padded {:05}, sign {:+}", 255, 5,
                  42, 7));
  LOG(std::format("positional: {1} before {0}", "second", "first"));

  std::string buffer;
  std::format_to(std::back_inserter(buffer), "appended {} times",
                 3);  // no temporary string
  LOG(buffer);
}

void concatenation() {
  LOG_SECTION("2. operator+");
  const std::string text = "User " + kName + " has " + std::to_string(kScore) +
                           " points, Pi: " + std::to_string(kPi);
  LOG(text + "   <- to_string always prints 6 decimals");
}

void stream() {
  LOG_SECTION("3. std::ostringstream");
  std::ostringstream out;
  out << "User " << kName << " has " << kScore << " points, Pi: " << std::fixed
      << std::setprecision(2) << kPi;
  LOG(out.str());
}

void cStyle() {
  LOG_SECTION("4. std::snprintf");
  char buffer[128];
  const int written =
      std::snprintf(buffer, sizeof(buffer), "User %s has %d points, Pi: %.2f",
                    kName.c_str(), kScore, kPi);
  LOG_S(buffer << "   (" << written << " characters)");
  // A wrong specifier such as %d for a double compiles and is undefined behavior.
}

struct Point {
  int x;
  int y;
};

}  // namespace

/// Teaching std::format about a custom type.
template <>
struct std::formatter<Point> : std::formatter<std::string> {
  auto format(const Point& point, std::format_context& context) const {
    return std::formatter<std::string>::format(
        std::format("({}, {})", point.x, point.y), context);
  }
};

namespace {

void customType() {
  LOG_SECTION("std::formatter for a custom type");
  const Point point{3, -4};
  LOG(std::format("point = {}, right-aligned: [{:>10}]", point, point));
}

}  // namespace

LAB_EXAMPLE(
    "StringFormatting",
    "std::format, concatenation, ostringstream, snprintf, custom formatters") {
  stdFormat();
  concatenation();
  stream();
  cStyle();
  customType();
}
