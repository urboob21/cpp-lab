// -----------------------------------------------------------------------------
// std::forward_list (C++11)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - no size(), no back(), no push_back: the minimal linked list
//   - before_begin() and why insert_after / erase_after exist
//   - building a list front to back and reversing it
//   - memory footprint compared with std::list
//   - when a singly linked list is the right data structure
//
// Reference: https://en.cppreference.com/w/cpp/container/forward_list
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "no size(), no back(), no push_back: the minimal linked list",
    "before_begin() and why insert_after / erase_after exist",
    "building a list front to back and reversing it",
    "memory footprint compared with std::list",
    "when a singly linked list is the right data structure",
};

void outline() {
  LOG_SECTION("std::forward_list (C++11) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("ForwardList",
            "std::forward_list: a singly linked list with before_begin and "
            "insert_after",
            lab::kDraft) {
  outline();
}
