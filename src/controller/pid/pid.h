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

#pragma once

#include <memory>

namespace controller {

class PIDImpl;

/// Discrete PID controller.
///
///   error  = setpoint - process_value
///   output = Kp * error + Ki * sum(error * dt) + Kd * (error - previous_error) / dt
///
/// The output is clamped to [min, max]. The implementation is hidden behind a
/// pointer ("pimpl" idiom), so changing it does not recompile the users.
class PID {
 public:
  /// @param dt  loop interval time (must be > 0)
  /// @param max maximum value of the manipulated variable
  /// @param min minimum value of the manipulated variable (must be <= max)
  /// @param kp  proportional gain
  /// @param kd  derivative gain
  /// @param ki  integral gain
  PID(double dt, double max, double min, double kp, double kd, double ki);
  ~PID();

  PID(const PID&) = delete;
  PID& operator=(const PID&) = delete;
  PID(PID&&) noexcept;
  PID& operator=(PID&&) noexcept;

  /// Returns the manipulated variable for a setpoint and the current process value.
  double calculate(double setpoint, double process_value);

 private:
  std::unique_ptr<PIDImpl> impl_;
};

}  // namespace controller
