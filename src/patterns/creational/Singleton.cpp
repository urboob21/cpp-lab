// cppcheck-suppress-file [functionStatic]

// Singleton is a creational design pattern that lets you ensure that a class
// has only one instance, while providing a global access point to this
// instance. Appicability:
// (*)  when a class in your program should have just a single instance
// available to all clients; for example, a single database object shared by
// different parts of the program.
// (**) when you need stricter control over global variables.

// UML: docs/uml/patterns_creational_singleton.drawio.svg

#include <iostream>

namespace {
namespace SingletonPattern {

/**
 * The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 */
class Singleton {
 private:
  static inline Singleton* instance = nullptr;
  static inline int num = 0;
  /**
   * The Singleton's constructor should always be private to prevent direct
   * construction calls with the `new` operator.
   */
  Singleton() = default;

 public:
  // 1. Should not be cloneable.
  Singleton(const Singleton& other) = delete;

  // 2. Should not be assignable
  Singleton& operator=(const Singleton& other) = delete;

  static Singleton* getInstance() {
    if (instance == nullptr) {
      instance = new Singleton();
      num++;
    }

    return instance;
  }

  void operation() const {
    std::cout << "Singleton operating num:" << num << "\n";
  }
};

namespace Client {
void clientCode(const Singleton* const s) {
  s->operation();
}
}  // namespace Client

void run() {
  const Singleton* s1 = Singleton::getInstance();
  Client::clientCode(s1);

  const Singleton* s2 = Singleton::getInstance();
  Client::clientCode(s2);

  // Singleton* s3 = new Singleton(); // ERROR
}

}  // namespace SingletonPattern
}  // namespace

struct SingletonAutoRunner {
  SingletonAutoRunner() {
    std::cout << "\n--- Singleton Pattern Example ---\n";
    SingletonPattern::run();
  }
};

static SingletonAutoRunner instance;