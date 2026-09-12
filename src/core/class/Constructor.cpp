// -----------------------------------------------------------------------------
// Constructors
//
//   1. Member initializer list : initialize members before the body runs
//   2. Default constructor     : callable without arguments
//   3. Delegating constructor  : one constructor forwards to another (C++11)
//   4. Copy constructor        : T(const T&) creates an independent copy
//   5. Move constructor        : T(T&&) steals the resources of an rvalue
//   6. explicit                : forbids implicit conversions via a constructor
//
// Notes:
//   - Members are initialized in DECLARATION order, whatever the order of the
//     initializer list (-Wreorder warns about mismatches).
//   - A constructor can throw to report failure; the object then never exists.
//
// Reference: https://en.cppreference.com/w/cpp/language/constructor
// -----------------------------------------------------------------------------

#include <string>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace member_initializer_list {

class Point3D {
 public:
  // Preferred: every member is initialized directly with its final value.
  Point3D(int x, int y, int z) : x_{x}, y_{y}, z_{z} {
    LOG("Point3D(int, int, int) : x_{x}, y_{y}, z_{z}");
  }

  // Works for ints, but members are default-initialized first and assigned in
  // the body. const members and references cannot be assigned, so they MUST
  // use the initializer list.
  Point3D(int x, int y) {
    LOG("Point3D(int, int) assigns in the body");
    x_ = x;
    y_ = y;
    z_ = 0;
  }

  void print() const {
    LOG_S("  -> {" << x_ << ", " << y_ << ", " << z_ << "}");
  }

 private:
  int x_;
  int y_;
  int z_;
};

void run() {
  LOG_SECTION("1. Member initializer list");
  const Point3D a(1, 2, 3);
  a.print();
  const Point3D b(4, 5);
  b.print();
}

}  // namespace member_initializer_list

namespace default_constructor {

/// No constructor declared: the compiler generates Implicit().
class Implicit {
 public:
  int value{7};  // default member initializer
};

/// Declaring any constructor suppresses the implicit default constructor,
/// `= default` brings it back.
class Defaulted {
 public:
  Defaulted() = default;
  explicit Defaulted(double value) : value_{value} {}
  double value() const { return value_; }

 private:
  double value_{1.5};
};

class WithDefaultArgument {
 public:
  // Default arguments must be the rightmost parameters. This constructor is
  // also the default constructor, so `WithDefaultArgument() = default;` would
  // be ambiguous.
  explicit WithDefaultArgument(int x = 1) : x_{x} {
    LOG_S("WithDefaultArgument(int x = 1), x = " << x_);
  }

 private:
  int x_;
};

void run() {
  LOG_SECTION("2. Default constructor");

  const Implicit implicit;
  LOG_S("Implicit implicit;      value = " << implicit.value);

  const Defaulted defaulted;
  const Defaulted from_double{2.5};
  LOG_S("Defaulted defaulted;    value = " << defaulted.value());
  LOG_S("Defaulted{2.5};         value = " << from_double.value());

  [[maybe_unused]] const WithDefaultArgument uses_default;
  [[maybe_unused]] const WithDefaultArgument uses_value{5};

  // Implicit oops();  // most vexing parse: declares a function, not an object
}

}  // namespace default_constructor

namespace delegating {

class Rectangle {
 public:
  Rectangle(int width, int height) : width_{width}, height_{height} {
    LOG("Rectangle(int, int)  <- does the real work");
  }
  explicit Rectangle(int side) : Rectangle{side, side} {
    LOG("Rectangle(int)       <- delegates to Rectangle(int, int)");
  }
  Rectangle() : Rectangle{1} {
    LOG("Rectangle()          <- delegates to Rectangle(int)");
  }

  int area() const { return width_ * height_; }

 private:
  int width_;
  int height_;
};

void run() {
  LOG_SECTION("3. Delegating constructor");
  const Rectangle unit;
  LOG_S("area = " << unit.area());
}

}  // namespace delegating

namespace copy {

/// The compiler generates a member-wise copy constructor.
class Implicit {
 public:
  Implicit(int x, int y) : x_{x}, y_{y} {}
  void print() const { LOG_S("  {" << x_ << ", " << y_ << "}"); }

