// cppcheck-suppress-file [functionStatic]

// Builder is a creational design pattern that lets you construct complex
// objects step by step. The pattern allows you to produce different types and
// representations of an object using the same construction code. Appicability:
// (*)  Use the Builder pattern to get rid of a “telescoping constructor”.
// (**) when you want your code to be able to create different representations
// of some product (for example, stone and wooden houses).

// UML: docs/uml/patterns_behavioral_iterator.drawio.svg

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {
namespace BuilderPattern {
class Product {
 private:
  std::vector<std::string> m_parts;

 public:
  void addPart(const std::string& part) { m_parts.push_back(part); }

  void print() const {
    std::cout << "Product parts: ";
    for (size_t i = 0; i < m_parts.size(); ++i) {
      std::cout << m_parts[i];
      if (i + 1 < m_parts.size())
        std::cout << ", ";
    }
    std::cout << "\n\n";
  }
};

/**
 * The Builder interface specifies methods for creating the different parts of
 * the Product objects.
 */
class IBuilder {
 public:
  virtual ~IBuilder() = default;
  virtual IBuilder& reset() = 0;
  virtual IBuilder& producePart1() = 0;
  virtual IBuilder& producePart2() = 0;
  virtual IBuilder& producePart3() = 0;

  virtual Product* const build() = 0;
};

class AbstractBuilder : public IBuilder {
 protected:
  Product* m_product;

 public:
  explicit AbstractBuilder() { m_product = new Product(); }

  ~AbstractBuilder() { delete m_product; }

  AbstractBuilder(const AbstractBuilder& other) {
    if (m_product != nullptr) {
      delete m_product;
    }
    m_product = new Product();
    *m_product = *other.m_product;
  }

  AbstractBuilder& operator=(const AbstractBuilder& other) {
    if (this == &other) {
      return *this;
    }
    if (m_product != nullptr) {
      delete m_product;
    }
    m_product = new Product();
    *m_product = *other.m_product;
    return *this;
  }

  IBuilder& reset() override final {
    if (m_product != nullptr) {
      delete m_product;
    }
    m_product = new Product();

    return *this;
  }
};

/**
 * The Concrete Builder classes follow the Builder interface and provide
 * specific implementations of the building steps. Your program may have several
 * variations of Builders, implemented differently.
 */
class SimpleBuilder : public AbstractBuilder {
 public:
  IBuilder& producePart1() override {
    m_product->addPart("PART1");
    return *this;
  }

  IBuilder& producePart2() override {
    m_product->addPart("PART2");
    return *this;
  }

  IBuilder& producePart3() override {
    m_product->addPart("PART3");
    return *this;
  }

  Product* const build() override { return m_product; }
};

class ComplexBuilder : public AbstractBuilder {
 public:
  IBuilder& producePart1() override {
    m_product->addPart("PART_1-X9a7Fq!2@Lm#48Z");
    return *this;
  }

  IBuilder& producePart2() override {
    m_product->addPart("PART_2-X9a7Fq!2@Lm#48Z");
    return *this;
  }

  IBuilder& producePart3() override {
    m_product->addPart("PART_3-X9a7Fq!2@Lm#48Z");
    return *this;
  }

  Product* const build() override { return m_product; }
};

namespace Client {
void clientCode(IBuilder* const builder) {
  const Product* product1 =
      (*builder).producePart1().producePart2().producePart3().build();
  product1->print();

  const Product* product2 = (*builder).reset().producePart1().build();
  product2->print();
}
}  // namespace Client

void run() {
  {
    std::cout << "ConcreteBuilder: Simple\n";
    IBuilder* builder = new SimpleBuilder();
    Client::clientCode(builder);
    delete builder;
  }
  {
    std::cout << "ConcreteBuilder: Complex\n";
    IBuilder* builder = new ComplexBuilder();
    Client::clientCode(builder);
    delete builder;
  }
}
}  // namespace BuilderPattern
}  // namespace

struct BuilderAutoRunner {
  BuilderAutoRunner() {
    std::cout << "\n--- Builder Pattern Example ---\n";
    BuilderPattern::run();
  }
};

static BuilderAutoRunner instance;