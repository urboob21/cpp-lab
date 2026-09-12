// -----------------------------------------------------------------------------
// Linkage: can a name be used from another .cpp file?
//
//   no linkage       local variables - only inside their block
//   internal linkage static globals, const/constexpr globals, anonymous
//                    namespaces - only inside their translation unit (.cpp)
//   external linkage non-static globals and functions, extern variables,
//                    inline variables - shared by the whole program
//
// Each .cpp is compiled separately; the LINKER then connects uses of external
// names to their single definition (the One Definition Rule).
//
// Files: External.h / External.cpp (external names), Internal.cpp (internal
// names), sharing/ (three ways to share global constants).
//
// Reference: https://www.learncpp.com/cpp-tutorial/internal-linkage/
// -----------------------------------------------------------------------------

#include "External.h"
#include "lab/Example.h"
#include "lab/Logger.h"

// Declaration of the only externally visible function of Internal.cpp.
namespace internal {
void printInternals();
}  // namespace internal

namespace {

void internalLinkage() {
  LOG_SECTION("Internal linkage");
  // Declaring the internal names from Internal.cpp here would compile, but the
  // linker would fail with "undefined reference" as soon as they are used:
  //   namespace internal { extern int g_internal; void func_internal(); }
  //   internal::func_internal();  // link error
  LOG("internal names cannot be reached from this file; Internal.cpp prints "
      "them:");
  internal::printInternals();
}

void externalLinkage() {
  LOG_SECTION("External linkage");
  LOG_S("non_static_var        = " << external::non_static_var);
  LOG_S("kExternConstVar       = " << external::kExternConstVar);
  LOG_S("name::namespace_var   = " << external::name::namespace_var);
  LOG_S("kInlineConstVar       = " << external::kInlineConstVar
                                   << " (inline variable in the header)");
  LOG_S("kConstexprVar         = " << external::kConstexprVar
                                   << " (constexpr in the header)");
  external::nonStaticFnc();
}

}  // namespace

LAB_EXAMPLE("Linkage",
            "internal vs external linkage across translation units") {
  internalLinkage();
  externalLinkage();
}
