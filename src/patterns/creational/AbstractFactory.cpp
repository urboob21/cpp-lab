// cppcheck-suppress-file [functionStatic]

// Abstract Factory is a creational design pattern that lets you produce
// families of related objects without specifying their concrete classes.
// Appicability:
// (*)  when your code needs to work with various families of related products,
//      but you don’t want it to depend on the concrete classes
//      of those products—they might be unknown beforehand or you simply want to
//      allow for future extensibility.
// (**) when you have a class with a set of Factory Methods that blur its
//      primary responsibility.

// UML: docs/uml/patterns_creational_abstractfactory.drawio.svg

#include <iostream>
#include <string>
namespace {
namespace AbstractFactory {
/**
 * The Product interface declares the operations that all concrete products must
 * implement.
 */
class IGdbProduct {
 public:
  virtual ~IGdbProduct() = default;
  virtual void launch() const = 0;
};

/**
 * Concrete Products provide various implementations of the Product interface.
 */
class LinuxGdbProduct : public IGdbProduct {
 public:
  void launch() const override {
    std::cout
        << "\tsudo apt update && sudo apt install -y gdb && gdb --version\n";
  }
};

class WindowsGdbProduct : public IGdbProduct {
 public:
  void launch() const override {
    std::cout << "\tpacman -Syu mingw-w64-x86_64-gdb && gdb --version\n";
  }
};

class MacOsGdbProduct : public IGdbProduct {
 public:
  void launch() const override {
    std::cout << "\tbrew install gdb && gdb --version\n";
  }
};

class ICMakeProduct {
 public:
  virtual ~ICMakeProduct() = default;
  virtual void launch() const = 0;
};

class LinuxCMakeProduct : public ICMakeProduct {
 public:
  void launch() const override {
    std::cout << "\tsudo apt update && sudo apt install -y cmake && cmake "
                 "--version\n";
  }
};

class WindowsCMakeProduct : public ICMakeProduct {
 public:
  void launch() const override {
    std::cout << "\tpacman -Syu cmake && cmake --version\n";
  }
};

class MacOsCMakeProduct : public ICMakeProduct {
 public:
  void launch() const override {
    std::cout << "\tbrew install cmake && cmake --version\n";
  }
};

// ===================================================================================

/*
 * Abstract Factory
 * provides an abstract interface for creating a family of products
 */
class IProductAbstractFactory {
 public:
  virtual ~IProductAbstractFactory() = default;
  virtual IGdbProduct* createGdbProduct() = 0;
  virtual ICMakeProduct* createCMakeProduct() = 0;
};

/*
 * Concrete Factory
 * each concrete factory create a family of products and client uses
 * one of these factories so it never has to instantiate a product object
 */
class WindowsProductFactory : public IProductAbstractFactory {
 public:
  IGdbProduct* createGdbProduct() override { return new WindowsGdbProduct(); }
  ICMakeProduct* createCMakeProduct() override {
    return new WindowsCMakeProduct();
  }
};

class LinuxProductFactory : public IProductAbstractFactory {
 public:
  IGdbProduct* createGdbProduct() override { return new LinuxGdbProduct(); }
  ICMakeProduct* createCMakeProduct() override {
    return new LinuxCMakeProduct();
  }
};

class MacOsProductFactory : public IProductAbstractFactory {
 public:
  IGdbProduct* createGdbProduct() override { return new MacOsGdbProduct(); }
  ICMakeProduct* createCMakeProduct() override {
    return new MacOsCMakeProduct();
  }
};

// ===================================================================================

/**
 * The client code works with factories and products only through abstract
 * types: AbstractFactory and AbstractProduct. This lets you pass any factory or
 * product subclass to the client code without breaking it.
 */
namespace ClientCode {
void clientCode(IProductAbstractFactory* f) {
  ICMakeProduct* cmake = f->createCMakeProduct();
  IGdbProduct* gdb = f->createGdbProduct();
  cmake->launch();
  gdb->launch();

  delete cmake;
  delete gdb;
}
}  // namespace ClientCode

// static redudant inside anonymous namespace
IProductAbstractFactory* createProductFactory(const std::string& os) {
  if (os == "linux") {
    return new LinuxProductFactory();
  } else if (os == "windows") {
    return new WindowsProductFactory();
  } else if (os == "macos") {
    return new MacOsProductFactory();
  } else {
    std::cout << "OS not support yet - " << os << "\n";
    return nullptr;
  }
}

void run() {
  std::string os = "linux";
  IProductAbstractFactory* factory = createProductFactory(os);
  ClientCode::clientCode(factory);
  delete factory;
}
}  // namespace AbstractFactory
}  // namespace

struct AbstractFactoryAutoRunner {
  AbstractFactoryAutoRunner() {
    std::cout << "\n--- AbstractFactory Pattern Example ---\n";
    AbstractFactory::run();
  }
};

static AbstractFactoryAutoRunner instance;