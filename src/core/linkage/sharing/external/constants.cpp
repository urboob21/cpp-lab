#include "constants.h"

#include "../inline/constants.h"
#include "../internal/constants.h"

namespace external_constants {
// The single definitions. `extern` keeps external linkage (const alone would
// make them internal).
extern constexpr double kPi{3.14159};
extern constexpr double kAvogadro{6.0221413e23};
extern constexpr double kMyGravity{
    9.2};  // m/s^2 -- gravity is light on this planet
}  // namespace external_constants

namespace sharing_demo {

const double* internalAvogadroAddress() {
  return &internal_constants::kAvogadro;  // this file's own copy
}

const double* inlineAvogadroAddress() {
  return &inline_constants::kAvogadro;  // the one shared object
}

}  // namespace sharing_demo
