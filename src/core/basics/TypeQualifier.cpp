// -----------------------------------------------------------------------------
// const, constexpr, consteval, constinit, mutable, volatile
//
//   const      : cannot be modified after initialization (value may come at run time)
//   constexpr  : value known at compile time; constexpr functions MAY run there
//   consteval  : (C++20) function that MUST run at compile time
//   constinit  : (C++20) static variable that MUST be initialized at compile time
//   mutable    : member that may change even inside a const member function
//   volatile   : value may change outside the program (hardware register)
//
// Notes:
//   - A constexpr function is implicitly inline.
//   - A constexpr static data member is implicitly an inline variable (C++17).
//
// Reference: https://en.cppreference.com/w/cpp/language/constexpr
// -----------------------------------------------------------------------------

#include <cstddef>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

constexpr int square(int x) {
  return x * x;
}

consteval int cube(int x) {
  return x * x * x;
}

// Initialized at compile time (no static initialization order problems), but
// the variable itself is not const and can change at run time.
constinit int g_counter = square(3);

// A macro is plain text replacement: no type, no scope, no debugger symbol.
#define LAB_GRAVITY_MACRO 9.8

class Sensor {
 public:
  int read() const {
    ++reads_;  // allowed because reads_ is mutable
    return 42;
  }
  int reads() const { return reads_; }

 private:
  mutable int reads_{
      0};  // bookkeeping that does not change the "logical" state
};

void constVariables() {
  LOG_SECTION("const");

  // The value is only known at run time, but it can never change afterwards.
  const std::size_t length = std::string("hello").size();
  // length = 3;  // error: assignment of read-only variable
  LOG_S("const std::size_t length = " << length);
}

void compileTimeConstants() {
  LOG_SECTION("constexpr and consteval");

  constexpr double kGravity = 9.8;  // typed compile-time constant
  constexpr int kArea = square(4);  // constexpr function at compile time
  constexpr int kVolume = cube(3);  // consteval: always at compile time
  static_assert(kArea == 16, "checked by the compiler, free at run time");

  const int side = static_cast<int>(std::string("four").size());
  const int runtime_area = square(side);  // the same function at run time
  // constexpr int bad = square(side);    // error: side is not a constant expression
  // const int also_bad = cube(side);     // error: consteval needs a constant argument

  const int buffer[square(2)]{};  // constant expressions can size an array
  LOG_S("kGravity = " << kGravity << ", kArea = " << kArea
                      << ", kVolume = " << kVolume);
  LOG_S("square(side) at run time = " << runtime_area);
  LOG_S("int buffer[square(2)] has " << sizeof(buffer) / sizeof(buffer[0])
                                     << " elements");
  LOG_S("LAB_GRAVITY_MACRO = " << LAB_GRAVITY_MACRO
                               << " (prefer constexpr over #define)");

  if constexpr (sizeof(void*) == 8) {  // the other branch is discarded
    LOG("64-bit build (decided at compile time with if constexpr)");
  } else {
    LOG("not a 64-bit build (decided at compile time with if constexpr)");
  }
}

void constinitVariables() {
  LOG_SECTION("constinit (C++20)");
  LOG_S("g_counter starts at square(3) = " << g_counter);
  ++g_counter;
  LOG_S("after ++g_counter            = " << g_counter);
}

void mutableAndVolatile() {
  LOG_SECTION("mutable and volatile");

  const Sensor sensor;
  sensor.read();
  sensor.read();
  LOG_S("const Sensor was read " << sensor.reads()
                                 << " times (mutable counter)");

  LOG("volatile: reads are never optimized away because the value may change");
  LOG("outside the program, e.g. a memory-mapped register (see "
      "TypeConversions).");
}

}  // namespace

#undef LAB_GRAVITY_MACRO

LAB_EXAMPLE("TypeQualifier",
            "const, constexpr, consteval, constinit, mutable, volatile") {
  constVariables();
  compileTimeConstants();
  constinitVariables();
  mutableAndVolatile();
}
