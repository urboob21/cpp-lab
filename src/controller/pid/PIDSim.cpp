// -----------------------------------------------------------------------------
// PID controller simulation
//
// A PID controller drives a process value toward a setpoint by combining
// three reactions to the error e = setpoint - value:
//
//   P = Kp * e          present : the bigger the error, the stronger the push
//   I = Ki * sum(e*dt)  past    : removes the remaining steady-state offset
//   D = Kd * de/dt      future  : brakes when the error shrinks quickly
//
// Here the controller output is simply added to the value each step - a very
// crude model of a real process - to show how the value converges.
//
// Reference: https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller
// -----------------------------------------------------------------------------

#include <algorithm>
#include <cmath>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"
#include "pid.h"

namespace {

std::string bar(double value, double scale_max) {
  constexpr int kWidth = 40;
  const int filled = static_cast<int>(
      std::lround(std::clamp(value / scale_max, 0.0, 1.0) * kWidth));
  return std::string(static_cast<std::size_t>(filled), '#') +
         std::string(static_cast<std::size_t>(kWidth - filled), '.');
}

void run() {
  LOG_SECTION("Driving a value from 20 to the setpoint 100");
  // dt = 0.1, output limits [-100, 100], Kp = 0.1, Kd = 0.01, Ki = 0.5
  controller::PID pid(0.1, 100, -100, 0.1, 0.01, 0.5);
  constexpr double kSetpoint = 100.0;
  constexpr int kSteps = 200;
  double value = 20.0;
  int settled_at = -1;

  for (int step = 0; step < kSteps; ++step) {
    const double increment = pid.calculate(kSetpoint, value);
    value += increment;

    if (settled_at < 0 && std::abs(kSetpoint - value) < 0.5) {
      settled_at = step;
    }
    if (step % 10 == 0 || step == kSteps - 1) {
      LOG_S("step " << (step < 10    ? "  "
                        : step < 100 ? " "
                                     : "")
                    << step << "  value " << bar(value, 120.0) << ' ' << value
                    << "  (increment " << increment << ")");
    }
  }

  LOG_S("final value " << value << ", within 0.5 of the setpoint from step "
                       << settled_at);
  LOG("Try other gains: a larger Ki overshoots more, Kd = 0 oscillates "
      "longer.");
}

}  // namespace

LAB_EXAMPLE("PIDSim", "simulate a PID controller converging to a setpoint") {
  run();
}
