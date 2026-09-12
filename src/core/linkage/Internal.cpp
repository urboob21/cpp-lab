// Entities with INTERNAL linkage: they are visible only inside this .cpp file
// (this "translation unit"). Another file may even define entities with the
// same names without any conflict.

#include "lab/Logger.h"

namespace internal {
namespace {

// 3. Everything in an anonymous namespace has internal linkage (preferred in C++).
int ns_internal{300};

}  // namespace

// 1. `static` at namespace scope gives internal linkage (the C way).
static int g_internal{42};
static void func_internal() {
  LOG("internal::func_internal() - only callable inside Internal.cpp");
}

// 2. const and constexpr variables at namespace scope are internal by default.
const int c_internal{100};
constexpr int ce_internal{200};

/// External linkage on purpose: the one door into this file (see Linkage.cpp).
void printInternals() {
  func_internal();
  LOG_S("g_internal = " << g_internal << ", c_internal = " << c_internal
                        << ", ce_internal = " << ce_internal
                        << ", ns_internal = " << ns_internal);
}

}  // namespace internal
