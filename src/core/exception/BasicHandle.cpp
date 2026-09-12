// -----------------------------------------------------------------------------
// Exception handling basics
//
//   throw   : report an error; the stack unwinds until a matching catch
//   try     : a block whose exceptions you want to handle
//   catch   : handler, tried in order - put the most derived types first
//
// Guidelines:
//   - Throw by value, catch by (const) reference: avoids copies and slicing.
//   - Derive custom exceptions from std::exception (usually std::runtime_error).
//   - `throw;` rethrows the CURRENT exception unchanged; `throw e;` would
//     throw a copy of the caught (possibly sliced) object.
//   - std::throw_with_nested adds context without losing the original error.
//
// Reference: https://en.cppreference.com/w/cpp/language/exceptions
// -----------------------------------------------------------------------------

#include <exception>
#include <stdexcept>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// A custom exception carrying extra data.
class ConfigError : public std::runtime_error {
 public:
  ConfigError(const std::string& key, const std::string& message)
      : std::runtime_error("config key '" + key + "': " + message), key_{key} {}
  const std::string& key() const { return key_; }

 private:
  std::string key_;
};

int parsePort(const std::string& text) {
  const int port =
      std::stoi(text);  // throws std::invalid_argument / out_of_range
  if (port <= 0 || port > 65535) {
    throw ConfigError("port", "value " + text + " is out of range");
  }
  return port;
}

void catchOrder() {
  LOG_SECTION("Catch clauses are tried in order");
  for (const std::string input : {"8080", "99999", "abc"}) {
    try {
      LOG_S("parsePort(\"" << input << "\") = " << parsePort(input));
    } catch (const ConfigError& e) {  // most specific first
      LOG_S("ConfigError for key '" << e.key() << "': " << e.what());
    } catch (const std::invalid_argument& e) {
      LOG_S("std::invalid_argument: " << e.what());
    } catch (
        const std::exception& e) {  // everything derived from std::exception
      LOG_S("std::exception: " << e.what());
    } catch (...) {  // anything else, even `throw 42;`
      LOG("unknown exception");
    }
  }
}

void rethrow() {
  LOG_SECTION("Rethrowing with throw;");
  try {
    try {
      throw ConfigError("timeout", "missing");
    } catch (const std::exception& e) {
      LOG_S("inner handler logs and rethrows: " << e.what());
      throw;  // rethrows the original ConfigError, not a std::exception copy
    }
  } catch (const ConfigError& e) {
    LOG_S("outer handler still sees a ConfigError (key '" << e.key() << "')");
  }
}

void loadConfiguration() {
  try {
    parsePort("0");
  } catch (...) {
    std::throw_with_nested(std::runtime_error("failed to load configuration"));
  }
}

void printNested(const std::exception& e, int depth = 0) {
  LOG_S(std::string(static_cast<std::size_t>(depth) * 2, ' ')
        << "- " << e.what());
  try {
    std::rethrow_if_nested(e);
  } catch (const std::exception& nested) {
    printNested(nested, depth + 1);
  }
}

void nestedExceptions() {
  LOG_SECTION("Nested exceptions keep the cause");
  try {
    loadConfiguration();
  } catch (const std::exception& e) {
    printNested(e);
  }
}

}  // namespace

LAB_EXAMPLE(
    "BasicHandle",
    "try/catch/throw, custom exceptions, catch order, rethrow, nesting") {
  catchOrder();
  rethrow();
  nestedExceptions();
}
