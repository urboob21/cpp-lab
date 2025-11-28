#include <iostream>
using namespace std;

// *1. Basic Destructor
// Generated if no destructor is declared
// Calls destructors of members automatically
// Does not free dynamically allocated memory unless you write it
namespace Basic {
class CDestructors {
 public:
  CDestructors() { cout << "Called CDestructors() \n"; }

  ~CDestructors() { cout << "Called ~CDestructors() \n"; }

  // Using `default` keyword
  // ~CDestructors() = default;

  // Using `delete` // I forbid this destructor
  // ~CDestructors() = delete;
};

void destructers() {
  cout << "\n--- Basic Destructer Examples ---\n";
  { CDestructors obj; }
}
}  // namespace Basic

// *2. Virtual Destructor
namespace Virtual {
class CDestructorsBase  // final => cannot inherit
{
 public:
  CDestructorsBase() { cout << "Called CDestructorsBase() \n"; }

  virtual ~CDestructorsBase() { cout << "Called ~CDestructorsBase() \n"; }

  // Using `default` keyword
  // ~CDestructorsBase() = default;
};

class CDestructorsDerived : public CDestructorsBase {
 public:
  CDestructorsDerived() { cout << "Called CDestructorsDerived() \n"; }

  ~CDestructorsDerived() override {
    cout << "Called ~CDestructorsDerived() \n";
  }
};

void destructers() {
  cout << "\n--- Virtual Destructer Examples ---\n";
  CDestructorsDerived* derived = {new CDestructorsDerived()};
  CDestructorsBase* base{derived};
  delete base;
  // without virtual -> only call ~CDestructorsBase()
  // with virtual -> call ~CDestructorsBase() && ~CDestructorsDerived()
}
}  // namespace Virtual

struct CDestructorsAutoRuner {
  CDestructorsAutoRuner() {
    Basic::destructers();
    Virtual::destructers();
  }
};

static CDestructorsAutoRuner instance;
