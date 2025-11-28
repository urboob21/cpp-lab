// cppcheck-suppress-file [noCopyConstructor, noOperatorEq, unsafeClassCanLeak]

#include <iostream>

namespace {
namespace Shallow {
class Model {
 private:
  int m_x{0}, m_y{1};
  int* ptr;

 public:
  // Default constructor: Model a = Model(1,2)
  explicit Model(int x, int y, int z) : m_x{x}, m_y{y}, ptr{nullptr} {
    ptr = new int;
    *ptr = z;
  }

  ~Model() {
    // [P1]
    // Shallow copying makes both objects use the same pointer.
    // If one object deletes the pointer, the other object now
    // points to invalid memory.
    // delete ptr;   // Commented out on purpose for demo
  }

  void changePtr(int value) {
    if (ptr != nullptr) {
      *ptr = value;
    }
  }

  int getPtr() const {
    if (ptr != nullptr) {
      return *ptr;
    }
    return 0;
  }

  // Implicit copy constructor: Model a{b};
  // Model(const Model &f) : m_x{f.m_x}, m_y{f.m_y} {}

  // Implicit assignment operator (like the way compiler gen this function):
  // Model a = b; Model &operator=(const Model &f)
  // {
  //     // self-assignment guard
  //     if (this == &f)
  //     {
  //         return *this;
  //     }

  //     // do the copy
  //     m_x = f.m_x;
  //     m_y = f.m_y;

  //     //  return the existing object so we can chain this operator
  //     return *this;
  // }
};

void run() {
  Model obj1 = Model(1, 2, 3);
  Model obj2 = obj1;
  obj2.changePtr(30);
  // [P2]
  std::cout << "\n=== Shallow Copy Demo ===\n";
  std::cout << "obj1.ptr = " << obj1.getPtr() << "\n";
  std::cout << "obj2.ptr = " << obj2.getPtr() << "\n";
}

}  // namespace Shallow

// To do a deep copy on any non-null pointers being copied
// Requires that we write our own `copy constructors` and `overloaded assignment
// operators`.
namespace DeepCopying {
class Model {
 private:
  int m_x{0}, m_y{1};
  int* ptr;

  void deepCopy(const Model& source) {
    // sallow copy the normal fields
    m_x = source.m_x;
    m_y = source.m_y;

    // deep copy the ptr field
    // m_data is a pointer, so we need to deep copy it if it is non-null
    // first we need to deallocate any value that this Model is holding!
    delete ptr;
    if (source.ptr != nullptr) {
      // allocate memory for our copy
      ptr = new int;
      // do the copy
      *ptr = *source.ptr;
    } else {
      ptr = nullptr;
    }
  }

 public:
  // Constructor
  explicit Model(int x, int y, int z) : m_x{x}, m_y{y}, ptr{nullptr} {
    ptr = new int;
    *ptr = z;
  }

  // Destructor
  ~Model() { delete ptr; }

  // Copy constructor
  // no need to check self-copy [if (this != &source)]
  // because it cannot happen a(a);
  Model(const Model& source) { this->deepCopy(source); }

  // Assignment operator
  // need to check self-copy [if (this != &source)]
  // because it posible a = a;
  Model& operator=(const Model& source) {
    if (this != &source) {
      // now do the deep copy
      this->deepCopy(source);
    }
    return *this;
  }

  void changePtr(int value) {
    if (ptr != nullptr) {
      *ptr = value;
    }
  }

  int getPtr() const {
    if (ptr != nullptr) {
      return *ptr;
    }
    return 0;
  }
};

void run() {
  Model obj1 = Model(1, 2, 3);
  Model obj2 = obj1;
  obj2.changePtr(30);

  std::cout << "\n=== Deep Copy Demo ===\n";
  std::cout << "obj1.ptr = " << obj1.getPtr() << "\n";
  std::cout << "obj2.ptr = " << obj2.getPtr() << "\n";
}

}  // namespace DeepCopying
}  // namespace

struct ShallowDeepCopying {
  ShallowDeepCopying() {
    Shallow::run();
    DeepCopying::run();
  }
};

static ShallowDeepCopying instance;