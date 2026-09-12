// -----------------------------------------------------------------------------
// Abstract Factory (creational pattern)
//
// Creates FAMILIES of related objects without naming their concrete classes.
// One factory produces matching products, so products from different families
// cannot be mixed by accident.
//
// Example: a toolchain installer. Each operating system needs its own GDB and
// CMake installation commands; a Linux factory only creates Linux products.
//
// Flow:
//   1. Product interfaces            GdbProduct, CMakeProduct
//   2. Concrete products per family  Linux / Windows / MacOs variants
//   3. Abstract factory              ToolchainFactory
//   4. One concrete factory per family
//   5. The client receives one factory and uses it for every product
//
// Factory Method vs Abstract Factory: a factory method creates ONE product; an
// abstract factory groups several factory methods for a family of products.
//
// UML: docs/uml/dp/creational_abstractfactory.drawio.svg
// -----------------------------------------------------------------------------

#include <memory>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// 1. Product interfaces
class GdbProduct {
 public:
  virtual ~GdbProduct() = default;
  virtual void install() const = 0;
};

class CMakeProduct {
 public:
  virtual ~CMakeProduct() = default;
  virtual void install() const = 0;
};

// 2. Concrete products, one set per family
class LinuxGdb : public GdbProduct {
 public:
  void install() const override {
    LOG("    sudo apt install -y gdb && gdb --version");
  }
};
class WindowsGdb : public GdbProduct {
 public:
  void install() const override {
    LOG("    pacman -S mingw-w64-x86_64-gdb && gdb --version");
  }
};
class MacOsGdb : public GdbProduct {
 public:
  void install() const override {
    LOG("    brew install gdb && gdb --version");
  }
};

class LinuxCMake : public CMakeProduct {
 public:
  void install() const override {
    LOG("    sudo apt install -y cmake && cmake --version");
  }
};
class WindowsCMake : public CMakeProduct {
 public:
  void install() const override {
    LOG("    pacman -S mingw-w64-x86_64-cmake && cmake --version");
  }
};
class MacOsCMake : public CMakeProduct {
 public:
  void install() const override {
    LOG("    brew install cmake && cmake --version");
  }
};

// 3. Abstract factory
class ToolchainFactory {
 public:
  virtual ~ToolchainFactory() = default;
  virtual std::unique_ptr<GdbProduct> createGdb() const = 0;
  virtual std::unique_ptr<CMakeProduct> createCMake() const = 0;
};

// 4. Concrete factories
class LinuxToolchainFactory : public ToolchainFactory {
 public:
  std::unique_ptr<GdbProduct> createGdb() const override {
    return std::make_unique<LinuxGdb>();
  }
  std::unique_ptr<CMakeProduct> createCMake() const override {
    return std::make_unique<LinuxCMake>();
  }
};

class WindowsToolchainFactory : public ToolchainFactory {
 public:
  std::unique_ptr<GdbProduct> createGdb() const override {
    return std::make_unique<WindowsGdb>();
  }
  std::unique_ptr<CMakeProduct> createCMake() const override {
    return std::make_unique<WindowsCMake>();
  }
};

class MacOsToolchainFactory : public ToolchainFactory {
 public:
  std::unique_ptr<GdbProduct> createGdb() const override {
    return std::make_unique<MacOsGdb>();
  }
  std::unique_ptr<CMakeProduct> createCMake() const override {
    return std::make_unique<MacOsCMake>();
  }
};

/// Picks the family once, e.g. from configuration. Returns nullptr if unknown.
std::unique_ptr<ToolchainFactory> makeFactory(std::string_view os) {
  if (os == "linux") {
    return std::make_unique<LinuxToolchainFactory>();
  }
  if (os == "windows") {
    return std::make_unique<WindowsToolchainFactory>();
  }
  if (os == "macos") {
    return std::make_unique<MacOsToolchainFactory>();
  }
  return nullptr;
}

// 5. Client code: depends only on the abstract interfaces.
void installToolchain(const ToolchainFactory& factory) {
  const auto cmake = factory.createCMake();
  const auto gdb = factory.createGdb();
  cmake->install();
  gdb->install();  // guaranteed to belong to the same family as cmake
}

void run() {
  for (const std::string_view os : {"linux", "windows", "macos", "solaris"}) {
    LOG_SECTION(os);
    const std::unique_ptr<ToolchainFactory> factory = makeFactory(os);
    if (!factory) {
      LOG_S("    no toolchain factory for '" << os << "'");
      continue;
    }
    installToolchain(*factory);
  }
}

}  // namespace

LAB_EXAMPLE("AbstractFactory",
            "create families of related products that always match") {
  run();
}
