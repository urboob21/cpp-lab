// -----------------------------------------------------------------------------
// std::unordered_map - hash table of unique keys to values
//
//   - Average O(1) insert, find and erase (O(n) worst case).
//   - Iteration order is unspecified and can change after a rehash.
//   - operator[] INSERTS a default value when the key is missing, so it is not
//     available on a const map. Use find() / contains() to only look.
//   - insert / emplace / try_emplace keep an existing value;
//     insert_or_assign and operator[] overwrite it.
//
// Reference: https://en.cppreference.com/w/cpp/container/unordered_map
// -----------------------------------------------------------------------------

#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

using Inventory = std::unordered_map<std::string, int>;

/// Prints the map sorted by key, because iteration order is unspecified.
std::string sorted(const Inventory& inventory) {
  std::vector<std::pair<std::string, int>> entries(inventory.begin(),
                                                   inventory.end());
  std::sort(entries.begin(), entries.end());
  std::string text;
  for (const auto& [key, value] : entries) {  // structured bindings
    text += "[" + key + ": " + std::to_string(value) + "] ";
  }
  return text;
}

void insertion() {
  LOG_SECTION("Inserting");
  Inventory inventory{{"apple", 5}, {"banana", 2}};
  inventory.insert({"cherry", 7});
  inventory.emplace("date", 1);
  inventory["elderberry"] = 3;
  LOG_S("inventory: " << sorted(inventory));

  const bool inserted = inventory.insert({"apple", 100}).second;
  LOG_S("insert(apple, 100)            -> inserted = "
        << std::boolalpha << inserted << ", apple = " << inventory["apple"]);

  inventory.try_emplace("apple", 200);  // also keeps the existing value
  LOG_S("try_emplace(apple, 200)       -> apple = " << inventory["apple"]);

  inventory.insert_or_assign("apple", 300);  // overwrites
  LOG_S("insert_or_assign(apple, 300)  -> apple = " << inventory["apple"]);
}

void lookup() {
  LOG_SECTION("Looking up without inserting");
  Inventory inventory{{"apple", 5}};

  if (const auto it = inventory.find("apple"); it != inventory.end()) {
    LOG_S("find(apple)     -> " << it->second);
  }
  LOG_S("contains(kiwi)  -> " << std::boolalpha << inventory.contains("kiwi"));

  try {
    const Inventory& read_only = inventory;
    LOG_S("at(kiwi)        -> " << read_only.at("kiwi"));
  } catch (const std::out_of_range&) {
    LOG("at(kiwi)        -> throws std::out_of_range");
  }

  LOG_S("operator[](kiwi) -> " << inventory["kiwi"]
                               << "  <- pitfall: silently inserted kiwi = 0");
  LOG_S("size is now " << inventory.size());
}

void erasing() {
  LOG_SECTION("Erasing while iterating");
  Inventory inventory{{"apple", 0}, {"banana", 2}, {"cherry", 0}, {"date", 4}};
  for (auto it = inventory.begin(); it != inventory.end();) {
    if (it->second == 0) {
      it = inventory.erase(it);  // erase returns the next valid iterator
    } else {
      ++it;
    }
  }
  LOG_S("removed empty items: " << sorted(inventory));
  // C++20 shortcut: std::erase_if(inventory, [](const auto& e) { return e.second == 0; });
}

void wordCount() {
  LOG_SECTION("Use case: counting words");
  std::istringstream text(
      "the quick brown fox jumps over the lazy dog the end");
  Inventory counts;
  std::string word;
  while (text >> word) {
    ++counts[word];  // here operator[] inserting 0 is exactly what we want
  }
  LOG_S("counts: " << sorted(counts));
  LOG_S("buckets = " << counts.bucket_count()
                     << ", load factor = " << counts.load_factor());
}

}  // namespace

LAB_EXAMPLE(
    "UnorderedMap",
    "std::unordered_map: insert vs assign, lookup pitfalls, erase, buckets") {
  insertion();
  lookup();
  erasing();
  wordCount();
}
