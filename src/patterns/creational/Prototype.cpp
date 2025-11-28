// cppcheck-suppress-file [functionStatic]

// Prototype is a creational design pattern that lets you "copy existing objects" without making your code "dependent on their classes".
// Appicability:
// (*)  when your code shouldn’t depend on the concrete classes of objects that you need to copy.
// (**) when you want to reduce the number of subclasses that only differ in the way they initialize their respective objects.

// UML: docs/uml/patterns_behavioral_prototype.drawio.svg

#include <iostream>
#include <unordered_map>
namespace {
namespace Prototy {

/*
 * Prototype interface declares the cloning methods.
 * In most cases, it’s a single `clone` method.
 */
class IExtensionPrototype {
 public:
  virtual ~IExtensionPrototype() = default;
  virtual IExtensionPrototype* clone() = 0;
  virtual void execute() const = 0;
};

/*
 * Concrete Prototype implement an operation for cloning itself
 * In addition to copying the original object’s data to the clone,
 * this method may also handle some edge cases of the cloning process related to cloning linked objects,
 * untangling recursive dependencies, etc.
 */
class LoggerExtension : public IExtensionPrototype {
 private:
  std::string m_logLevel;

 public:
  explicit LoggerExtension(const std::string& level = "DEBUG")
      : m_logLevel{level} {}

  IExtensionPrototype* clone() override { return new LoggerExtension(*this); }

  void execute() const override {
    std::cout << "[Logger] log level: " << m_logLevel << "\n";
  }
};

class AnalyticsExtension : public IExtensionPrototype {
 private:
  int m_sRate;

 public:
  explicit AnalyticsExtension(int level = 1) : m_sRate{level} {}

  IExtensionPrototype* clone() override {
    return new AnalyticsExtension(*this);
  }

  void execute() const override {
    std::cout << "[Analytics] sampling rate: " << m_sRate << "\n";
  }
};

/**
 * Prototype Registry provides an easy way to access frequently-used prototypes.
 * It stores a set of pre-built objects that are ready to be copied.
 * The simplest prototype registry is a name ^ prototype hash map.
 * However, if you need better search criteria than a simple name, you can build a much more robust version of the registry.
 */
class ExtensionPrototypeRegistry {
 private:
  std::unordered_map<std::string, IExtensionPrototype*> prototypes;

 public:
  ~ExtensionPrototypeRegistry() {
    for (auto it = prototypes.begin(); it != prototypes.end();) {
      delete it->second;          // free the pointer
      it = prototypes.erase(it);  // erase and move to next
    }
  }
  void registerExtension(const std::string& id, IExtensionPrototype* proto) {
    prototypes[id] = proto;
  }

  IExtensionPrototype* create(const std::string& id) const {
    auto it = prototypes.find(id);
    if (it != prototypes.end()) {
      return it->second->clone();
    }
    return nullptr;
  }
};

/*
 * Client creates a new object by asking a prototype to clone itself
 */
namespace Client {
void clientCode(const ExtensionPrototypeRegistry* const registry) {
  IExtensionPrototype* loggerEtx = registry->create("logger");
  loggerEtx->execute();
  IExtensionPrototype* analyxEtx = registry->create("analyze");
  analyxEtx->execute();

  delete loggerEtx;
  delete analyxEtx;
}

}  // namespace Client

void run() {
  ExtensionPrototypeRegistry* registry = new ExtensionPrototypeRegistry();
  registry->registerExtension("logger", new LoggerExtension("DEBUG"));
  registry->registerExtension("analyze", new AnalyticsExtension(1200));
  Client::clientCode(registry);

  delete registry;
}
}  // namespace Prototy
}  // namespace

struct PrototypeAutoRunner {
  PrototypeAutoRunner() {
    std::cout << "\n--- Prototype Pattern Example ---\n";
    Prototy::run();
  }
};

static PrototypeAutoRunner instance;