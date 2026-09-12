// -----------------------------------------------------------------------------
// Pointers
//
//   int* p = &x;    p holds the ADDRESS of x;  *p reads/writes x itself
//
//   const int* p          pointer to const : *p cannot change, p can
//   int* const p          const pointer    : p cannot change, *p can
//   const int* const p    both are fixed
//   (read right to left: "p is a const pointer to an int that is const")
//
// Pitfalls: dereferencing nullptr or a dangling pointer (to an object that no
// longer exists) is undefined behavior. For ownership use smart pointers
// (see core/smart_pointer); raw pointers should only observe.
//
// Reference: https://en.cppreference.com/w/cpp/language/pointer
// -----------------------------------------------------------------------------

#include <cstddef>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void basics() {
  LOG_SECTION("Address-of & and dereference *");
  int value = 10;
  int* pointer = &value;
  LOG_S("value = " << value << ", &value = " << &value
                   << ", pointer = " << pointer);
  LOG_S("*pointer = " << *pointer);

  *pointer = 20;  // writes through the pointer
  LOG_S("after *pointer = 20, value = " << value);

  const int* nothing = nullptr;  // points to no object
  LOG_S("nullptr pointer = " << nothing << " - check before dereferencing");
}

void constCombinations() {
  LOG_SECTION("const and pointers");
  int a = 1;
  int b = 2;

  const int* to_const = &a;  // pointer to const
  // *to_const = 5;          // error: the pointee is read-only through this pointer
  to_const = &b;  // OK: the pointer itself may change
  LOG_S("const int* to_const       -> *to_const = " << *to_const);

  int* const const_pointer = &a;  // const pointer
  *const_pointer = 5;             // OK: the pointee may change
  // const_pointer = &b;          // error: the pointer is fixed
  LOG_S("int* const const_pointer  -> a is now " << a);

  const int* const both = &b;  // nothing may change
  LOG_S("const int* const both     -> *both = " << *both);
}

void arithmetic() {
  LOG_SECTION("Pointer arithmetic");
  int values[] = {10, 20, 30, 40};
  int* first = values;     // array decays to a pointer to its first element
  int* third = first + 2;  // moves by 2 ELEMENTS, not 2 bytes
  LOG_S("*(first + 2) = " << *third);
  LOG_S("third - first = " << third - first << " elements ("
                           << static_cast<std::ptrdiff_t>(sizeof(int)) *
                                  (third - first)
                           << " bytes)");
  ++third;
  LOG_S("after ++third: *third = " << *third);
  // first + 4 may be computed (one past the end) but must not be dereferenced.
}

void pointerToPointer() {
  LOG_SECTION("Pointer to pointer");
  int value = 7;
  int* pointer = &value;
  int** pointer_to_pointer = &pointer;
  **pointer_to_pointer = 70;
  LOG_S("**pointer_to_pointer = 70 -> value = " << value);
}

void voidPointer() {
  LOG_SECTION("void* (generic pointer)");
  int value = 42;
  void* generic = &value;  // any object pointer converts to void*
  // *generic;             // error: cannot dereference void*
  const int* back = static_cast<int*>(generic);  // you must know the real type
  LOG_S("static_cast<int*>(generic) -> " << *back);
}

int twice(int x) {
  return 2 * x;
}

void increment(int* counter) {
  if (counter != nullptr) {  // pointers can be null, references cannot
    ++*counter;
  }
}

int* globalCounter() {
  static int counter = 100;  // lives for the whole program
  return &counter;           // never return the address of a local variable!
}

void functionsAndParameters() {
  LOG_SECTION("Function pointers and pointer parameters");
  int (*operation)(int) =
      &twice;  // pointer to a function taking int, returning int
  LOG_S("(*operation)(5) = " << (*operation)(5)
                             << ", operation(6) = " << operation(6));

  int counter = 0;
  increment(&counter);
  increment(nullptr);  // safely ignored
  LOG_S("increment(&counter) -> counter = " << counter);

  const int* global = globalCounter();
  LOG_S("*globalCounter() = " << *global);
}

}  // namespace

LAB_EXAMPLE("Pointer",
            "address-of, dereference, const pointers, arithmetic, void*, "
            "function pointers") {
  basics();
  constCombinations();
  arithmetic();
  pointerToPointer();
  voidPointer();
  functionsAndParameters();
}
