// -----------------------------------------------------------------------------
// Standard algorithms (<algorithm>, <numeric>) and C++20 ranges
//
// Prefer a named algorithm over a raw loop: the name states the intent, the
// implementation is tested, and it is often faster.
//
//   searching   find, find_if, any_of, all_of, none_of, count_if
//   ordering    sort, stable_sort, partition, binary_search, lower_bound
//   modifying   transform, remove_if + erase (or std::erase_if), unique, fill
//   numeric     accumulate, reduce, iota, inner_product
//   ranges      std::ranges::sort(v), v | std::views::filter(...) | ... (C++20)
//
// Reference: https://en.cppreference.com/w/cpp/algorithm
// -----------------------------------------------------------------------------

#include <algorithm>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

std::string join(const std::vector<int>& values) {
  std::string text;
  for (const int value : values) {
    text += std::to_string(value) + " ";
  }
  return text;
}

bool isEven(int value) {
  return value % 2 == 0;
}

void searching() {
  LOG_SECTION("Searching and counting");
  const std::vector<int> values{1, 3, 4, 7, 8, 11};
  LOG_S("values: " << join(values));

  if (const auto it = std::find_if(values.begin(), values.end(), isEven);
      it != values.end()) {
    LOG_S("find_if(even)     -> " << *it << " at index "
                                  << it - values.begin());
  }
  if (const auto it = std::find_if_not(values.begin(), values.end(),
                                       [](int v) { return v < 5; });
      it != values.end()) {
    LOG_S("find_if_not(< 5)  -> " << *it);
  }
  LOG_S("count_if(even)    -> "
        << std::count_if(values.begin(), values.end(), isEven));
  LOG_S(std::boolalpha << "any_of(> 10) "
                       << std::any_of(values.begin(), values.end(),
                                      [](int v) { return v > 10; })
                       << ", all_of(> 0) "
                       << std::all_of(values.begin(), values.end(),
                                      [](int v) { return v > 0; })
                       << ", none_of(< 0) "
                       << std::none_of(values.begin(), values.end(),
                                       [](int v) { return v < 0; }));
}

void ordering() {
  LOG_SECTION("Sorting and searching sorted data");
  std::vector<int> values{5, 3, 9, 3, 1, 9, 7};
  std::sort(values.begin(), values.end());
  LOG_S("sort                 -> " << join(values));

  values.erase(std::unique(values.begin(), values.end()), values.end());
  LOG_S("unique + erase       -> " << join(values)
                                   << "(removes adjacent duplicates)");

  LOG_S(std::boolalpha << "binary_search(7)     -> "
                       << std::binary_search(values.begin(), values.end(), 7));
  const auto position = std::lower_bound(values.begin(), values.end(), 6);
  LOG_S("lower_bound(6)       -> " << *position << " (first element >= 6)");

  const auto [smallest, largest] =
      std::minmax_element(values.begin(), values.end());
  LOG_S("minmax_element       -> " << *smallest << " and " << *largest);

  std::vector<int> mixed{1, 2, 3, 4, 5, 6};
  std::partition(mixed.begin(), mixed.end(), isEven);
  LOG_S("partition(even)      -> " << join(mixed)
                                   << "(evens first, order not kept)");
}

void modifyingAndNumeric() {
  LOG_SECTION("Transforming and numeric algorithms");
  std::vector<int> values(5);
  std::iota(values.begin(), values.end(), 1);  // 1 2 3 4 5
  LOG_S("iota(1)               -> " << join(values));

  std::vector<int> squares(values.size());
  std::transform(values.begin(), values.end(), squares.begin(),
                 [](int v) { return v * v; });
  LOG_S("transform(square)     -> " << join(squares));

  LOG_S("accumulate(+)         -> "
        << std::accumulate(values.begin(), values.end(), 0));
  LOG_S("accumulate(*)         -> " << std::accumulate(
            values.begin(), values.end(), 1, std::multiplies<>{}));
  LOG_S("inner_product         -> " << std::inner_product(
            values.begin(), values.end(), squares.begin(), 0));

  std::erase_if(squares, [](int v) {
    return v > 10;
  });  // C++20, replaces remove_if + erase
  LOG_S("erase_if(> 10)        -> " << join(squares));
}

void ranges() {
  LOG_SECTION("C++20 ranges and views");
  std::vector<int> values{8, 3, 5, 1, 9, 2, 6};

  std::ranges::sort(values);  // pass the container, not two iterators
  LOG_S("ranges::sort          -> " << join(values));

  // Views are lazy: nothing is computed until the loop pulls the elements.
  auto pipeline = values | std::views::filter(isEven) |
                  std::views::transform([](int v) { return v * 10; });
  std::string result;
  for (const int value : pipeline) {
    result += std::to_string(value) + " ";
  }
  LOG_S("filter(even) | x10    -> " << result);

  std::string first_three;
  for (const int value : values | std::views::reverse | std::views::take(3)) {
    first_three += std::to_string(value) + " ";
  }
  LOG_S("reverse | take(3)     -> " << first_three);
}

}  // namespace

LAB_EXAMPLE("Algorithm",
            "find/count/sort/transform/accumulate and C++20 ranges pipelines") {
  searching();
  ordering();
  modifyingAndNumeric();
  ranges();
}
