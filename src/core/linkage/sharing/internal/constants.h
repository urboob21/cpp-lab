// Global constants as internal variables
//
// Advantages:
//     Works before C++17.
//     Usable in constant expressions in every file that includes the header.
// Downsides:
//     Every file that includes the header gets its OWN copy of each constant.
//     Changing a value recompiles every file that includes the header.

#ifndef CPPLAB_INTERNAL_CONSTANTS_H
#define CPPLAB_INTERNAL_CONSTANTS_H

namespace internal_constants {
// const/constexpr variables at namespace scope have internal linkage by default.
constexpr double kPi{3.14159};
constexpr double kAvogadro{6.0221413e23};
constexpr double kMyGravity{9.2};  // m/s^2 -- gravity is light on this planet
}  // namespace internal_constants

#endif  // CPPLAB_INTERNAL_CONSTANTS_H
