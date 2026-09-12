// -----------------------------------------------------------------------------
// std::array - a fixed-size array with a container interface
//
//   template<class T, std::size_t N> struct array;
//
//   - The size is part of the type and fixed at compile time; the elements are
//     stored inline (no heap allocation).
//   - Unlike a C array it never decays to a pointer, knows its size(), and can
//     be copied, compared, and returned from functions.
//   - operator[] is unchecked; at() throws std::out_of_range.
//
// Reference: https://en.cppreference.com/w/cpp/container/array
// -----------------------------------------------------------------------------

#include <algorithm>
#include <array>
#include <numeric>
#include <stdexcept>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

template <typename Range>
std::string join(const Range& range) {
  std::string text;
  for (const auto& element : range) {
    text += std::to_string(element) + " ";
  }
  return text;
}

void initialization() {
  LOG_SECTION("Initialization");
  const std::array<int, 3> values{9, 100, 0};
  const std::array<int, 3> zeros{};  // every element value-initialized: 0 0 0
  const std::array<int, 3> partial{1};  // missing elements are zero: 1 0 0
  [[maybe_unused]] std::array<int, 3>
      garbage;  // ints are NOT initialized - don't read
  const auto deduced =
      std::array{1.5, 2.5};  // C++17 deduction: std::array<double, 2>
  // std::array<int, 3> too_many{1, 2, 3, 4};   // error: too many initializers

  LOG_S("values{9, 100, 0} -> " << join(values));
  LOG_S("zeros{}           -> " << join(zeros));
  LOG_S("partial{1}        -> " << join(partial));
  LOG_S("std::array{1.5, 2.5} has size " << deduced.size());
}

void elementAccess() {
  LOG_SECTION("Element access");
  std::array<int, 3> values{9, 100, 0};
  LOG_S("front() = " << values.front() << ", back() = " << values.back()
                     << ", values[1] = " << values[1]);

  const int* raw = values.data();  // pointer to the first element (C interop)
  LOG_S("data()[2] = " << raw[2]);

  try {
    values.at(3) = 1;  // bounds-checked
  } catch (const std::out_of_range& e) {
    LOG_S("at(3) threw std::out_of_range: " << e.what());
  }
  // values[3] = 1;  // NOT checked: undefined behavior
}

void iteration() {
  LOG_SECTION("Iteration");
  const std::array<int, 4> values{1, 2, 3, 4};

  std::string by_index;
  for (std::size_t i = 0; i < values.size(); ++i) {
    by_index += std::to_string(values[i]) + " ";
  }
  LOG_S("index loop   : " << by_index);
  LOG_S("range-for    : " << join(values));

  std::string reversed;
  for (auto it = values.rbegin(); it != values.rend(); ++it) {
    reversed += std::to_string(*it) + " ";
  }
  LOG_S("rbegin..rend : " << reversed);
}

void operations() {
  LOG_SECTION("Operations");
  std::array<int, 5> values{5, 3, 9, 1, 7};
  std::sort(values.begin(), values.end());
  LOG_S("sorted        : " << join(values));
  LOG_S("sum           : " << std::accumulate(values.begin(), values.end(), 0));

  const std::array<int, 5> copy = values;  // arrays copy like any value type
  LOG_S("copy == values: " << std::boolalpha << (copy == values));

  const auto [first, second, third, fourth, fifth] =
      values;  // structured bindings
  LOG_S("structured bindings: first = " << first << ", fifth = " << fifth
                                        << " (" << second + third + fourth
                                        << " in between)");

  values.fill(0);
  LOG_S("after fill(0) : " << join(values));
  LOG_S("size() = max_size() = " << values.max_size()
                                 << " (fixed at compile time)");
}

void board() {
  LOG_SECTION("A 2D board: std::array of std::array");
  constexpr std::size_t kSide = 4;
  using Row = std::array<const char*, kSide>;
  std::array<Row, kSide> cells{};

  for (std::size_t row = 0; row < kSide; ++row) {
    for (std::size_t col = 0; col < kSide; ++col) {
      cells[row][col] = (row + col) % 2 == 0 ? "##" : "  ";
    }
  }
  for (const Row& row : cells) {
    std::string line;
    for (const char* cell : row) {
      line += cell;
    }
    LOG(line);
  }
}

}  // namespace

LAB_EXAMPLE("Array",
            "std::array: fixed-size container, access, iteration, algorithms") {
  initialization();
  elementAccess();
  iteration();
  operations();
  board();
}
