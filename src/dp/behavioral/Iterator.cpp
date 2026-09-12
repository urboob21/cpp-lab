// -----------------------------------------------------------------------------
// Iterator (behavioral pattern)
//
// Lets clients traverse the elements of a collection without knowing how the
// collection is stored (array, list, tree, ...).
//
// Use it when:
//   - a collection has a complex internal structure you want to hide
//   - the same client code should traverse different kinds of collections
//   - traversal logic should not be duplicated across the code base
//
// Participants:
//   Iterator           interface: hasNext() / next()
//   ConcreteIterator   VectorIterator, ListIterator (track their own position)
//   Aggregate          interface: createIterator()
//   ConcreteAggregate  VectorCollection, ListCollection
//
// In C++ the standard library already implements this pattern: containers
// provide begin()/end() iterators, which range-based for loops and algorithms
// use. The second part shows how a custom collection plugs into that.
//
// UML: docs/uml/dp/behavioral_iterator.drawio.svg
// -----------------------------------------------------------------------------

#include <cstddef>
#include <list>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace classic {

template <typename T>
class Iterator {
 public:
  virtual ~Iterator() = default;
  virtual bool hasNext() const = 0;
  virtual const T& next() = 0;
};

template <typename T>
class Aggregate {
 public:
  virtual ~Aggregate() = default;
  virtual std::unique_ptr<Iterator<T>> createIterator() const = 0;
};

template <typename T>
class VectorIterator : public Iterator<T> {
 public:
  explicit VectorIterator(const std::vector<T>& data) : data_{data} {}
  bool hasNext() const override { return index_ < data_.size(); }
  const T& next() override { return data_[index_++]; }

 private:
  const std::vector<T>& data_;
  std::size_t index_{0};
};

template <typename T>
class ListIterator : public Iterator<T> {
 public:
  explicit ListIterator(const std::list<T>& data)
      : data_{data}, position_{data.begin()} {}
  bool hasNext() const override { return position_ != data_.end(); }
  const T& next() override { return *position_++; }

 private:
  const std::list<T>& data_;
  typename std::list<T>::const_iterator position_;
};

template <typename T>
class VectorCollection : public Aggregate<T> {
 public:
  void add(const T& value) { data_.push_back(value); }
  std::unique_ptr<Iterator<T>> createIterator() const override {
    return std::make_unique<VectorIterator<T>>(data_);
  }

 private:
  std::vector<T> data_;
};

template <typename T>
class ListCollection : public Aggregate<T> {
 public:
  void add(const T& value) { data_.push_back(value); }
  std::unique_ptr<Iterator<T>> createIterator() const override {
    return std::make_unique<ListIterator<T>>(data_);
  }

 private:
  std::list<T> data_;
};

/// Client code: works with any Aggregate<std::string>.
void printAll(const char* label, const Aggregate<std::string>& collection) {
  std::string line;
  for (auto it = collection.createIterator(); it->hasNext();) {
    line += it->next() + " ";
  }
  LOG_S("  " << label << line);
}

void run() {
  LOG_SECTION("Classic Iterator: one client, two storages");
  VectorCollection<std::string> vector_names;
  ListCollection<std::string> list_names;
  for (const char* name : {"Ada", "Grace", "Linus"}) {
    vector_names.add(name);
    list_names.add(name);
  }
  printAll("vector-backed: ", vector_names);
  printAll("list-backed:   ", list_names);
}

}  // namespace classic

namespace cpp_style {

/// A fixed ring of numbers that supports range-based for and std algorithms.
class Ring {
 public:
  class iterator {  // NOLINT(readability-identifier-naming): standard naming
   public:
    using value_type = int;
    using difference_type = std::ptrdiff_t;

    iterator() = default;
    iterator(const Ring* ring, std::size_t index)
        : ring_{ring}, index_{index} {}

    int operator*() const {
      return ring_->values_[(ring_->start_ + index_) % kSize];
    }
    iterator& operator++() {
      ++index_;
      return *this;
    }
    iterator operator++(int) {
      iterator old = *this;
      ++index_;
      return old;
    }
    bool operator==(const iterator& other) const {
      return index_ == other.index_;
    }

   private:
    const Ring* ring_{nullptr};
    std::size_t index_{0};
  };

  explicit Ring(std::size_t start) : start_{start % kSize} {}
  iterator begin() const { return {this, 0}; }
  iterator end() const { return {this, kSize}; }

 private:
  static constexpr std::size_t kSize = 5;
  int values_[kSize]{10, 20, 30, 40, 50};
  std::size_t start_;
};

void run() {
  LOG_SECTION("The C++ way: begin()/end() iterators");
  const Ring ring{3};  // starts at the fourth element and wraps around
  std::string line;
  for (const int value : ring) {  // range-based for uses begin() and end()
    line += std::to_string(value) + " ";
  }
  LOG_S("  range-for over Ring(3): " << line);
  LOG_S("  std::accumulate works too: " << std::accumulate(ring.begin(),
                                                           ring.end(), 0));
}

}  // namespace cpp_style

}  // namespace

LAB_EXAMPLE("Iterator",
            "traverse collections without exposing their storage; "
            "begin()/end() in C++") {
  classic::run();
  cpp_style::run();
}
