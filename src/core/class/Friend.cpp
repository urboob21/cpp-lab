// -----------------------------------------------------------------------------
// Friends
//
// A friend declaration grants a function or a class access to the private and
// protected members of the class that declares it.
//
//   1. Friend non-member function : typical for operator<< / operator>>
//   2. Friend member function     : one specific member of another class
//   3. Friend class               : every member of another class
//
// Friendship is granted, never taken; it is not inherited and not transitive.
// Use it sparingly - it widens the set of code that depends on private data.
//
// Reference: https://www.learncpp.com/cpp-tutorial/friend-non-member-functions/
// -----------------------------------------------------------------------------

#include <ostream>
#include <sstream>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace non_member {

class Accumulator {
 public:
  void add(int value) { value_ += value; }

  // As a MEMBER, operator<< must be called as `accumulator << stream`,
  // because the left operand of a member operator is always *this.
  std::ostream& operator<<(std::ostream& os) const {
    return os << "member: " << value_;
  }

  // As a friend NON-MEMBER, the natural `stream << accumulator` works and the
  // function can still read value_.
  friend std::ostream& operator<<(std::ostream& os,
                                  const Accumulator& accumulator);

 private:
  int value_{0};
};

std::ostream& operator<<(std::ostream& os, const Accumulator& accumulator) {
  return os << "friend non-member: " << accumulator.value_;
}

void run() {
  LOG_SECTION("1. Friend non-member function");
  Accumulator accumulator;
  accumulator.add(5);

  std::ostringstream backwards;
  accumulator << backwards;  // works, but reads backwards
  LOG_S("accumulator << stream  -> " << backwards.str());
  LOG_S("stream << accumulator  -> " << accumulator);
}

}  // namespace non_member

namespace friend_member {

class Accumulator;  // forward declaration

class Display {
 public:
  void show(const Accumulator& accumulator) const;       // is a friend
  void tryToShow(const Accumulator& accumulator) const;  // is not
};

class Accumulator {
 public:
  void add(int value) { value_ += value; }

  // Only Display::show gets access, not the whole Display class.
  friend void Display::show(const Accumulator& accumulator) const;

 private:
  int value_{0};
};

void Display::show(const Accumulator& accumulator) const {
  LOG_S("Display::show reads value_ = " << accumulator.value_);
}

void Display::tryToShow(const Accumulator& /*accumulator*/) const {
  // LOG_S(accumulator.value_);  // error: 'value_' is private
  LOG("Display::tryToShow has no access to value_");
}

void run() {
  LOG_SECTION("2. Friend member function");
  Accumulator accumulator;
  accumulator.add(-99);
  const Display display;
  display.show(accumulator);
  display.tryToShow(accumulator);
}

}  // namespace friend_member

namespace friend_class {

class Accumulator {
 public:
  void add(int value) { value_ += value; }

 private:
  friend class Display;  // every member function of Display has access
  int value_{0};
};

class Display {
 public:
  void show(const Accumulator& accumulator) const {
    LOG_S("Display::show reads value_ = " << accumulator.value_);
  }
  void showTwice(const Accumulator& accumulator) const {
    LOG_S("Display::showTwice reads value_ * 2 = " << accumulator.value_ * 2);
  }
};

void run() {
  LOG_SECTION("3. Friend class");
  Accumulator accumulator;
  accumulator.add(99);
  const Display display;
  display.show(accumulator);
  display.showTwice(accumulator);
}

}  // namespace friend_class

}  // namespace

LAB_EXAMPLE("Friend",
            "friend functions, friend member functions and friend classes") {
  non_member::run();
  friend_member::run();
  friend_class::run();
}
