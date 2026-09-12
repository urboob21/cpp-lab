// Global constants as external variables
//
// Advantages:
//     Works before C++17.
//     Only one copy of each variable exists.
//     Changing a value only requires recompiling constants.cpp.
// Downsides:
//     Declarations (here) and definitions (constants.cpp) must be kept in sync.
//     Not usable in constant expressions outside constants.cpp.

#ifndef CPPLAB_EXTERNAL_CONSTANTS_H
#define CPPLAB_EXTERNAL_CONSTANTS_H

namespace external_constants {
// The definitions live in a namespace, so the declarations must too.
// A variable cannot be forward declared as constexpr, only as (run-time) const.
extern const double kPi;
extern const double kAvogadro;
extern const double kMyGravity;
}  // namespace external_constants

#endif  // CPPLAB_EXTERNAL_CONSTANTS_H
