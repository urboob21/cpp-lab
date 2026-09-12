// -----------------------------------------------------------------------------
// C strings (<cstring>, <cstdio>, <cstdlib>)
//
// A C string is an array of char terminated by '\0'. The functions below
// trust you to provide big enough buffers - a missing '\0' or a too-small
// buffer is a buffer overflow (undefined behavior, security bugs).
//
//   length   strlen (counts until '\0') vs sizeof (size of the array/pointer)
//   copy     strcpy, strncpy (may NOT add '\0'!), snprintf (always terminates)
//   append   strcat, strncat
//   compare  strcmp, strncmp, memcmp
//   parse    strtok (not thread-safe), strtok_r (POSIX), strcspn
//   convert  atoi/atof (no error reporting), strtol/strtod (report the end)
//
// In C++ prefer std::string and std::string_view; know these for C APIs.
//
// Reference: https://en.cppreference.com/w/cpp/string/byte
// -----------------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void logString(const char* label, const char* text, std::size_t size) {
  LOG_S(label << ": \"" << text << "\" | sizeof = " << size
              << " | strlen = " << std::strlen(text));
}

void create() {
  LOG_SECTION("Creating C strings");
  // A char array holds a modifiable COPY of the literal (on the stack):
  //   0x7ffc...: 't' 'h' 'i' 's' ... '\0'
  char array[] = "this is a char array";
  logString("array", array, sizeof(array));
  array[0] = 'T';  // allowed
  logString("modified", array, sizeof(array));

  // A pointer to a string literal points into read-only memory:
  //   pointer (stack) -> 0x4030...: 't' 'h' 'i' 's' ... '\0' (read-only data)
  const char* pointer = "this is a string literal";
  logString("pointer", pointer,
            sizeof(pointer));  // sizeof gives the POINTER size!
  // pointer[0] = 'T';  // error (and undefined behavior without the const)

  char formatted[50];
  std::snprintf(formatted, sizeof(formatted), "snprintf %s %d", "example", 21);
  logString("snprintf", formatted, sizeof(formatted));
}

void copy() {
  LOG_SECTION("Copying");
  const char source[] = "CopyStr";
  char destination[50];

  std::strcpy(destination, source);  // destination must be large enough
  logString("strcpy", destination, sizeof(destination));

  std::strncpy(destination, "Hello123",
               5);        // copies 5 chars, NO '\0' added here
  destination[5] = '\0';  // so terminate it yourself
  logString("strncpy(5)", destination, sizeof(destination));
}

void concatenate() {
  LOG_SECTION("Concatenating");
  char destination[50] = "";
  std::strcat(destination, "Hello");
  std::strcat(destination, "World");
  std::strcat(destination, " !!");
  logString("strcat", destination, sizeof(destination));

  std::strncat(destination, "1234",
               3);  // appends at most 3 chars, always adds '\0'
  logString("strncat(3)", destination, sizeof(destination));
}

void compare() {
  LOG_SECTION("Comparing");
  const char shorter[] = "abc";
  const char longer[] = "abcde";
  // Result < 0, 0 or > 0 - never compare C strings with == (that compares addresses).
  LOG_S("strcmp(\"abc\", \"abc\")      = " << std::strcmp(shorter, "abc"));
  LOG_S("strcmp(\"abc\", \"abcde\")    = " << std::strcmp(shorter, longer)
                                           << " (< 0)");
  LOG_S(
      "strncmp(\"abc\", \"abcde\", 3) = " << std::strncmp(shorter, longer, 3));
  LOG_S("memcmp over 4 bytes          = "
        << std::memcmp(shorter, longer, sizeof(shorter)) << " ('\\0' vs 'd')");
}

void parse() {
  LOG_SECTION("Tokenizing");
  char list[] =
      "A,B,C,D,";  // strtok MODIFIES the string: it must not be a literal
  for (const char* token = std::strtok(list, ","); token != nullptr;
       token = std::strtok(nullptr, ",")) {
    LOG_S("strtok token: " << token);
  }

  LOG("strtok keeps hidden global state - interleaving two strings breaks it:");
  char first[] = "a,b,c";
  char second[] = "1,2,3";
  const char* token1 = std::strtok(first, ",");
  const char* token2 = std::strtok(second, ",");  // resets the hidden state
  LOG_S("  first token of each: " << token1 << ", " << token2);
  token1 = std::strtok(nullptr, ",");
  LOG_S("  'next token of first' is really from second: " << token1);

  char reentrant[] = "one,two,three";
  char* save = nullptr;  // strtok_r keeps the state in `save` instead
  for (const char* token = strtok_r(reentrant, ",", &save); token != nullptr;
       token = strtok_r(nullptr, ",", &save)) {
    LOG_S("strtok_r token: " << token);
  }

  const char sample[] = "hello123world";
  const std::size_t index = std::strcspn(sample, "0123456789");
  LOG_S("strcspn: first digit of \"" << sample << "\" is '" << sample[index]
                                     << "' at index " << index);
}

void convert() {
  LOG_SECTION("Number conversions");
  LOG_S("atoi(\"100\")          = " << std::atoi("100"));
  LOG_S("atoi(\"abc\")          = " << std::atoi("abc")
                                    << " (errors look like 0!)");

  const char text[] = "100.1234__123";
  char* end = nullptr;
  const double value =
      std::strtod(text, &end);  // reports where parsing stopped
  LOG_S("strtod(\"" << text << "\") = " << value << ", remaining \"" << end
                    << "\"");

  const long hex = std::strtol("ff", nullptr, 16);
  LOG_S("strtol(\"ff\", 16)     = " << hex);
}

}  // namespace

LAB_EXAMPLE(
    "CString",
    "C strings: strlen vs sizeof, copy, concatenate, compare, strtok, strtod") {
  create();
  copy();
  concatenate();
  compare();
  parse();
  convert();
}
