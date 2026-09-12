// -----------------------------------------------------------------------------
// Prototype (creational pattern)
//
// Creates new objects by CLONING existing ones, without depending on their
// concrete classes.
//
// Problem: to copy an object through a base-class pointer you would need its
// concrete type - `Base copy = *pointer;` slices, and the client may not even
// know which derived classes exist.
// Solution: each class implements a virtual clone() that returns a copy of
// itself ("virtual copy constructor").
//
// Use it when:
//   - code must copy objects it only knows through an interface
//   - creating an object is expensive or needs a lot of configuration: set up
//     preset prototypes once and clone them
//
// Flow:
//   1. Prototype interface       Extension::clone()
//   2. Concrete prototypes       LoggerExtension, AnalyticsExtension
//   3. (Optional) registry       ExtensionRegistry of named, pre-configured presets
//   4. Client asks for a clone by name and never names a concrete type
//
// UML: docs/uml/dp/creational_prototype.drawio.svg
// -----------------------------------------------------------------------------

#include <map>
#include <memory>
#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// 1. Prototype interface: "I know how to copy myself."
class Extension {
 public:
  virtual ~Extension() = default;
  virtual std::unique_ptr<Extension> clone() const = 0;
  virtual std::string describe() const = 0;
};

/// 2. Concrete prototypes: clone() uses the (deep-copying) copy constructor.
class LoggerExtension : public Extension {
 public:
  explicit LoggerExtension(std::string level) : level_{std::move(level)} {}

  std::unique_ptr<Extension> clone() const override {
    return std::make_unique<LoggerExtension>(*this);
  }
  std::string describe() const override { return "logger, level " + level_; }
  void setLevel(std::string level) { level_ = std::move(level); }

 private:
  std::string level_;
};

class AnalyticsExtension : public Extension {
 public:
  AnalyticsExtension(int sampling_rate, std::string endpoint)
      : sampling_rate_{sampling_rate}, endpoint_{std::move(endpoint)} {}

  std::unique_ptr<Extension> clone() const override {
    return std::make_unique<AnalyticsExtension>(*this);
  }
  std::string describe() const override {
    return "analytics, sampling " + std::to_string(sampling_rate_) + " Hz to " +
           endpoint_;
  }

 private:
  int sampling_rate_;
  std::string endpoint_;
};

/// 3. Registry of named presets.
class ExtensionRegistry {
 public:
  void add(const std::string& name, std::unique_ptr<Extension> prototype) {
    prototypes_[name] = std::move(prototype);
  }

  /// Returns a fresh copy of the preset, or nullptr for unknown names.
  std::unique_ptr<Extension> create(const std::string& name) const {
    const auto it = prototypes_.find(name);
    return it == prototypes_.end() ? nullptr : it->second->clone();
  }

 private:
  std::map<std::string, std::unique_ptr<Extension>> prototypes_;
};

void run() {
  LOG_SECTION("Registering configured prototypes");
  ExtensionRegistry registry;
  registry.add("debug-logger", std::make_unique<LoggerExtension>("DEBUG"));
  registry.add("analytics", std::make_unique<AnalyticsExtension>(
                                1200, "https://stats.example"));

  LOG_SECTION("Client clones by name");
  for (const std::string name : {"debug-logger", "analytics", "unknown"}) {
    if (const auto extension = registry.create(name)) {
      LOG_S("  " << name << " -> " << extension->describe());
    } else {
      LOG_S("  " << name << " -> no such prototype");
    }
  }

  LOG_SECTION("Clones are independent copies");
  const LoggerExtension original{"INFO"};
  std::unique_ptr<Extension> copy = original.clone();
  static_cast<LoggerExtension&>(*copy).setLevel("TRACE");
  LOG_S("  original: " << original.describe());
  LOG_S("  clone   : " << copy->describe());

  const Extension& through_base = original;
  const std::unique_ptr<Extension> full_copy =
      through_base.clone();  // no slicing
  LOG_S("  clone through a base reference keeps the real type: "
        << full_copy->describe());
}

}  // namespace

LAB_EXAMPLE(
    "Prototype",
    "copy objects through an interface with virtual clone() and presets") {
  run();
}
