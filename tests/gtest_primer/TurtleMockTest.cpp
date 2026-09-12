// GoogleMock primer: mocks vs fakes.
//
//   Fake : a working but simplified implementation (FakeTurtle records a path).
//          Tests check the RESULT - "what happened?".
//   Mock : an object with expectations about the calls it receives
//          (MockTurtle via MOCK_METHOD). Tests check the INTERACTION -
//          "how did it happen?".

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <utility>

#include "mock/FakeTurtle.h"
#include "mock/MockTurtle.h"
#include "mock/Painter.h"

namespace {

using testing::InSequence;

TEST(MockPainterTest, DrawSquareCallsTheTurtleOperations) {
  MockTurtle turtle;
  const Painter painter{turtle};

  // Expectations must be set BEFORE the code under test runs.
  EXPECT_CALL(turtle, PenDown()).Times(1);
  EXPECT_CALL(turtle, Forward(100)).Times(4);
  EXPECT_CALL(turtle, Turn(90)).Times(4);
  EXPECT_CALL(turtle, PenUp()).Times(1);

  painter.DrawSquare(100);
}  // the mock verifies its expectations when it is destroyed

TEST(MockPainterTest, PenGoesDownBeforeMovingAndUpAtTheEnd) {
  MockTurtle turtle;
  const Painter painter{turtle};

  {
    const InSequence in_order;  // the following expectations must happen in this order
    EXPECT_CALL(turtle, PenDown());
    EXPECT_CALL(turtle, Forward(10)).Times(4);
    EXPECT_CALL(turtle, PenUp());
  }
  EXPECT_CALL(turtle, Turn(90)).Times(4);  // not part of the sequence

  painter.DrawSquare(10);
}

TEST(FakePainterTest, DrawSquareProducesAClosedSquarePath) {
  FakeTurtle turtle;
  const Painter painter{turtle};

  painter.DrawSquare(10);

  const auto& path = turtle.Path();
  ASSERT_EQ(path.size(), 8U);  // 4 lines -> 8 points (start and end of each)

  EXPECT_EQ(path[0], std::make_pair(0, 0));
  EXPECT_EQ(path[1], std::make_pair(0, 10));
  EXPECT_EQ(path[3], std::make_pair(10, 10));
  EXPECT_EQ(path[5], std::make_pair(10, 0));
  EXPECT_EQ(path[7], std::make_pair(0, 0));  // back at the start
}

}  // namespace
