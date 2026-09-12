// -----------------------------------------------------------------------------
// Fundamental types
//
//   bool, char types, integers, floating point, void, std::nullptr_t
//
//   - The standard only guarantees MINIMUM sizes (e.g. int >= 16 bits, long
//     >= 32 bits). Use <cstdint> (std::int32_t, std::uint8_t, ...) when the
//     exact width matters.
//   - std::numeric_limits<T> describes the range of a type.
//   - Floating-point numbers are approximations: 0.1 + 0.2 != 0.3.
//
// Reference: https://en.cppreference.com/w/cpp/language/types
// -----------------------------------------------------------------------------

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <limits>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void booleans() {
  LOG_SECTION("bool");
  const bool is_ready = true;
  LOG_S("is_ready = " << is_ready << " (printed as an int), with boolalpha: "
                      << std::boolalpha << is_ready);
  LOG_S("sizeof(bool) = " << sizeof(bool));
}

void characters() {
  LOG_SECTION("Character types");
  const char c = 'A';
  const unsigned char byte = 200;
  const wchar_t wide = L'Ω';     // Greek capital omega
  const char8_t utf8 = u8'A';    // C++20
  const char16_t utf16 = u'ß';   // German sharp s
  const char32_t utf32 = U'中';  // CJK "middle"

  LOG_S("char           'A' = " << c << " (code " << static_cast<int>(c)
                                << ")");
  LOG_S("unsigned char  200 printed as a number: " << static_cast<int>(byte));
  // Mixing std::wcout with std::cout on the same stream is not allowed,
  // so the wide characters are printed as code points.
  LOG_S("wchar_t   U+" << std::hex << static_cast<unsigned long>(wide)
                       << std::dec << ", sizeof = " << sizeof(wchar_t));
  LOG_S("char8_t   U+" << std::hex << static_cast<unsigned>(utf8) << std::dec
                       << ", sizeof = " << sizeof(char8_t));
  LOG_S("char16_t  U+" << std::hex << static_cast<unsigned>(utf16) << std::dec
                       << ", sizeof = " << sizeof(char16_t));
  LOG_S("char32_t  U+" << std::hex << static_cast<unsigned long>(utf32)
                       << std::dec << ", sizeof = " << sizeof(char32_t));
}

template <typename T>
void describeInteger(const char* name) {
  LOG_S(std::left << std::setw(14) << name << "sizeof = " << sizeof(T)
                  << "  range [" << +std::numeric_limits<T>::min() << ", "
                  << +std::numeric_limits<T>::max() << "]");
}

void integers() {
  LOG_SECTION("Integer types");
  describeInteger<short>("short");
  describeInteger<int>("int");
  describeInteger<unsigned int>("unsigned int");
  describeInteger<long>("long");
  describeInteger<long long>("long long");

  LOG("Fixed-width types from <cstdint>:");
  describeInteger<std::int8_t>(
      "std::int8_t");  // unary + prints numbers, not chars
  describeInteger<std::uint16_t>("std::uint16_t");
  describeInteger<std::int32_t>("std::int32_t");
  describeInteger<std::uint64_t>("std::uint64_t");
  LOG_S("std::size_t   sizeof = " << sizeof(std::size_t)
                                  << "  (type of sizeof and indices)");

  const long long big = 9'876'543'210LL;  // suffixes: U, L, LL, UL, ULL
  LOG_S("9'876'543'210LL = " << big);
}

void floatingPoint() {
  LOG_SECTION("Floating-point types");
  const float f = 3.14F;
  const double d = 2.718281828;
  const long double ld = 1.6180339887L;
  LOG_S("float        " << f << "  sizeof = " << sizeof(float) << ", ~"
                        << std::numeric_limits<float>::digits10 << " digits");
  LOG_S("double       " << d << "  sizeof = " << sizeof(double) << ", ~"
                        << std::numeric_limits<double>::digits10 << " digits");
  LOG_S("long double  " << ld << "  sizeof = " << sizeof(long double));

  const double sum = 0.1 + 0.2;
  LOG_S(std::setprecision(17) << "0.1 + 0.2 = " << sum << " -> == 0.3 is "
                              << std::boolalpha << (sum == 0.3));
  LOG_S("compare with a tolerance instead: |sum - 0.3| < 1e-9 is "
        << std::boolalpha << (std::abs(sum - 0.3) < 1e-9));
}

void voidAndNullptr() {
  LOG_SECTION("void and std::nullptr_t");
  LOG("void: \"no value\" - used as a return type and for void* pointers");
  const std::nullptr_t null = nullptr;
  const int* pointer = null;
  LOG_S("nullptr converts to any pointer type: pointer == nullptr is "
        << std::boolalpha << (pointer == nullptr));
}

}  // namespace

LAB_EXAMPLE(
    "Fundamental",
    "bool, character, integer and floating-point types, sizes and limits") {
  booleans();
  characters();
  integers();
  floatingPoint();
  voidAndNullptr();
}
