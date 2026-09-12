// -----------------------------------------------------------------------------
// std::print and std::println (C++23)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - std::println("{} scored {}", name, points)
//   - printing to a file stream or to stderr
//   - std::format_to and std::format_to_n into an existing buffer
//   - why print is faster and safer than printf, and shorter than cout
//   - compiler support: GCC 14 / Clang 18; the std::format fallback
//
// Reference: https://en.cppreference.com/w/cpp/io/print
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "std::println(\"{} scored {}\", name, points)",
    "printing to a file stream or to stderr",
    "std::format_to and std::format_to_n into an existing buffer",
    "why print is faster and safer than printf, and shorter than cout",
    "compiler support: GCC 14 / Clang 18; the std::format fallback",
};

void outline() {
  LOG_SECTION("std::print and std::println (C++23) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Print",
            "formatted output without iostreams, and how it compares with "
            "printf and cout",
            lab::kDraft) {
  outline();
}
