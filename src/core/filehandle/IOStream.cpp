// -----------------------------------------------------------------------------
// Standard streams and stream states
//
//   std::cin   standard input  (std::istream)
//   std::cout  standard output (std::ostream, buffered)
//   std::cerr  standard error  (unbuffered: appears immediately)
//   std::clog  standard error  (buffered)
//
// Every stream has state flags: good(), eof(), fail(), bad(). A failed
// extraction sets failbit and all further reads do nothing until clear().
// Validate input with `if (stream >> value)`.
//
// Reference: https://www.learncpp.com/cpp-tutorial/stream-states-and-input-validation/
// -----------------------------------------------------------------------------

#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void standardStreams() {
  LOG_SECTION("cout, cerr and clog");
  std::cout << "[cout] regular output\n";
  std::cerr << "[cerr] errors, unbuffered\n";
  std::clog << "[clog] diagnostics, buffered\n";
  std::cout.flush();
}

void redirectCin() {
  LOG_SECTION("Reading std::cin from a string");
  // Every stream reads through a stream buffer (rdbuf). Swapping the buffer
  // makes std::cin read from a string - handy for tests.
  std::istringstream fake_input("Ada Lovelace\n36\n");
  std::streambuf* original = std::cin.rdbuf(fake_input.rdbuf());

  std::string name;
  int age = 0;
  std::getline(std::cin, name);  // whole line, including the space
  std::cin >> age;

  std::cin.rdbuf(original);  // always restore the original buffer
  LOG_S("read name = \"" << name << "\", age = " << age);
}

void validation() {
  LOG_SECTION("Stream states and input validation");
  std::istringstream input("42 abc 7");
  int value = 0;

  input >> value;
  LOG_S("read 42  -> value = " << value << ", good() = " << std::boolalpha
                               << input.good());

  if (!(input >> value)) {  // "abc" is not a number
    LOG_S("read abc -> fail() = " << std::boolalpha << input.fail()
                                  << ", value = " << value
                                  << " (a failed read stores 0)");
    input.clear();  // reset the flags...
    input.ignore(std::numeric_limits<std::streamsize>::max(),
                 ' ');  // ...skip the bad token
  }

  if (input >> value) {
    LOG_S("read 7   -> value = " << value);
  }
  input >> value;
  LOG_S("read past the end -> eof() = " << std::boolalpha << input.eof()
                                        << ", fail() = " << input.fail());
}

}  // namespace

LAB_EXAMPLE(
    "IOStream",
    "cout/cerr/clog, redirecting cin, stream states and input validation") {
  standardStreams();
  redirectCin();
  validation();
}
