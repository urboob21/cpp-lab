#pragma once

#include <utility>
#include <vector>
#include "Turtle.h"

class FakeTurtle : public Turtle {
 public:
  FakeTurtle() = default;

  void PenUp() override { penDown_ = false; }

  void PenDown() override { penDown_ = true; }

  void Forward(int distance) override {
    if (penDown_) {
      path_.emplace_back(x_, y_);
    }

    switch (direction_) {
      case 0:
        y_ += distance;
        break;  // north
      case 90:
        x_ += distance;
        break;  // east
      case 180:
        y_ -= distance;
        break;  // south
      case 270:
        x_ -= distance;
        break;  // west
      default:
        break;  // other angles are not used by the tests
    }

    if (penDown_) {
      path_.emplace_back(x_, y_);
    }
  }

  void Turn(int degrees) override { direction_ = (direction_ + degrees) % 360; }

  void GoTo(int x, int y) override {
    x_ = x;
    y_ = y;
  }

  int GetX() const override { return x_; }
  int GetY() const override { return y_; }

  const std::vector<std::pair<int, int>>& Path() const { return path_; }

 private:
  int x_{0};
  int y_{0};
  int direction_{0};  // 0=north, 90=east, ...
  bool penDown_{false};

  std::vector<std::pair<int, int>> path_;
};
