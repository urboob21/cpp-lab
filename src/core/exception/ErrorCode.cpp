// -----------------------------------------------------------------------------
// Error codes and exception safety (C++11)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - error_code + error_condition: a portable errno
//   - std::system_error: an exception that carries an error_code
//   - the filesystem API offers both: throwing and error_code overloads
//   - the guarantees: nothrow, strong, basic - and what each promises
//   - copy-and-swap as a recipe for the strong guarantee
//
// Reference: https://en.cppreference.com/w/cpp/error/error_code
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "error_code + error_condition: a portable errno",
    "std::system_error: an exception that carries an error_code",
    "the filesystem API offers both: throwing and error_code overloads",
    "the guarantees: nothrow, strong, basic - and what each promises",
    "copy-and-swap as a recipe for the strong guarantee",
};

void outline() {
  LOG_SECTION("Error codes and exception safety (C++11) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("ErrorCode",
            "std::error_code, std::system_error, errno, and the exception "
            "safety guarantees",
            lab::kDraft) {
  outline();
}
