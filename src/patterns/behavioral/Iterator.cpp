// Iterator is a behavioral design pattern that lets you traverse elements of a
// collection without exposing its underlying representation (list, stack, tree,
// etc.). Provide a way to access the elements of an aggregate object
// sequentially without exposing its underlying representation. Appicability:
// (*)   when your collection has a complex data structure under the hood, but
// you want to hide its complexity from clients (either for convenience or
// security reasons).
// (**)  reduce duplication of the traversal code across your app.
// (***) when you want your code to be able to traverse different data
// structures or when types of these structures are unknown beforehand.

// UML: docs/uml/patterns_behavioral_iterator.drawio.svg

#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

namespace {
namespace Iterator {
class DataModel {
 private:
  int m_value;

 public:
  explicit DataModel(int value) : m_value{value} {}

  void setValue(int v) { m_value = v; }

  int getValue() const { return m_value; }
};

/**
 * Iterator interface: declares the operations required for traversing a
 * collection: fetching the next element, retrieving the current position,
 * restarting iteration, etc.
 */
template <typename T>
class IIterator {
 public:
  virtual bool hasNext() const = 0;
  virtual const T* next() = 0;
  virtual ~IIterator() = default;
};

/**
 * Aggregate interface: declares one or multiple methods for getting iterators
 * compatible with the collection. Note that the return type of the methods must
 * be declared as the iterator interface so that the concrete collections can
 * return various kinds of iterators.
 */
template <typename T>
class IAggregate {
 public:
  virtual IIterator<T>* createIterator() = 0;
  virtual ~IAggregate() = default;
};

/**
 * Concrete Iterator: implement specific algorithms for traversing a collection.
 * The iterator object should track the traversal progress on its own. This
 * allows several iterators to traverse the same collection independently of
 * each other.
 */
template <typename T>
class VectorConcreteIterator : public IIterator<T> {
 private:
  const std::vector<T>& m_data;
  size_t m_currentIndex{0};

 public:
  explicit VectorConcreteIterator(const std::vector<T>& data) : m_data{data} {}

  bool hasNext() const override { return m_currentIndex < m_data.size(); }

  const T* next() override {
    if (hasNext()) {
      return (T*)&m_data[m_currentIndex++];
    } else {
      return nullptr;
    }
  }
};

template <typename T>
class ListConcreteIterator : public IIterator<T> {
 private:
  const std::list<T>& m_data;
  typename std::list<T>::const_iterator m_it;

 public:
  explicit ListConcreteIterator(const std::list<T>& data)
      : m_data(data), m_it(m_data.begin()) {}

  bool hasNext() const override { return m_it != m_data.end(); }

  const T* next() override {
    if (!hasNext())
      return nullptr;
    const T* ptr = &(*m_it);
    ++m_it;
    return ptr;
  }
};

/**
 * Concrete Aggregate: return new instances of a particular concrete iterator
 * class each time the client requests one.
 */
template <typename T>
class ListConreteAggregate : public IAggregate<T> {
 private:
  std::list<T> m_data;

 public:
  void add(const T& i) { m_data.push_back(i); }

  IIterator<T>* createIterator() override {
    return new ListConcreteIterator<T>(m_data);
  }
};

template <typename T>
class VectorConcreteAggregate : public IAggregate<T> {
 private:
  std::vector<T> m_data;

 public:
  void add(const T& i) { m_data.push_back(i); }

  IIterator<T>* createIterator() override {
    return new VectorConcreteIterator<T>(m_data);
  }
};

/**
 * The Client works with both collections and iterators via their interfaces.
 * This way the client isn’t coupled to concrete classes, allowing you to use
 * various collections and iterators with the same client code.
 */
namespace Client {

void clientCode(IAggregate<int>* collection) {
  IIterator<int>* iterator = collection->createIterator();

  if (iterator != nullptr) {
    while (iterator->hasNext()) {
      std::cout << "int: " << *(iterator->next()) << "\n";
    }
  }

  delete iterator;
}

void clientCode(IAggregate<DataModel>* collection) {
  IIterator<DataModel>* iterator = collection->createIterator();

  if (iterator != nullptr) {
    while (iterator->hasNext()) {
      std::cout << "data: " << iterator->next()->getValue() << "\n";
    }
  }
  delete iterator;
}
}  // namespace Client

void run() {
  std::cout << "\nVectorConcreteAggregate\n";
  VectorConcreteAggregate<int> intCollection;
  for (int i = 0; i < 10; ++i) {
    intCollection.add(i);
  }
  Client::clientCode(&intCollection);
  std::cout << "\n";
  VectorConcreteAggregate<DataModel> dataCollection;
  for (int i = 0; i < 10; ++i) {
    dataCollection.add(DataModel(i * 10));
  }
  Client::clientCode(&dataCollection);

  std::cout << "\nListConreteAggregate\n";
  ListConreteAggregate<int> intCollection2;
  for (int i = 0; i < 10; ++i) {
    intCollection2.add(i);
  }

  Client::clientCode(&intCollection2);
  std::cout << "\n";
  ListConreteAggregate<DataModel> dataCollection2;
  for (int i = 0; i < 10; ++i) {
    dataCollection2.add(DataModel(i * 10));
  }
  Client::clientCode(&dataCollection2);
}
}  // namespace Iterator
}  // namespace

struct IteratorAutoRunner {
  IteratorAutoRunner() {
    std::cout << "\n--- Iterator Pattern Example ---\n";
    Iterator::run();
  }
};

static IteratorAutoRunner instance;