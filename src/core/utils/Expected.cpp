// -----------------------------------------------------------------------------
// std::expected (C++23)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - expected<T, E> carries either a result or an error, unlike optional
//   - has_value, value, error, value_or and std::unexpected
//   - chaining with and_then, transform, or_else and transform_error
//   - expected vs exceptions: cost, visibility in the signature, nodiscard
//   - compiler support: needs GCC 13 / Clang 16 or newer
//
// Reference: https://en.cppreference.com/w/cpp/utility/expected
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "expected<T, E> carries either a result or an error, unlike optional",
    "has_value, value, error, value_or and std::unexpected",
    "chaining with and_then, transform, or_else and transform_error",
    "expected vs exceptions: cost, visibility in the signature, nodiscard",
    "compiler support: needs GCC 13 / Clang 16 or newer",
};

void outline() {
  LOG_SECTION("std::expected (C++23) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Expected",
            "errors as values: expected<T, E>, and_then/transform, compared "
            "with optional and exceptions",
            lab::kDraft) {
  outline();
}
