// -----------------------------------------------------------------------------
// Virtual base classes and the diamond problem
//
//          PoweredDevice
//           ^         ^
//           |         |
//       Scanner     Printer
//           ^         ^
//           |         |
//             Copier
//
// With plain inheritance a Copier contains TWO PoweredDevice sub-objects, so
// the base is constructed twice and `PoweredDevice&` conversions are ambiguous.
// `virtual` inheritance shares one PoweredDevice sub-object. The MOST DERIVED
// class (Copier) is then responsible for constructing it.
//
// Reference: https://www.learncpp.com/cpp-tutorial/virtual-base-classes/
// -----------------------------------------------------------------------------

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace problem {

class PoweredDevice {
 public:
  explicit PoweredDevice(int watts) {
    LOG_S("  PoweredDevice(" << watts << " W)");
  }
};

class Scanner : public PoweredDevice {
 public:
  Scanner() : PoweredDevice{10} { LOG("  Scanner()"); }
  void start() const { LOG("  Scanner::start"); }
};

class Printer : public PoweredDevice {
 public:
  Printer() : PoweredDevice{20} { LOG("  Printer()"); }
  void start() const { LOG("  Printer::start"); }
};

class Copier : public Scanner, public Printer {
 public:
  Copier() { LOG("  Copier()"); }
};

void run() {
  LOG_SECTION("Problem: plain multiple inheritance");
  const Copier copier;
  LOG("  -> PoweredDevice was constructed TWICE");

  // copier.start();                          // error: ambiguous
  // PoweredDevice& device = copier;          // error: ambiguous base
  copier.Scanner::start();  // must name the path explicitly
  copier.Printer::start();
  LOG_S("  sizeof(Copier) = " << sizeof(Copier));
}

}  // namespace problem

namespace virtual_inheritance {

class PoweredDevice {
 public:
  explicit PoweredDevice(int watts) {
    LOG_S("  PoweredDevice(" << watts << " W)");
  }
};

class Scanner : public virtual PoweredDevice {
 public:
  // This initializer is ignored when a Scanner is part of a Copier.
  Scanner() : PoweredDevice{10} { LOG("  Scanner()"); }
  void start() const { LOG("  Scanner::start"); }
};

class Printer : public virtual PoweredDevice {
 public:
  Printer() : PoweredDevice{20} { LOG("  Printer()"); }
  void start() const { LOG("  Printer::start"); }
};

class Copier : public Scanner, public Printer {
 public:
  // The most derived class constructs the shared virtual base.
  Copier() : PoweredDevice{30} { LOG("  Copier()"); }
};

void run() {
  LOG_SECTION("Solution: virtual inheritance");
  const Copier copier;
  LOG("  -> ONE PoweredDevice, constructed by Copier with 30 W");

  [[maybe_unused]] const PoweredDevice& device = copier;  // no longer ambiguous
  copier.Scanner::start();
  copier.Printer::start();
  LOG_S("  sizeof(Copier) = " << sizeof(Copier)
                              << " (virtual bases add hidden pointers)");
}

}  // namespace virtual_inheritance

}  // namespace

LAB_EXAMPLE("VirtualBase", "the diamond problem and virtual inheritance") {
  problem::run();
  virtual_inheritance::run();
}
