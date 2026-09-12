// -----------------------------------------------------------------------------
// String streams (<sstream>)
//
//   std::ostringstream  build a string with <<
//   std::istringstream  parse a string with >> or std::getline
//   std::stringstream   both directions
//
// Pitfalls:
//   - str("") replaces the CONTENT; clear() resets the error FLAGS.
//     Reusing a stream usually needs both.
//   - Since C++17 std::to_string / std::stoi or std::from_chars are simpler
//     and faster for single number conversions.
//
// Reference: https://en.cppreference.com/w/cpp/io/basic_stringstream
// -----------------------------------------------------------------------------

#include <sstream>
#include <string>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void building() {
  LOG_SECTION("Building strings with std::ostringstream");
  std::ostringstream out;
  out << "sensor " << 7 << " reads " << 21.5 << " C";
  LOG_S("result: \"" << out.str() << "\"");
}

void parsing() {
  LOG_SECTION("Parsing with std::istringstream");
  std::istringstream in("0x1F 42 3.14 hello");
  int hex_value = 0;
  int number = 0;
  double real = 0.0;
  std::string word;
  in >> std::hex >> hex_value >> std::dec >> number >> real >> word;
  LOG_S("0x1F -> " << hex_value << ", 42 -> " << number << ", 3.14 -> " << real
                   << ", word -> " << word);

  std::istringstream csv("red,green,,blue");
  std::vector<std::string> fields;
  for (std::string field;
       std::getline(csv, field, ',');) {  // ',' as the delimiter
    fields.push_back(field);
  }
  LOG_S("\"red,green,,blue\" split on ',' -> "
        << fields.size() << " fields (one of them empty)");
}

void reuse() {
  LOG_SECTION("Reusing a stream: str() vs clear()");
  std::stringstream stream("12");
  int value = 0;
  stream >> value;  // reads 12 and hits the end: eofbit is set
  LOG_S("read " << value << ", eof() = " << std::boolalpha << stream.eof());

  stream.str("34");  // new content, but the eof flag is still set...
  stream >> value;
  LOG_S("after str(\"34\") only: value still "
        << value << ", fail() = " << std::boolalpha << stream.fail());

  stream.clear();  // ...so reset the flags as well
  stream.str("56");
  stream >> value;
  LOG_S("after clear() + str(\"56\"): value = " << value);
}

void conversions() {
  LOG_SECTION("Number <-> string");
  std::ostringstream out;
  out << 0xFFF << ' ' << 0x001;
  LOG_S("stream: \"" << out.str() << "\", std::to_string(3.5) = \""
                     << std::to_string(3.5)
                     << "\", std::stoi(\"123\") = " << std::stoi("123"));
}

}  // namespace

LAB_EXAMPLE("StringStream",
            "build and parse strings with ostringstream/istringstream") {
  building();
  parsing();
  reuse();
  conversions();
}
