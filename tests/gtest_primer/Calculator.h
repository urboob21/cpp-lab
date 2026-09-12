#pragma once

/// A deliberately tiny class to demonstrate GoogleTest (see README.md in tests/).
class Calculator {
 public:
  bool add(int lhs, int rhs) {
    last_result_ = lhs + rhs;
    return true;
  }

  bool multiply(int lhs, int rhs) {
    last_result_ = lhs * rhs;
    return true;
  }

  /// Returns false (and stores 0) for a division by zero.
  bool divide(int dividend, int divisor) {
    if (divisor == 0) {
      last_result_ = 0;
      return false;
    }
    last_result_ = dividend / divisor;
    return true;
  }

  int lastResult() const { return last_result_; }

 private:
  int last_result_{0};
};
