// -----------------------------------------------------------------------------
// Template specialization (C++98)
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop `lab::kDraft` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - full specialization of a class template: Serializer<bool>
//   - partial specialization: Serializer<T*>, Serializer<std::vector<T>>
//   - function templates cannot be partially specialized - overload instead
//   - tag dispatch as the older alternative to if constexpr
//   - specializing std::hash for a user-defined type
//
// Reference: https://en.cppreference.com/w/cpp/language/template_specialization
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 5> kPlanned{
    "full specialization of a class template: Serializer<bool>",
    "partial specialization: Serializer<T*>, Serializer<std::vector<T>>",
    "function templates cannot be partially specialized - overload instead",
    "tag dispatch as the older alternative to if constexpr",
    "specializing std::hash for a user-defined type",
};

void outline() {
  LOG_SECTION("Template specialization (C++98) - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("Specialization",
            "full and partial specialization, and why function templates use "
            "overloading instead",
            lab::kDraft) {
  outline();
}
