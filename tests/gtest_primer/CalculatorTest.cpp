// GoogleTest primer: TEST, TEST_F (fixtures) and TEST_P (parameterized tests).
// See tests/README.md for an explanation of each concept.

#include "gtest_primer/Calculator.h"

#include <gtest/gtest.h>

#include <tuple>

namespace {

// TEST(TestSuiteName, TestName) - for free functions and simple classes.
//
// ASSERT_* stops the test at the first failure (use it when continuing makes no
// sense); EXPECT_* records the failure and continues, so one run can report
// several problems. Prefer EXPECT_* by default.
TEST(CalculatorAddTest, AddsPositiveAndNegativeNumbers) {
  Calculator calculator;

  ASSERT_TRUE(calculator.add(1, 2));
  EXPECT_EQ(calculator.lastResult(), 3);

  calculator.add(4, -6);
  EXPECT_EQ(calculator.lastResult(), -2) << "a custom message explains a failure";

  calculator.add(-4, -6);
  EXPECT_EQ(calculator.lastResult(), -10);
}

// TEST_F(FixtureName, TestName) - the fixture's SetUp() runs before EVERY test,
// so each test starts from a fresh, known state.
class CalculatorDivideTest : public testing::Test {
 protected:
  void SetUp() override { calculator_.add(40, 2); }  // lastResult() == 42

  Calculator calculator_;
};

TEST_F(CalculatorDivideTest, DividesAndTruncates) {
  ASSERT_TRUE(calculator_.divide(7, 2));
  EXPECT_EQ(calculator_.lastResult(), 3);
}

TEST_F(CalculatorDivideTest, DivisionByZeroFailsAndResetsTheResult) {
  EXPECT_EQ(calculator_.lastResult(), 42);  // state from SetUp()
  EXPECT_FALSE(calculator_.divide(7, 0));
  EXPECT_EQ(calculator_.lastResult(), 0);
}

// TEST_P - one test body, many inputs.
class CalculatorMultiplyTest : public testing::TestWithParam<std::tuple<int, int, int>> {};

TEST_P(CalculatorMultiplyTest, MultipliesTwoNumbers) {
  const auto [lhs, rhs, expected] = GetParam();
  Calculator calculator;
  calculator.multiply(lhs, rhs);
  EXPECT_EQ(calculator.lastResult(), expected);
}

INSTANTIATE_TEST_SUITE_P(Values, CalculatorMultiplyTest,
                         testing::Values(std::make_tuple(2, 3, 6), std::make_tuple(-2, 3, -6),
                                         std::make_tuple(0, 99, 0),
                                         std::make_tuple(-4, -5, 20)));

}  // namespace
