#include "controller/pid/pid.h"

#include <gtest/gtest.h>

#include <cmath>
#include <stdexcept>
#include <utility>

namespace {

TEST(PidTest, ZeroErrorGivesZeroOutput) {
  controller::PID pid(0.1, 100, -100, 0.1, 0.01, 0.5);
  EXPECT_DOUBLE_EQ(pid.calculate(50.0, 50.0), 0.0);
}

TEST(PidTest, OutputIsClampedToTheLimits) {
  controller::PID pid(0.1, 10, -5, 100.0, 0.0, 0.0);  // huge proportional gain
  EXPECT_DOUBLE_EQ(pid.calculate(1000.0, 0.0), 10.0);
  EXPECT_DOUBLE_EQ(pid.calculate(-1000.0, 0.0), -5.0);
}

TEST(PidTest, ProportionalOnlyIsKpTimesError) {
  controller::PID pid(1.0, 100, -100, 0.5, 0.0, 0.0);
  EXPECT_DOUBLE_EQ(pid.calculate(10.0, 4.0), 3.0);  // 0.5 * (10 - 4)
}

TEST(PidTest, ConvergesToTheSetpoint) {
  controller::PID pid(0.1, 100, -100, 0.1, 0.01, 0.5);
  double value = 20.0;
  for (int step = 0; step < 200; ++step) {
    value += pid.calculate(100.0, value);
  }
  EXPECT_NEAR(value, 100.0, 0.5);
}

TEST(PidTest, RejectsInvalidParameters) {
  EXPECT_THROW(controller::PID(0.0, 1, -1, 1, 0, 0), std::invalid_argument);
  EXPECT_THROW(controller::PID(0.1, -1, 1, 1, 0, 0), std::invalid_argument);
}

TEST(PidTest, CanBeMoved) {
  controller::PID original(1.0, 100, -100, 1.0, 0.0, 0.0);
  controller::PID moved = std::move(original);
  EXPECT_DOUBLE_EQ(moved.calculate(2.0, 1.0), 1.0);
}

}  // namespace
