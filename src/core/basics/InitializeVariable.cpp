#include <iostream>
using namespace std;

void initialize_variable();

// A struct that runs code when its object is created
struct InitializeVariable {
  InitializeVariable() {
    cout << "\n"
         << "\n"
         << "InitializeVariable\n";
    initialize_variable();
  }
};

// All global and static objects are constructed before main() begins.
static InitializeVariable autoRunInstance;

struct Foo {
  Foo() { cout << "Default constructor/ default init\n"; }

  // Foo(int)
  explicit Foo(int) { cout << "Constructor called with int / copy init\n"; }

  Foo(const Foo& other) { std::cout << "Copy constructor called\n"; }
};

void initialize_variable() {
  cout << "\n--- Variable Initialization Examples ---\n";
  // There are there common ways to intialize a variable
  // * Default
  [[maybe_unused]] int initDefaultVar;
  [[maybe_unused]] Foo initDefaultObj;

  // * Traditional initialization
  //   * Copy-init: Type var = value;
  //     1. Compiler tries to convert the value to a temporary Foo.
  //     2. If the constructor is explicit, implicit conversion is blocked -> compilation error.
  //     3. Otherwise, a temporary Foo is created and then copied/moved into the variable.
  [[maybe_unused]] Foo copyInitObj =
      (Foo)2.3;  // explicit cast: double 2.3 -> int 2 (implicit narrowing) -> Foo(int)
                 // -> temporary Foo -> copied/moved into copyInitObj
  // Foo copyInitObjError = 2.3; // ERROR: implicit conversion blocked by explicit constructor
  // We can explicitly prevent certain conversions using = delete or using {}

  //      * direct-init: Type var(value);
  [[maybe_unused]] Foo directInitObj(4);  // call Foo(int)
  [[maybe_unused]] Foo directInitObj2(
      4.3);  // look for constructor -> implicit 4.3(float) -> 4(int) -> call Foo(int) ->

  //      * Brace init
  // calls the constructor directly without allowing implicit conversions.
  [[maybe_unused]] Foo braceInit{3};
  // Foo braceInit2{3.3}; // ERORR => Prefer this way
}