// -----------------------------------------------------------------------------
// Overloading operator new and operator delete
//
// `new T` does two things: calls `operator new(sizeof(T))` to get raw memory,
// then runs T's constructor in it. `delete p` runs the destructor, then calls
// `operator delete(p)`. Only the memory part can be customized.
//
//   - Class-specific overloads (static members) affect only that class:
//     logging, memory pools, alignment, debugging leaks.
//   - Overload new/delete AND new[]/delete[] together.
//   - operator new must throw std::bad_alloc on failure (or be nothrow).
//   - Placement new `new (buffer) T` constructs in memory you already own.
//
// Reference: https://en.cppreference.com/w/cpp/memory/new/operator_new
// -----------------------------------------------------------------------------

#include <cstddef>
#include <memory>
#include <new>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Tracked {
 public:
  Tracked() { LOG("    Tracked()   constructor"); }
  ~Tracked() { LOG("    ~Tracked()  destructor"); }
  Tracked(const Tracked&) = delete;
  Tracked& operator=(const Tracked&) = delete;

  static void* operator new(std::size_t size) {
    LOG_S("    operator new(" << size << ")");
    return ::operator new(size);  // delegate to the global allocator
  }

  static void operator delete(void* memory) noexcept {
    LOG("    operator delete");
    ::operator delete(memory);
  }

  static void* operator new[](std::size_t size) {
    LOG_S("    operator new[](" << size
                                << ")  (may include bookkeeping bytes)");
    return ::operator new[](size);
  }

  static void operator delete[](void* memory) noexcept {
    LOG("    operator delete[]");
    ::operator delete[](memory);
  }

 private:
  int data_[3]{10, 20, 30};
};

void singleObject() {
  LOG_SECTION("new / delete: memory first, then constructor");
  LOG("  new Tracked:");
  auto* object = new Tracked;
  LOG("  delete object:");
  delete object;

  LOG("  std::make_unique uses the class operator new as well:");
  auto owned = std::make_unique<Tracked>();
  owned.reset();
}

void arrays() {
  LOG_SECTION("new[] / delete[]");
  LOG_S("  sizeof(Tracked) = " << sizeof(Tracked) << ", allocating 3:");
  auto* objects = new Tracked[3];
  LOG("  delete[] objects:");
  delete[] objects;  // mixing new[] with delete (or new with delete[]) is UB
}

void placementNew() {
  LOG_SECTION("Placement new: construct in existing memory");
  alignas(Tracked) std::byte buffer[sizeof(Tracked)];
  LOG("  new (buffer) Tracked  -> no allocation, only the constructor runs:");
  Tracked* object =
      ::new (buffer) Tracked;  // ::new bypasses the class overload
  LOG("  object->~Tracked()     -> call the destructor manually, never "
      "delete:");
  object->~Tracked();
}

void nothrowNew() {
  LOG_SECTION("std::nothrow");
  // The nothrow form returns nullptr instead of throwing std::bad_alloc.
  const std::unique_ptr<int[]> numbers(new (std::nothrow) int[16]);
  LOG_S("  new (std::nothrow) int[16] -> "
        << (numbers ? "allocated" : "nullptr"));
}

}  // namespace

LAB_EXAMPLE(
    "AllocationOperator",
    "class-specific operator new/delete, new[]/delete[], placement new") {
  singleObject();
  arrays();
  placementNew();
  nothrowNew();
}
