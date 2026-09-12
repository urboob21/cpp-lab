// -----------------------------------------------------------------------------
// Class templates (C++98/C++17)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - a small Stack<T> or Matrix<T>: where member functions must live
//   - default template arguments and template template parameters
//   - class template argument deduction: std::vector v{1, 2, 3} (C++17)
//   - writing a deduction guide when CTAD picks the wrong type
//   - member function templates and why they cannot be virtual
//
// Reference: https://en.cppreference.com/w/cpp/language/class_template
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "a small Stack<T> or Matrix<T>: where member functions must live",
    "default template arguments and template template parameters",
    "class template argument deduction: std::vector v{1, 2, 3} (C++17)",
    "writing a deduction guide when CTAD picks the wrong type",
    "member function templates and why they cannot be virtual",
};

void outline() {
  LOG_SECTION("Class templates (C++98/C++17) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("ClassTemplate",
            "class templates: member functions, default arguments, CTAD and "
            "deduction guides",
            lab::kDraft) {
  outline();
}
