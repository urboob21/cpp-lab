#include "DeleteMe.h"

#include <gtest/gtest.h>

TEST(AddTest, ValidNumbers) {
  DeleteMe calc = DeleteMe();

  ASSERT_TRUE(calc.add(1, 2));
  ASSERT_EQ(3, calc.get_last_result());

  ASSERT_TRUE(calc.add(0, 5));
  ASSERT_EQ(5, calc.get_last_result());

  ASSERT_TRUE(calc.add(5, 0));
  ASSERT_EQ(5, calc.get_last_result());

  ASSERT_TRUE(calc.add(4, -6));
  ASSERT_EQ(-2, calc.get_last_result());

  ASSERT_TRUE(calc.add(-4, 6));
  ASSERT_EQ(2, calc.get_last_result());

  ASSERT_TRUE(calc.add(-4, -6));
  ASSERT_EQ(-10, calc.get_last_result());
}

TEST(DivTest, ValidNumbers) {
  DeleteMe calc = DeleteMe();

  ASSERT_TRUE(calc.div(10, 2));
  ASSERT_EQ(5, calc.get_last_result());

  ASSERT_TRUE(calc.div(-12, 3));
  ASSERT_EQ(-4, calc.get_last_result());

  ASSERT_TRUE(calc.div(12, -3));
  ASSERT_EQ(-4, calc.get_last_result());
}

TEST(DivTest, InvalidNumbers) {
  DeleteMe calc = DeleteMe();

  ASSERT_FALSE(calc.div(7, 0));
  ASSERT_EQ(0, calc.get_last_result());
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
