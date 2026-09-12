// -----------------------------------------------------------------------------
// std::vector - dynamic contiguous array (the default container)
//
//   - size() is the number of elements, capacity() the allocated room.
//   - When size would exceed capacity, the vector allocates a bigger buffer
//     and moves every element into it.
//   - reserve() allocates up front when the final size is known.
//   - Reallocation invalidates ALL pointers, references and iterators.
//   - emplace_back constructs in place; push_back copies/moves an object.
//   - C++20 std::erase / std::erase_if replace the erase-remove idiom.
//
// Reference: https://en.cppreference.com/w/cpp/container/vector
// -----------------------------------------------------------------------------

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

template <typename T>
std::string join(const std::vector<T>& vector) {
  std::string text;
  for (const T& element : vector) {
    text += std::to_string(element) + " ";
  }
  return text;
}

void initialization() {
  LOG_SECTION("Initialization");
  const std::vector<int> listed{9, 100, 0};  // initializer list
  const std::vector<int> repeated(3, 7);     // 3 elements with value 7
  const std::vector<int> empty;
  std::vector<int> assigned;
  assigned = {1, 2, 3, 5, 6};  // unlike std::array the size can change
  // std::vector<int> oops();  // most vexing parse: declares a function!

  LOG_S("listed{9, 100, 0} -> " << join(listed));
  LOG_S("repeated(3, 7)    -> " << join(repeated));
  LOG_S("empty.size()      =  " << empty.size());
  LOG_S("assigned          -> " << join(assigned));
}

void growth() {
  LOG_SECTION("size() vs capacity()");
  std::vector<int> numbers;
  std::size_t last_capacity = numbers.capacity();
  for (int i = 0; i < 20; ++i) {
    numbers.push_back(i);
    if (numbers.capacity() != last_capacity) {
      LOG_S("size " << numbers.size() << ": reallocated, capacity "
                    << last_capacity << " -> " << numbers.capacity());
      last_capacity = numbers.capacity();
    }
  }

  std::vector<int> reserved;
  reserved.reserve(20);  // one allocation, no reallocation below
  const auto before = reinterpret_cast<std::uintptr_t>(reserved.data());
  for (int i = 0; i < 20; ++i) {
    reserved.push_back(i);
  }
  LOG_S("with reserve(20): buffer unchanged = "
        << std::boolalpha
        << (before == reinterpret_cast<std::uintptr_t>(reserved.data())));

  numbers.shrink_to_fit();  // non-binding request to release unused capacity
  LOG_S("after shrink_to_fit: size " << numbers.size() << ", capacity "
                                     << numbers.capacity());
}

void modifiers() {
  LOG_SECTION("Modifiers");
  std::vector<int> numbers{1, 2, 3};

  numbers.insert(numbers.begin(), 0);  // O(n): shifts every element
  numbers.push_back(4);
  numbers.emplace_back(5);
  LOG_S("insert front, push_back, emplace_back -> " << join(numbers));

  numbers.erase(numbers.begin() + 2);  // removes the element at index 2
  numbers.pop_back();
  LOG_S("erase index 2, pop_back              -> " << join(numbers));

  std::vector<int> mixed{1, 2, 3, 4, 5, 6, 7, 8};
  std::erase_if(mixed, [](int value) { return value % 2 == 0; });  // C++20
  LOG_S("std::erase_if(even)                  -> " << join(mixed));

  std::vector<std::pair<std::string, int>> people;
  people.emplace_back("Alice",
                      30);  // pair constructed directly inside the vector
  LOG_S("emplace_back(\"Alice\", 30) -> " << people.front().first << ", "
                                          << people.front().second);
}

void invalidation() {
  LOG_SECTION("Iterator invalidation");
  std::vector<int> numbers{1, 2, 3};
  numbers.shrink_to_fit();
  // Kept as an integer: after the reallocation the old POINTER value must not
  // be used any more, not even for a comparison.
  const auto old_address = reinterpret_cast<std::uintptr_t>(numbers.data());

  numbers.push_back(4);  // capacity was full -> reallocation
  LOG_S("buffer moved after push_back: "
        << std::boolalpha
        << (old_address != reinterpret_cast<std::uintptr_t>(numbers.data())));
  LOG("Any pointer, reference or iterator taken before is now dangling.");
  LOG("Take new ones after modifying, or reserve() enough capacity first.");
}

}  // namespace

LAB_EXAMPLE(
    "Vector",
    "std::vector: growth and capacity, modifiers, erase_if, invalidation") {
  initialization();
  growth();
  modifiers();
  invalidation();
}
