// Definitions of the entities with EXTERNAL linkage used by Linkage.cpp.
// They must be defined in the same namespace as their declarations in
// External.h, otherwise the linker cannot match them.

#include "External.h"

#include "lab/Logger.h"

namespace external {

// 1. Non-static global variables and functions have external linkage.
int non_static_var{1};

void nonStaticFnc() {
  LOG("external::nonStaticFnc() defined in External.cpp");
}

// 2. const variables have INTERNAL linkage by default; `extern` makes them external.
extern const int kExternConstVar{200};

// 3. Variables inside a named namespace behave like globals.
namespace name {
int namespace_var = 123;
}  // namespace name

}  // namespace external
