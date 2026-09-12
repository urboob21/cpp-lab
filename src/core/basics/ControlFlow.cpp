// -----------------------------------------------------------------------------
// Control flow
//
// Statements that change the order in which code runs:
//   - conditionals : if / else if / else, switch, if with initializer (C++17)
//   - loops        : while, do-while, for, range-based for
//   - jumps        : break, continue, return, goto
//   - exceptions   : throw / try / catch (see core/exception for details)
//   - halts        : std::exit / std::abort end the whole program
//
// Reference: https://www.learncpp.com/cpp-tutorial/control-flow-introduction/
// -----------------------------------------------------------------------------

#include <random>
#include <stdexcept>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// std::rand() is simple but low quality; `<random>` is the modern way.
int randomInt(int min, int max) {
  static std::mt19937 engine{std::random_device{}()};
  std::uniform_int_distribution<int> distribution(min, max);
  return distribution(engine);
}

void conditionals() {
  LOG_SECTION("Conditionals: if / switch");

  const int x = randomInt(-5, 5);
  LOG_S("x = " << x);
  if (x > 0) {
    LOG("x is positive");
  } else if (x < 0) {
    LOG("x is negative");
  } else {
    LOG("x is zero");
  }

  // C++17: if with initializer. `remainder` only exists inside the if/else.
  if (const int remainder = x % 2; remainder == 0) {
    LOG("x is even");
  } else {
    LOG("x is odd");
  }

  const int choice = randomInt(1, 3);
  LOG_S("choice = " << choice);
  switch (choice) {
    case 1:
      LOG("choice is 1");
      break;  // without break, execution "falls through" into the next case
    case 2:
      LOG("choice is 2, falling through on purpose...");
      [[fallthrough]];  // documents that the fall through is intentional
    default:
      LOG("reached the default label");
      break;
  }
}

void loops() {
  LOG_SECTION("Loops");

  int i = 0;
  while (i < 3) {  // checks the condition before every iteration
    LOG_S("while      i = " << i);
    ++i;
  }

  int j = 5;
  do {  // checks the condition after the body, so it runs at least once
    LOG_S("do-while   j = " << j << "  (body ran although j < 2 is false)");
    ++j;
  } while (j < 2);

  for (int k = 0; k < 3; ++k) {  // for (init; condition; update)
    LOG_S("for        k = " << k);
  }

  const int values[] = {10, 20, 30};
  for (const int value : values) {  // range-based for (C++11)
    LOG_S("range-for  value = " << value);
  }
}

void jumps() {
  LOG_SECTION("Jumps: break / continue / goto");

  for (int i = 0; i < 6; ++i) {
    if (i == 2) {
      continue;  // skip the rest of this iteration
    }
    if (i == 4) {
      break;  // leave the loop
    }
    LOG_S("i = " << i << "  (2 is skipped, the loop stops at 4)");
  }

  // goto jumps to a label in the same function. It makes code hard to follow;
  // leaving deeply nested loops is one of the few accepted uses.
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      if (row * col == 2) {
        LOG_S("row * col == 2 at (" << row << ", " << col
                                    << "), leaving both loops");
        goto done;
      }
    }
  }
done:
  LOG("after the nested loops");
}

int square(int n) {
  return n * n;  // return ends the function and hands a value to the caller
}

void exceptionsAndHalts() {
  LOG_SECTION("return, exceptions and halts");
  LOG_S("square(5) = " << square(5));

  try {
    throw std::runtime_error("thrown to jump to the matching catch block");
  } catch (const std::exception& e) {
    LOG_S("caught: " << e.what());
  }

  // std::exit(0) : normal termination, destructors of static objects run.
  // std::abort() : abnormal termination, no cleanup at all.
  // Both would end the whole lab, so they are only mentioned here.
  LOG("std::exit / std::abort would terminate the whole program");
}

}  // namespace

LAB_EXAMPLE("ControlFlow", "if/switch, loops, break/continue/goto, halts") {
  conditionals();
  loops();
  jumps();
  exceptionsAndHalts();
}
