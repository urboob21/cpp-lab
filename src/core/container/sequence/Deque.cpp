// -----------------------------------------------------------------------------
// std::deque - double-ended queue
//
//   - O(1) insertion and removal at BOTH ends, O(1) random access.
//   - Elements live in fixed-size blocks, not in one contiguous buffer: there
//     is no data(), and indexing is a little slower than std::vector.
//   - push_front / push_back never move existing elements, so references and
//     pointers to them stay valid (iterators do not).
//   - Insertion or removal in the middle is O(n).
//
// Reference: https://en.cppreference.com/w/cpp/container/deque
// -----------------------------------------------------------------------------

#include <deque>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

std::string join(const std::deque<int>& deque) {
  std::string text;
  for (const int element : deque) {
    text += std::to_string(element) + " ";
  }
  return text;
}

void bothEnds() {
  LOG_SECTION("Adding and removing at both ends");
  std::deque<int> deque{1, 2, 3};
  LOG_S("start          : " << join(deque));

  deque.push_front(0);
  deque.push_back(4);
  LOG_S("push_front(0), push_back(4): " << join(deque));

  deque.pop_front();
  deque.pop_back();
  LOG_S("pop_front(), pop_back()    : " << join(deque));

  deque.insert(deque.begin() + 1, 99);  // O(n) in the middle
  LOG_S("insert at index 1          : " << join(deque));
  LOG_S("random access deque[2]     = " << deque[2]);
}

void referenceStability() {
  LOG_SECTION("References survive push_back / push_front");
  std::deque<int> deque{42};
  const int& first = deque.front();

  for (int i = 0; i < 10'000; ++i) {
    deque.push_back(i);
    deque.push_front(-i);
  }
  // With std::vector, growing would reallocate and `first` would dangle.
  LOG_S("after 20000 insertions, `first` still reads "
        << first << ", size = " << deque.size());
}

}  // namespace

LAB_EXAMPLE("Deque",
            "std::deque: fast insertion at both ends, stable references") {
  bothEnds();
  referenceStability();
}
