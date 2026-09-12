// -----------------------------------------------------------------------------
// std::set - sorted collection of unique keys
//
//   template<class Key, class Compare = std::less<Key>, ...> class set;
//
// Usually a red-black tree: insert, erase and find are O(log n), and
// iteration visits the keys in sorted order.
//
//   - insert() returns {iterator, bool}: false if the key already existed.
//   - contains() (C++20) is the readable way to test membership.
//   - lower_bound / upper_bound find ranges of keys.
//   - std::multiset allows duplicate keys; std::unordered_set is hashed and
//     unsorted (O(1) on average).
//
// Reference: https://en.cppreference.com/w/cpp/container/set
// -----------------------------------------------------------------------------

#include <functional>
#include <set>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

template <typename Set>
std::string join(const Set& set) {
  std::string text;
  for (const auto& key : set) {
    text += std::to_string(key) + " ";
  }
  return text;
}

void basics() {
  LOG_SECTION("Unique and sorted");
  std::set<int> numbers{3, 4, 5, 1, 2, 3, 3};  // duplicates are dropped
  LOG_S("{3, 4, 5, 1, 2, 3, 3} -> " << join(numbers));

  const auto [position, inserted] = numbers.insert(-1);  // structured bindings
  LOG_S("insert(-1): inserted = " << std::boolalpha << inserted
                                  << ", *position = " << *position);
  const bool inserted_again = numbers.insert(-1).second;
  LOG_S("insert(-1) again: inserted = " << std::boolalpha << inserted_again);

  numbers.erase(5);
  LOG_S("erase(5) -> " << join(numbers));
  LOG_S("contains(4) = " << std::boolalpha << numbers.contains(4)
                         << ", contains(5) = " << numbers.contains(5));
}

void ranges() {
  LOG_SECTION("Range queries");
  const std::set<int> scores{10, 25, 40, 55, 70, 85};
  // [lower_bound(30), upper_bound(70)) -> every key in 30..70
  auto first = scores.lower_bound(30);       // first key >= 30
  const auto last = scores.upper_bound(70);  // first key > 70
  std::string in_range;
  for (; first != last; ++first) {
    in_range += std::to_string(*first) + " ";
  }
  LOG_S("scores between 30 and 70: " << in_range);
}

void customOrder() {
  LOG_SECTION("Custom comparator and std::multiset");
  const std::set<int, std::greater<>> descending{3, 1, 4, 1, 5};
  LOG_S("std::set<int, std::greater<>>: " << join(descending));

  const std::multiset<int> with_duplicates{3, 1, 4, 1, 5};
  LOG_S("std::multiset<int>:            "
        << join(with_duplicates) << "(count(1) = " << with_duplicates.count(1)
        << ")");
}

}  // namespace

LAB_EXAMPLE("Set",
            "std::set: unique sorted keys, insert result, ranges, multiset") {
  basics();
  ranges();
  customOrder();
}
