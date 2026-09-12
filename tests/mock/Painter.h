#pragma once

#include "Turtle.h"

/// Code under test: depends only on the Turtle interface, so tests can pass a
/// mock or a fake instead of a real drawing turtle.
class Painter {
 public:
  explicit Painter(Turtle& turtle) : turtle_(turtle) {}

  void DrawSquare(int size) const {
    turtle_.PenDown();
    for (int i = 0; i < 4; ++i) {
      turtle_.Forward(size);
      turtle_.Turn(90);
    }
    turtle_.PenUp();
  }

 private:
  Turtle& turtle_;
};
