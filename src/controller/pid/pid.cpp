/**
 * Copyright 2019 Bradley J. Snyder <snyder.bradleyj@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Modified for cpp-lab: namespace, std::unique_ptr pimpl, input validation.
 */

#include "pid.h"

#include <algorithm>
#include <stdexcept>

namespace controller {

class PIDImpl {
 public:
  PIDImpl(double dt, double max, double min, double kp, double kd, double ki)
      : dt_{dt}, max_{max}, min_{min}, kp_{kp}, kd_{kd}, ki_{ki} {
    if (dt_ <= 0.0) {
      throw std::invalid_argument("PID: dt must be greater than zero");
    }
    if (min_ > max_) {
      throw std::invalid_argument("PID: min must not be greater than max");
    }
  }

  double calculate(double setpoint, double process_value) {
    const double error = setpoint - process_value;

    // Proportional term: reacts to the current error.
    const double proportional = kp_ * error;

    // Integral term: accumulates past errors and removes steady-state offset.
    integral_ += error * dt_;
    const double integral = ki_ * integral_;

    // Derivative term: reacts to how fast the error changes (damping).
    const double derivative = kd_ * (error - previous_error_) / dt_;

    previous_error_ = error;

    // Restrict the output to [min, max].
    return std::clamp(proportional + integral + derivative, min_, max_);
  }

 private:
  double dt_;
  double max_;
  double min_;
  double kp_;
  double kd_;
  double ki_;
  double previous_error_{0.0};
  double integral_{0.0};
};

PID::PID(double dt, double max, double min, double kp, double kd, double ki)
    : impl_{std::make_unique<PIDImpl>(dt, max, min, kp, kd, ki)} {}

// Defined here, where PIDImpl is a complete type: std::unique_ptr needs that to
// delete it. This is why a pimpl class declares its destructor in the header.
PID::~PID() = default;
PID::PID(PID&&) noexcept = default;
PID& PID::operator=(PID&&) noexcept = default;

double PID::calculate(double setpoint, double process_value) {
  return impl_->calculate(setpoint, process_value);
}

}  // namespace controller
