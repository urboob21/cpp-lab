// -----------------------------------------------------------------------------
// Structs
//
//   - A struct groups related data. In C++ it can also have member functions;
//     the only difference to `class` is that members are public by default.
//   - Default member initializers give every field a sane starting value.
//   - Padding: the compiler inserts unused bytes so each member sits at an
//     address matching its alignment. Ordering members from largest to
//     smallest often reduces the size.
//   - C++20: designated initializers and `= default` comparison operators.
//
// Reference: https://en.cppreference.com/w/cpp/language/class
// -----------------------------------------------------------------------------

#include <cstddef>
#include <cstdint>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

struct Sensor {
  double voltage{0.0};  // default member initializers
  int id{0};
  char status{'N'};
  std::string label{"unknown"};

  void print() const {
    LOG_S("  Sensor " << id << " [" << label << "] voltage " << voltage
                      << " V, status " << status);
  }

  bool operator==(const Sensor&) const = default;  // C++20: member-wise ==
};

void setVoltage(Sensor& sensor,
                double volts) {  // by reference: modifies the caller's object
  sensor.voltage = volts;
}

void setVoltage(Sensor* sensor, double volts) {  // by pointer: may be null
  if (sensor != nullptr) {
    sensor->voltage = volts;  // same as (*sensor).voltage
  }
}

Sensor makeSensor(int id, double volts, const std::string& label) {
  return {volts, id, 'N', label};  // returned by value, no copy (RVO)
}

void usage() {
  LOG_SECTION("Creating and using structs");
  const Sensor defaults;                    // every member uses its initializer
  Sensor temperature{3.3, 1, 'N', "temp"};  // aggregate initialization
  const Sensor pressure{.voltage = 5.0, .id = 2, .label = "pressure"};  // C++20

  defaults.print();
  temperature.print();
  pressure.print();

  setVoltage(temperature, 4.2);
  temperature.print();
  setVoltage(&temperature, 4.8);
  temperature.print();

  const Sensor made = makeSensor(3, 1.5, "humidity");
  made.print();

  const auto& [volts, id, status, label] = made;  // structured bindings
  LOG_S("  structured bindings: id = " << id << ", label = " << label
                                       << ", volts = " << volts
                                       << ", status = " << status);
  LOG_S("  made == made copy: " << std::boolalpha << (made == Sensor{made}));
}

struct Unordered {  // members in "random" order
  char a;
  std::int32_t b;
  char c;
  std::int64_t d;
};

struct Ordered {  // same members, largest first
  std::int64_t d;
  std::int32_t b;
  char a;
  char c;
};

void padding() {
  LOG_SECTION("Padding and alignment");
  LOG_S("Unordered {char, int32, char, int64}: sizeof = " << sizeof(Unordered));
  LOG_S("  offsets: a " << offsetof(Unordered, a) << ", b "
                        << offsetof(Unordered, b) << ", c "
                        << offsetof(Unordered, c) << ", d "
                        << offsetof(Unordered, d));
  LOG_S("Ordered   {int64, int32, char, char}: sizeof = " << sizeof(Ordered));
  LOG_S("  offsets: d " << offsetof(Ordered, d) << ", b "
                        << offsetof(Ordered, b) << ", a "
                        << offsetof(Ordered, a) << ", c "
                        << offsetof(Ordered, c));
  LOG_S("alignof(std::int64_t) = " << alignof(std::int64_t));
}

}  // namespace

LAB_EXAMPLE("Struct",
            "structs: initialization, passing, structured bindings, padding") {
  usage();
  padding();
}