 private:
  int x_;
  int y_;
};

class Tracked {
 public:
  explicit Tracked(std::string name) : name_{std::move(name)} {
    LOG_S("Tracked(std::string)            " << name_);
  }
  Tracked(const Tracked& other) : name_{other.name_ + " (copy)"} {
    LOG_S("Tracked(const Tracked&)         " << name_);
  }
  Tracked& operator=(const Tracked& other) {
    LOG_S("operator=(const Tracked&)       " << other.name_);
    if (this != &other) {  // self-assignment guard
      name_ = other.name_;
    }
    return *this;
  }
  ~Tracked() = default;

 private:
  std::string name_;
};

/// `= delete` removes the copy operations: this type can not be copied.
class NonCopyable {
 public:
  NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};

void takeByValue(const Tracked /*copy*/) {}

void run() {
  LOG_SECTION("4. Copy constructor and copy assignment");

  const Implicit a(1, 2);
  const Implicit b{a};  // compiler-generated copy
  b.print();

  const Tracked original{"original"};
  LOG("Tracked copy1 = original;");
  const Tracked copy1 = original;
  LOG("Tracked copy2{original};");
  const Tracked copy2{original};
  LOG("takeByValue(original);  <- passing by value copies too");
  takeByValue(original);
  LOG("copy3 = original;       <- assignment to an existing object");
  Tracked copy3{"copy3"};
  copy3 = original;

  [[maybe_unused]] const NonCopyable unique;
  // NonCopyable duplicate{unique};  // error: use of deleted function
}

}  // namespace copy

namespace move {

/// Owns a (potentially large) buffer. Moving hands the buffer over instead of
/// copying every element.
class Buffer {
 public:
  explicit Buffer(std::size_t size) : data_(size) {}

  Buffer(const Buffer& other) : data_{other.data_} {
    LOG_S("Buffer(const Buffer&)  copied " << data_.size() << " elements");
  }

  Buffer(Buffer&& other) noexcept : data_{std::move(other.data_)} {
    LOG_S("Buffer(Buffer&&)       took over " << data_.size() << " elements");
  }

  Buffer& operator=(const Buffer& other) {
    LOG("operator=(const Buffer&)");
    if (this != &other) {
      data_ = other.data_;
    }
    return *this;
  }

  Buffer& operator=(Buffer&& other) noexcept {
    LOG("operator=(Buffer&&)");
    data_ = std::move(other.data_);
    return *this;
  }

  ~Buffer() = default;

  std::size_t size() const { return data_.size(); }

 private:
  std::vector<int> data_;
};

void run() {
  LOG_SECTION("5. Move constructor and move assignment");

  Buffer source(1000);
  LOG("Buffer copied = source;");
  const Buffer copied = source;
  LOG("Buffer moved = std::move(source);");
  const Buffer moved = std::move(source);
  // A moved-from object is valid but unspecified; std::vector leaves it empty.
  LOG_S("source.size() = " << source.size()
                           << ", moved.size() = " << moved.size());

  LOG("target = Buffer(10);   <- a temporary is an rvalue, so it is moved");
  Buffer target(1);
  target = Buffer(10);
  LOG_S("target.size() = " << target.size()
                           << ", copied.size() = " << copied.size());
}

}  // namespace move

namespace explicit_constructor {

class Seconds {
 public:
  Seconds(double value) : value_{value} {}  // NOLINT: implicit on purpose
  double value() const { return value_; }

 private:
  double value_;
};

class Meters {
 public:
  explicit Meters(double value) : value_{value} {}
  double value() const { return value_; }

 private:
  double value_;
};

void sleepFor(Seconds seconds) {
  LOG_S("sleepFor(" << seconds.value() << " s)");
}

void walk(Meters distance) {
  LOG_S("walk(" << distance.value() << " m)");
}

void run() {
  LOG_SECTION("6. explicit constructors");
  sleepFor(1.5);  // silently converts double -> Seconds
  // walk(2.0);   // error: Meters(double) is explicit
  walk(Meters{2.0});  // the conversion must be spelled out
}

}  // namespace explicit_constructor

}  // namespace

LAB_EXAMPLE("Constructor",
            "initializer lists, default, delegating, copy, move and explicit "
            "constructors") {
  member_initializer_list::run();
  default_constructor::run();
  delegating::run();
  copy::run();
  move::run();
  explicit_constructor::run();
}
