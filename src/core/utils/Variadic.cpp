// -----------------------------------------------------------------------------
// Variadic functions: C-style `...` vs variadic templates
//
// C style (<cstdarg>):   void log(const char* format, ...);
//   va_list args; va_start(args, format); va_arg(args, int); va_end(args);
//   - No type safety: the format must match the arguments exactly.
//   - Default promotions apply: char/short -> int, float -> double.
//
// C++ variadic templates:  template <typename... Args> void log(Args&&... args);
//   - Fully type-safe, resolved at compile time.
//   - sizeof...(Args) counts the arguments.
//   - C++17 fold expressions (args + ...) replace most recursion.
//   - std::forward<Args>(args)... passes arguments on unchanged.
//
// Reference: https://en.cppreference.com/w/cpp/language/variadic_arguments
//            https://en.cppreference.com/w/cpp/language/pack
// -----------------------------------------------------------------------------

#include <cstdarg>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace c_style {

/// Reads one argument per format character: d = int, c = char, f = double.
std::string format(const char* spec, ...) {
  std::ostringstream out;
  va_list args;
  va_start(args, spec);  // start reading after the last named parameter
  for (; *spec != '\0'; ++spec) {
    switch (*spec) {
      case 'd':
        out << va_arg(args, int) << ' ';
        break;
      case 'c':
        out << static_cast<char>(va_arg(args, int))
            << ' ';  // char was promoted to int
        break;
      case 'f':
        out << va_arg(args, double) << ' ';  // float was promoted to double
        break;
      default:
        out << "<unknown '" << *spec << "'> ";
        break;
    }
  }
  va_end(args);  // always pair va_start with va_end
  return out.str();
}

void run() {
  LOG_SECTION("C-style variadic function");
  LOG_S("format(\"dcff\", 3, 'a', 1.999, 42.5) -> "
        << format("dcff", 3, 'a', 1.999, 42.5));
  // format("d", 1.5) compiles too but reads garbage: undefined behavior.
}

}  // namespace c_style

namespace templates {

// Recursion (C++11 style): one base case plus one step.
void printRecursive(std::ostringstream& /*out*/) {}

template <typename First, typename... Rest>
void printRecursive(std::ostringstream& out, const First& first,
                    const Rest&... rest) {
  out << first << ' ';
  printRecursive(out, rest...);  // expands the remaining arguments
}

// Fold expressions (C++17): no recursion needed.
template <typename... Numbers>
auto sum(const Numbers&... numbers) {
  return (numbers + ... + 0);  // ((n1 + (n2 + (n3 + 0))))
}

template <typename... Args>
std::string joinAll(const Args&... args) {
  std::ostringstream out;
  ((out << args << ' '), ...);  // comma fold: stream every argument
  return out.str();
}

template <typename... Args>
std::size_t count(const Args&... /*args*/) {
  return sizeof...(Args);
}

/// Perfect forwarding: build any T from any arguments, like std::make_unique.
template <typename T, typename... Args>
std::unique_ptr<T> create(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

void run() {
  LOG_SECTION("Variadic templates");
  std::ostringstream out;
  printRecursive(out, 3, 'a', 1.999, "text", std::string("string"));
  LOG_S("printRecursive -> " << out.str());
  LOG_S("sum(1, 2, 3.5)  -> " << sum(1, 2, 3.5));
  LOG_S("joinAll(...)    -> " << joinAll("pi is", 3.14, '!', 42));
  LOG_S("count(1, \"two\", 3.0) -> " << count(1, "two", 3.0));

  const auto text =
      create<std::string>(5, '*');  // forwards (5, '*') to std::string
  LOG_S("create<std::string>(5, '*') -> " << *text);
}

}  // namespace templates

}  // namespace

LAB_EXAMPLE(
    "Variadic",
    "C-style va_list vs variadic templates, fold expressions, forwarding") {
  c_style::run();
  templates::run();
}
