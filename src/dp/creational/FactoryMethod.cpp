// -----------------------------------------------------------------------------
// Factory Method (creational pattern)
//
// Declares a method for creating an object in a base class and lets subclasses
// decide which concrete class to instantiate. Code that USES the product
// (installAndCheck) lives in the base class and never names a concrete type.
//
// Use it when:
//   - the exact types of objects your code works with are not known in advance
//   - users of your library should be able to extend its internal components
//   - you want to reuse existing objects instead of rebuilding them
//
// Flow:
//   1. Product interface          Debugger
//   2. Concrete products          LinuxGdb, WindowsGdb, MacOsLldb
//   3. Creator                    DebuggerInstaller: createDebugger() is the
//                                 factory method, installAndCheck() uses it
//   4. Concrete creators          one per platform, override the factory method
//   5. Client picks a creator     and never constructs products directly
//
// UML: docs/uml/dp/creational_factorymethod.drawio.svg
// -----------------------------------------------------------------------------

#include <memory>
#include <string>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// 1. Product interface
class Debugger {
 public:
  virtual ~Debugger() = default;
  virtual std::string installCommand() const = 0;
  virtual std::string versionCommand() const = 0;
};

// 2. Concrete products
class LinuxGdb : public Debugger {
 public:
  std::string installCommand() const override {
    return "sudo apt install -y gdb";
  }
  std::string versionCommand() const override { return "gdb --version"; }
};

class WindowsGdb : public Debugger {
 public:
  std::string installCommand() const override {
    return "pacman -S mingw-w64-x86_64-gdb";
  }
  std::string versionCommand() const override { return "gdb.exe --version"; }
};

class MacOsLldb : public Debugger {
 public:
  std::string installCommand() const override {
    return "xcode-select --install";
  }
  std::string versionCommand() const override { return "lldb --version"; }
};

// 3. Creator
class DebuggerInstaller {
 public:
  virtual ~DebuggerInstaller() = default;

  /// Business logic shared by all platforms. It relies on the factory method
  /// and works with ANY product.
  void installAndCheck() const {
    const std::unique_ptr<Debugger> debugger = createDebugger();
    LOG_S("    install: " << debugger->installCommand());
    LOG_S("    verify : " << debugger->versionCommand());
  }

 private:
  /// The factory method.
  virtual std::unique_ptr<Debugger> createDebugger() const = 0;
};

// 4. Concrete creators
class LinuxInstaller : public DebuggerInstaller {
  std::unique_ptr<Debugger> createDebugger() const override {
    return std::make_unique<LinuxGdb>();
  }
};

class WindowsInstaller : public DebuggerInstaller {
  std::unique_ptr<Debugger> createDebugger() const override {
    return std::make_unique<WindowsGdb>();
  }
};

class MacOsInstaller : public DebuggerInstaller {
  std::unique_ptr<Debugger> createDebugger() const override {
    return std::make_unique<MacOsLldb>();
  }
};

std::unique_ptr<DebuggerInstaller> installerFor(std::string_view os) {
  if (os == "linux") {
    return std::make_unique<LinuxInstaller>();
  }
  if (os == "windows") {
    return std::make_unique<WindowsInstaller>();
  }
  if (os == "macos") {
    return std::make_unique<MacOsInstaller>();
  }
  return nullptr;
}

// 5. Client
void run() {
  for (const std::string_view os : {"linux", "windows", "macos", "unknown"}) {
    LOG_SECTION(os);
    if (const auto installer = installerFor(os)) {
      installer->installAndCheck();
    } else {
      LOG_S("    no installer for '" << os << "'");
    }
  }
}

}  // namespace

LAB_EXAMPLE("FactoryMethod",
            "subclasses decide which product a creator makes") {
  run();
}
