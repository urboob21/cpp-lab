// -----------------------------------------------------------------------------
// Sharing global constants across multiple files
//
//   1. inline constexpr in a header (C++17)   <- preferred
//        one definition shared by every file, usable in constant expressions
//   2. constexpr in a header (internal linkage)
//        simple, but every .cpp that includes it gets its own copy
//   3. extern const declared in a header, defined in one .cpp
//        one copy, but not usable in constant expressions in other files and
//        declaration/definition must be kept in sync
//
// This example proves the "copies" claim by comparing the ADDRESS of the same
// constant as seen from this file and from external/constants.cpp.
//
// Reference: https://www.learncpp.com/cpp-tutorial/sharing-global-constants-across-multiple-files-using-inline-variables/
// -----------------------------------------------------------------------------

#include "external/constants.h"
#include "inline/constants.h"
#include "internal/constants.h"
#include "lab/Example.h"
#include "lab/Logger.h"

// Defined in external/constants.cpp: addresses of the constants as that file sees them.
namespace sharing_demo {
const double* internalAvogadroAddress();
const double* inlineAvogadroAddress();
}  // namespace sharing_demo

namespace {

void compare(const char* label, const double* here, const double* there) {
  LOG_S(label << "this file " << here << ", constants.cpp " << there
              << (here == there ? "  -> the SAME object"
                                : "  -> two different copies"));
}

void run() {
  LOG_SECTION("Values");
  LOG_S("inline_constants::kAvogadro   = " << inline_constants::kAvogadro);
  LOG_S("internal_constants::kAvogadro = " << internal_constants::kAvogadro);
  LOG_S("external_constants::kAvogadro = " << external_constants::kAvogadro);

  constexpr double kMoles =
      2.0 * inline_constants::kAvogadro;  // OK: constant expression
  // constexpr double bad = 2.0 * external_constants::kAvogadro; // error: value not known here
  LOG_S("2 mol = " << kMoles << " particles (computed at compile time)");

  LOG_SECTION("Addresses");
  compare("inline   : ", &inline_constants::kAvogadro,
          sharing_demo::inlineAvogadroAddress());
  compare("internal : ", &internal_constants::kAvogadro,
          sharing_demo::internalAvogadroAddress());
}

}  // namespace

LAB_EXAMPLE(
    "Sharing",
    "inline, internal and extern global constants shared between files") {
  run();
}
