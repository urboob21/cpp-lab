// Global constants as inline variables (preferred since C++17)
//
// Advantages:
//     Usable in constant expressions in every file that includes the header.
//     Only one copy of each variable exists (the linker merges them).
// Downsides:
//     Requires C++17.
//     Changing a value recompiles every file that includes the header.

#ifndef CPPLAB_INLINE_CONSTANTS_H
#define CPPLAB_INLINE_CONSTANTS_H

namespace inline_constants {
inline constexpr double kPi{3.14159};
inline constexpr double kAvogadro{6.0221413e23};
inline constexpr double kMyGravity{
    9.2};  // m/s^2 -- gravity is light on this planet
}  // namespace inline_constants

#endif  // CPPLAB_INLINE_CONSTANTS_H
