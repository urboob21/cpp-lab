// -----------------------------------------------------------------------------
// References
//
//   int& ref = x;          lvalue reference: another name (alias) for x
//   const int& cref = 42;  const reference: can bind to temporaries
//   int&& rref = 42;       rvalue reference: binds to temporaries / std::move
//
//   - A reference must be initialized and can never be re-bound.
//   - Unlike a pointer it cannot be null.
//   - Pass `const T&` to avoid copies of large objects; `T&` for out-params.
//   - Never return a reference to a local variable (it dangles).
//
// Reference: https://en.cppreference.com/w/cpp/language/reference
// -----------------------------------------------------------------------------

#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void basics() {
  LOG_SECTION("lvalue references");
  int value = 10;
  int& alias = value;  // alias IS value
  // int& unbound;     // error: references must be initialized

  alias = 20;
  LOG_S("alias = 20 -> value = " << value);
  LOG_S("&alias == &value: " << std::boolalpha << (&alias == &value));

  int other = 99;
  alias = other;  // assigns 99 to value, does NOT re-bind alias to other
  LOG_S("alias = other -> value = " << value
                                    << " (alias still refers to value)");
}

void increment(int& number) {
  ++number;  // modifies the caller's variable
}

std::size_t length(const std::string& text) {  // no copy, cannot modify
  return text.size();
}

const std::string& greeting() {
  static const std::string kGreeting = "hello";  // outlives the function
  return kGreeting;
  // Returning a reference to a local std::string would dangle.
}

void parameters() {
  LOG_SECTION("Passing and returning references");
  int counter = 0;
  increment(counter);
  LOG_S("increment(counter) -> counter = " << counter);

  LOG_S("length(\"temporary\") = "
        << length("temporary")
        << "  <- const& binds to a temporary std::string");
  LOG_S("greeting() = " << greeting());

  const std::string& extended = std::string("lifetime ") + "extended";
  LOG_S("const& to a temporary keeps it alive: " << extended);
}

void take(int& /*value*/) {
  LOG("  take(int&)       <- modifiable lvalue");
}
void take(const int& /*value*/) {
  LOG("  take(const int&) <- const lvalue");
}
void take(int&& /*value*/) {
  LOG("  take(int&&)      <- rvalue (temporary or std::move)");
}

/// Takes an rvalue reference, i.e. it is allowed to "consume" its argument.
std::string consume(std::string&& text) {
  // Inside the function `text` has a name, so it is an lvalue again.
  // std::move marks it as an rvalue once more to move it into the result.
  std::string result = std::move(text);
  return result + " (consumed)";
}

void rvalueReferences() {
  LOG_SECTION("rvalue references and overload resolution");
  int number = 1;
  const int constant = 2;
  take(number);
  take(constant);
  take(3);
  take(std::move(number));  // std::move is only a cast to int&&

  std::string message = "a long message";
  const std::string result = consume(std::move(message));
  LOG_S("result  = " << result);
  LOG_S("message = \"" << message << "\" (moved-from: valid but unspecified)");
}

}  // namespace

LAB_EXAMPLE(
    "Reference",
    "lvalue and rvalue references, const& lifetime extension, std::move") {
  basics();
  parameters();
  rvalueReferences();
}
