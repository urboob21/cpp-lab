// -----------------------------------------------------------------------------
// Overloading operator<< and operator>> for streams
//
//   std::ostream& operator<<(std::ostream& out, const T& value);
//   std::istream& operator>>(std::istream& in, T& value);
//
//   - They must be NON-members: the left operand is the stream, a class we
//     cannot modify. Make them friends if they need private members.
//   - Return the stream so calls chain: std::cout << a << b.
//   - operator>> should set failbit on malformed input and leave the target
//     unchanged, so `if (in >> value)` works like for built-in types.
//
// The same operators work with std::cout/std::cin, files and string streams;
// string streams are used below so the example needs no keyboard input.
//
// Reference: https://en.cppreference.com/w/cpp/language/operators#Stream_extraction_and_insertion
// -----------------------------------------------------------------------------

#include <istream>
#include <ostream>
#include <sstream>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Point {
 public:
  Point() = default;
  Point(int x, int y) : x_{x}, y_{y} {}

  friend std::ostream& operator<<(std::ostream& out, const Point& point);
  friend std::istream& operator>>(std::istream& in, Point& point);

 private:
  int x_{0};
  int y_{0};
};

/// Writes "(x, y)".
std::ostream& operator<<(std::ostream& out, const Point& point) {
  return out << '(' << point.x_ << ", " << point.y_ << ')';
}

/// Reads "(x, y)". On malformed input sets failbit and keeps `point` unchanged.
std::istream& operator>>(std::istream& in, Point& point) {
  char open = 0;
  char comma = 0;
  char close = 0;
  int x = 0;
  int y = 0;
  if (in >> open >> x >> comma >> y >> close && open == '(' && comma == ',' &&
      close == ')') {
    point = Point{x, y};
  } else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

void run() {
  LOG_SECTION("operator<<");
  const Point origin;
  const Point target{3, -4};
  std::ostringstream out;
  out << "origin " << origin << " -> target " << target;  // chained calls
  LOG(out.str());

  LOG_SECTION("operator>>");
  std::istringstream input("(10, 20) (7,8) (oops)");
  Point point;
  while (input >> point) {
    LOG_S("parsed " << point);
  }
  LOG_S("stopped at malformed input, fail() = "
        << std::boolalpha << input.fail() << ", point kept its last value "
        << point);
}

}  // namespace

LAB_EXAMPLE("StreamOperator",
            "operator<< and operator>> for a custom type, input validation") {
  run();
}
