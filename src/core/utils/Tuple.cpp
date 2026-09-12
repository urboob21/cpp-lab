// -----------------------------------------------------------------------------
// std::tuple and std::pair (C++11/C++17)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - std::pair and std::tuple: heterogeneous fixed-size collections
//   - std::get<0> and std::get<Type>, and why the index must be a constant
//   - returning several values: tuple + structured bindings vs a small struct
//   - std::tie for assignment and for lexicographic comparison
//   - std::apply and std::make_from_tuple (C++17)
//
// Reference: https://en.cppreference.com/w/cpp/utility/tuple
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "std::pair and std::tuple: heterogeneous fixed-size collections",
    "std::get<0> and std::get<Type>, and why the index must be a constant",
    "returning several values: tuple + structured bindings vs a small struct",
    "std::tie for assignment and for lexicographic comparison",
    "std::apply and std::make_from_tuple (C++17)",
};

void outline() {
  LOG_SECTION("std::tuple and std::pair (C++11/C++17) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Tuple",
            "tuples: make_tuple, get, tie, structured bindings, std::apply and "
            "returning several values",
            lab::kDraft) {
  outline();
}
