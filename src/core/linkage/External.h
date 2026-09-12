#pragma once

// Declarations shared between External.cpp (definitions) and Linkage.cpp (use).

namespace external {

// A header must only DECLARE non-inline variables (extern) - defining them here
// would create one definition per including file and break the linker.
extern int non_static_var;
extern const int kExternConstVar;
void nonStaticFnc();

namespace name {
extern int namespace_var;
}  // namespace name

// Exceptions that may be defined in a header:
inline const int kInlineConstVar{
    100};  // inline variable (C++17): one shared definition
constexpr int kConstexprVar{
    500};  // constexpr implies internal linkage at namespace scope

}  // namespace external
