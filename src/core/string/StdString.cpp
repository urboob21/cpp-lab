// -----------------------------------------------------------------------------
// std::string and std::string_view
//
//   std::string       owns and manages a dynamic array of char (always
//                     '\0'-terminated, so c_str() works with C APIs)
//   std::string_view  (C++17) a non-owning view: pointer + length. Cheap to
//                     pass by value, but must not outlive the viewed text.
//
//   - find() returns std::string::npos when nothing is found.
//   - Short strings are usually stored inside the object without allocating
//     (Small String Optimization).
//   - C++20: starts_with / ends_with; std::erase(str, ch).
//
// Reference: https://en.cppreference.com/w/cpp/string/basic_string
// -----------------------------------------------------------------------------

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void logString(std::string_view label, const std::string& text) {
  LOG_S(label << ": \"" << text << "\" (size " << text.size() << ")");
}

void create() {
  LOG_SECTION("Creating strings");
  const std::string literal = "string 1";
  const std::string constructed("string 2");
  const std::string repeated(5, 's');
  const std::string joined =
      "First "
      "Second";  // adjacent literals are joined at compile time
  const std::string raw = R"(C:\folder\file.txt)";  // raw literal: no escaping
  const std::string part(literal, 0, 6);            // substring constructor

  logString("literal    ", literal);
  logString("constructed", constructed);
  logString("repeated   ", repeated);
  logString("joined     ", joined);
  logString("raw        ", raw);
  logString("part       ", part);
}

void modify() {
  LOG_SECTION("Modifying");
  std::string name = "Phong";
  name.append(" Nguyen");  // "Phong Nguyen"
  name.insert(6, "Van ");  // "Phong Van Nguyen"
  logString("append + insert", name);

  name.erase(6, 4);  // erase(position, count) -> "Phong Nguyen"
  logString("erase(6, 4)    ", name);

  name.replace(0, 5, "Mr.");  // "Mr. Nguyen"
  logString("replace(0, 5)  ", name);

  std::erase(name, '.');  // C++20: remove every '.'
  logString("std::erase('.')", name);

  std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
    return static_cast<char>(std::toupper(c));
  });
  logString("toupper        ", name);

  std::string padded = "   Hello World   ";
  padded.erase(0, padded.find_first_not_of(" \t"));  // trim left
  padded.erase(padded.find_last_not_of(" \t") + 1);  // trim right
  logString("trimmed        ", padded);
}

void search() {
  LOG_SECTION("Searching and substrings");
  const std::string text = "PhongNguyen";

  if (const std::size_t position = text.find('N');
      position != std::string::npos) {
    LOG_S("find('N')        -> " << position << ", substr(" << position
                                 << ") = \"" << text.substr(position) << "\"");
  }
  LOG_S("find(\"ong\")      -> " << text.find("ong"));
  LOG_S("rfind('n')       -> " << text.rfind('n') << " (search from the end)");
  LOG_S("find('z') == npos -> " << std::boolalpha
                                << (text.find('z') == std::string::npos));
  LOG_S("starts_with(\"Phong\") = "
        << std::boolalpha << text.starts_with("Phong")
        << ", ends_with(\"yen\") = " << text.ends_with("yen"));
}

void compareAndConvert() {
  LOG_SECTION("Comparing and converting");
  const std::string a = "apple";
  const std::string b = "banana";
  LOG_S(std::boolalpha << "a == \"apple\" " << (a == "apple") << ", a < b "
                       << (a < b) << ", a.compare(b) = " << a.compare(b));

  LOG_S("std::stoi(\"42\") = "
        << std::stoi("42") << ", std::stod(\"3.5\") = " << std::stod("3.5")
        << ", std::to_string(999) = \"" << std::to_string(999) << "\"");
  try {
    [[maybe_unused]] const int parsed = std::stoi("not a number");
  } catch (const std::invalid_argument&) {
    LOG("std::stoi(\"not a number\") throws std::invalid_argument");
  }
}

void split() {
  LOG_SECTION("Splitting on a delimiter");
  std::istringstream stream("a,b,c");
  std::vector<std::string> parts;
  for (std::string item; std::getline(stream, item, ',');) {
    parts.push_back(item);
  }
  LOG_S("\"a,b,c\" -> " << parts.size() << " parts: " << parts[0] << " "
                        << parts[1] << " " << parts[2]);
}

std::size_t countVowels(
    std::string_view text) {  // accepts literals and strings without copies
  return static_cast<std::size_t>(
      std::count_if(text.begin(), text.end(), [](char c) {
        return std::string_view("aeiouAEIOU").find(c) != std::string_view::npos;
      }));
}

void stringView() {
  LOG_SECTION("std::string_view");
  const std::string owned = "Hello, string_view";
  const std::string_view view = owned;  // no copy
  std::string_view word =
      view.substr(0, 5);  // substr of a view is a view, still no copy

  LOG_S("countVowels(owned) = " << countVowels(owned)
                                << ", countVowels(\"literal\") = "
                                << countVowels("literal"));
  LOG_S("view.substr(0, 5) = " << word);
  word.remove_prefix(1);
  LOG_S("after remove_prefix(1): "
        << word << " (the string itself is unchanged: " << owned << ")");

  // Pitfall: std::string_view dangling = std::string("temporary");  // view of a dead string
  const std::string small = "short";
  LOG_S("sizeof(std::string) = "
        << sizeof(std::string) << ", capacity of \"short\" = "
        << small.capacity() << " (stored inline: SSO)");
}

}  // namespace

LAB_EXAMPLE("StdString",
            "std::string create/modify/search/convert, std::string_view") {
  create();
  modify();
  search();
  compareAndConvert();
  split();
  stringView();
}
