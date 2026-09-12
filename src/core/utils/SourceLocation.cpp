// -----------------------------------------------------------------------------
// std::source_location and std::stacktrace (C++20/C++23)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - std::source_location::current() as a default argument
//   - file_name, line, column and function_name
//   - a logging helper that reports its caller (this is how lab/Logger.h works)
//   - why this replaces __FILE__ / __LINE__ macros
//   - std::stacktrace (C++23) and linking with -lstdc++exp
//
// Reference: https://en.cppreference.com/w/cpp/utility/source_location
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "std::source_location::current() as a default argument",
    "file_name, line, column and function_name",
    "a logging helper that reports its caller (this is how lab/Logger.h works)",
    "why this replaces __FILE__ / __LINE__ macros",
    "std::stacktrace (C++23) and linking with -lstdc++exp",
};

void outline() {
  LOG_SECTION(
      "std::source_location and std::stacktrace (C++20/C++23) - planned "
      "content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("SourceLocation",
            "capture the call site without macros, and print a stack trace",
            lab::kDraft) {
  outline();
}
