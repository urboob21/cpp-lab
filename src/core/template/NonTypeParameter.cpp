// -----------------------------------------------------------------------------
// Non-type template parameters (C++11/C++20)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - template<std::size_t N> class FixedBuffer: the size is part of the type
//   - why std::array<int, 4> and std::array<int, 5> are unrelated types
//   - auto as a non-type parameter (C++17)
//   - structural types as NTTPs, including a compile-time string (C++20)
//   - NTTPs vs constructor arguments: what moves work to compile time
//
// Reference: https://en.cppreference.com/w/cpp/language/template_parameters
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "template<std::size_t N> class FixedBuffer: the size is part of the type",
    "why std::array<int, 4> and std::array<int, 5> are unrelated types",
    "auto as a non-type parameter (C++17)",
    "structural types as NTTPs, including a compile-time string (C++20)",
    "NTTPs vs constructor arguments: what moves work to compile time",
};

void outline() {
  LOG_SECTION("Non-type template parameters (C++11/C++20) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE(
    "NonTypeParameter",
    "non-type template parameters, auto NTTPs and compile-time sized types",
    lab::kDraft) {
  outline();
}
