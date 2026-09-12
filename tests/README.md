# Tests

```bash
ctest --test-dir build -L unit --output-on-failure      # the tests below
ctest --test-dir build -L example                       # run every example once
./build/bin/cpp_lab_project_unit_test --gtest_filter='Calculator*'
```

| Folder | Contents |
|---|---|
| `gtest_primer/` | tutorial tests: `TEST`, `TEST_F`, `TEST_P`, mocks and fakes |
| `mock/` | the Turtle interface, a mock, a fake and the `Painter` under test |
| `lab/` | tests for the lab framework: registry, command line, menu and runner |
| `controller/` | tests for the PID controller |

Every `*Test.cpp` below `tests/` is compiled automatically - just add a file.

---
# GoogleTest
- Ref: https://google.github.io/googletest/
- GoogleTest helps you write better C++ tests.
- Basic Concepts:
  - **Assertions:** the statememts that check whether a condition is true. The result can be:
     - success
     - nonfatal failure
     - fatal failure
  - **Test Suite:** that contains one or many tests.
  - **Text fixture:** the class contain test suite used for multiple tests to share common objects and subroutines.

## 1. Assertions:
- **ASSERT_** generates fatal failures and **abort** the current function.
- **EXPECT_** generates nonfatal failures and don't abort the current function. (Preferred)
- To provide a custom failure message, simply stream it into the macro using **`<<`**
- Ref: https://google.github.io/googletest/reference/assertions.html

## 2. Test Fixtures: Using the Same Data Configuration for Multiple Tests
- To create a fixture class:
   - derive a class from `testing::Test` with a protected body
   - declare the objects the tests share
   - write a default constructor or a `SetUp()` function to prepare them for each test
   - write a destructor or a `TearDown()` function to release resources
   - use `TEST_F(TestFixtureClassName, TestName)` instead of `TEST()`

> All documentation is covered in the official github repo. The primer documentation also covers a lot of information regarding the test macros. You could use the following summary and the examples linked to choose what you want to use. (https://stackoverflow.com/questions/58600728/what-is-the-difference-between-test-test-f-and-test-p)

- **TEST()** is useful when you want to write unit tests for static or global functions or simple classes.
- **TEST_F()** is useful when you need access to objects and subroutines in the unit test.
- **TEST_P()** is useful when you want to write tests with a parameter. Instead of writing multiple tests with different values of the parameter, you can write one test using TEST_P() which uses GetParam() and can be instantiated using INSTANTIATE_TEST_SUITE_P().

- e.g.

```cpp
#include "this/package/foo.h"

#include <gtest/gtest.h>

namespace my {
namespace project {
namespace {

// The fixture for testing class Foo.
class FooTest : public testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  FooTest() {
     // You can do set-up work for each test here.
  }

  ~FooTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(FooTest, MethodBarDoesAbc) {
  const std::string input_filepath = "this/package/testdata/myinputfile.dat";
  const std::string output_filepath = "this/package/testdata/myoutputfile.dat";
  Foo f;
  EXPECT_EQ(f.Bar(input_filepath, output_filepath), 0);
}

// Tests that Foo does Xyz.
TEST_F(FooTest, DoesXyz) {
  // Exercises the Xyz feature of Foo.
}

}  // namespace
}  // namespace project
}  // namespace my

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
```

## 3. gMock
- It's not feasible or wide to rely on real objects entirely when testing.
- A mocks object implements the same interface as a real object, but lets you specify at run time how it will be used and what it should do (which methods will be called? in which order? how many times? with what arguments? what will they return? etc).
- `Fake objects` vs `Mock objects`:
   - Fake objects have working implementations, but usually take some shortcut. e.g. return a static or in-memory value instead of the actual computation.“what happened?”

   - Mock objects are objects pre-programmed with expectations, which form a specification of the calls they are expected to receive. “how did it happen?”

- **gMock** is a library (sometimes we also call it a “framework” to make it sound cool) for creating mock classes and using them. It does to C++ what jMock/EasyMock does to Java (well, more or less).

>When using gMock,
+) first, you use some simple macros to describe the interface you want to mock, and they will expand to the implementation of your mock class;
+) next, you create some mock objects and specify its expectations and behavior using an intuitive syntax;
+) then you exercise code that uses the mock objects. gMock will catch any violation to the expectations as soon as it arises.

TBD https://google.github.io/googletest/gmock_for_dummies.html
