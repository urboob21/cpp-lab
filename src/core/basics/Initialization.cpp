// -----------------------------------------------------------------------------
// Initialization
//
// C++ has several ways to give an object its first value:
//   T x;           default initialization (built-in types: indeterminate value!)
//   T x{};         value initialization   (built-in types: zero)
//   T x(args);     direct initialization
//   T x = expr;    copy initialization    (explicit constructors not allowed)
//   T x{args};     list initialization    (C++11, rejects narrowing)
//   T x{.a = 1};   designated initializer (C++20, aggregates only)
//
// Rule of thumb: prefer braces `T x{...}` - they always initialize and they
// refuse narrowing conversions such as double -> int.
//
// Reference: https://en.cppreference.com/w/cpp/language/initialization
// -----------------------------------------------------------------------------

#include <string>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// Logs which constructor runs for each form of initialization.
struct Tracer {
  Tracer() { LOG("  Tracer()              default constructor"); }
  explicit Tracer(int value) {
    LOG_S("  Tracer(int)           value = " << value);
  }
  Tracer(const Tracer& /*other*/) {
    LOG("  Tracer(const Tracer&) copy constructor");
  }
};

/// An aggregate: no user-declared constructors and only public members.
struct Point {
  int x;
  int y;
};

void defaultInitialization() {
  LOG_SECTION("Default initialization: T x;");

  [[maybe_unused]] int
      number;  // indeterminate: reading it is undefined behavior
  LOG("int number;   -> indeterminate value, assign before reading it");

  auto* heap_number = new int;  // also indeterminate
  *heap_number = 42;
  LOG_S("new int       -> indeterminate until assigned, now " << *heap_number);
  delete heap_number;

  LOG("Tracer tracer; -> class types run their default constructor:");
  const Tracer tracer;
}

void valueInitialization() {
  LOG_SECTION("Value initialization: T x{}; / T()");

  const int braces{};
  const int parentheses = int();
  LOG_S("int braces{} = " << braces << ", int() = " << parentheses);

  auto* heap_number = new int();  // new int() and new int{} are zero
  LOG_S("new int()     = " << *heap_number);
  delete heap_number;

  LOG("Tracer tracer{};");
  const Tracer tracer{};

  // Tracer oops();  // Most vexing parse: this declares a FUNCTION named oops!
}

void directAndCopyInitialization() {
  LOG_SECTION("Direct T x(args) and copy T x = other");

  LOG("Tracer direct(1);");
  const Tracer direct(1);

  LOG("Tracer copy = direct;");
  [[maybe_unused]] const Tracer copy = direct;
  // Tracer implicit = 2;  // error: copy initialization ignores explicit constructors

  const std::string text = "copy-initialized from a string literal";
  LOG(text);
}

void listInitialization() {
  LOG_SECTION("List initialization: T x{args}");

  LOG("Tracer direct_list{3};");
  [[maybe_unused]] const Tracer direct_list{3};

  LOG("Tracer copy_list = Tracer{4};   (C++17 guarantees no copy is made)");
  [[maybe_unused]] const Tracer copy_list = Tracer{4};

  const std::vector<int> listed{
      3, 7};  // uses the std::initializer_list constructor
  const std::vector<int> sized(3, 7);  // uses the (count, value) constructor
  LOG_S("std::vector<int>{3, 7}.size() = " << listed.size()
                                           << "  -> elements 3 and 7");
  LOG_S("std::vector<int>(3, 7).size() = "
        << sized.size() << "  -> three 7s. () and {} differ!");

  const double pi = 3.14159;
  // const int narrowed{pi};  // error: braces reject narrowing conversions
  const int truncated(pi);  // compiles and silently drops the fraction
  LOG_S("int truncated(3.14159) = "
        << truncated << "  (int narrowed{3.14159} does not compile)");
}

void aggregateInitialization() {
  LOG_SECTION("Aggregate initialization");

  const Point p1 = {1, 2};
  const Point p2{3, 4};
  const Point p3{.x = 5, .y = 6};  // C++20: members named, in declaration order
  const Point p4{};                // every member value-initialized (zero)
  // Point p5{7};  // also valid: y is value-initialized to 0 (GCC warns with -Wextra)

  LOG_S("Point p1 = {1, 2}        -> {" << p1.x << ", " << p1.y << "}");
  LOG_S("Point p2{3, 4}           -> {" << p2.x << ", " << p2.y << "}");
  LOG_S("Point p3{.x = 5, .y = 6} -> {" << p3.x << ", " << p3.y << "}");
  LOG_S("Point p4{}               -> {" << p4.x << ", " << p4.y << "}");
}

}  // namespace

LAB_EXAMPLE("Initialization",
            "default, value, direct, copy, list and aggregate initialization") {
  defaultInitialization();
  valueInitialization();
  directAndCopyInitialization();
  listInitialization();
  aggregateInitialization();
}
