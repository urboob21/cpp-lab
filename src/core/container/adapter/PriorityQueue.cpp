// -----------------------------------------------------------------------------
// std::priority_queue - always pop the "largest" element first
//
//   template<class T, class Container = std::vector<T>,
//            class Compare = std::less<typename Container::value_type>>
//   class priority_queue;
//
// A container ADAPTER implemented as a binary heap on top of std::vector:
// push() and pop() are O(log n), top() is O(1).
//
//   - Default (std::less)  : max-heap, top() is the largest element.
//   - std::greater<T>      : min-heap, top() is the smallest element.
//   - Custom comparator    : "a < b" means a has LOWER priority than b.
//
// Reference: https://en.cppreference.com/w/cpp/container/priority_queue
// -----------------------------------------------------------------------------

#include <functional>
#include <queue>
#include <string>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void maxAndMinHeap() {
  LOG_SECTION("Max-heap and min-heap");
  const std::vector<int> values{5, 1, 8, 3, 9, 2};

  std::priority_queue<int> max_heap(values.begin(), values.end());
  std::priority_queue<int, std::vector<int>, std::greater<>> min_heap(
      values.begin(), values.end());

  std::string max_order;
  std::string min_order;
  while (!max_heap.empty()) {
    max_order += std::to_string(max_heap.top()) + " ";
    max_heap.pop();
    min_order += std::to_string(min_heap.top()) + " ";
    min_heap.pop();
  }
  LOG_S("max-heap pops: " << max_order);
  LOG_S("min-heap pops: " << min_order);
}

struct Task {
  std::string name;
  int priority;
};

void customComparator() {
  LOG_SECTION("Custom comparator: a task scheduler");

  // Returns true when `a` should be served AFTER `b`.
  auto lower_priority = [](const Task& a, const Task& b) {
    return a.priority < b.priority;
  };
  std::priority_queue<Task, std::vector<Task>, decltype(lower_priority)> tasks(
      lower_priority);

  tasks.push({"write docs", 1});
  tasks.push({"fix crash", 10});
  tasks.push({"code review", 5});

  while (!tasks.empty()) {
    LOG_S("serve '" << tasks.top().name << "' (priority "
                    << tasks.top().priority << ")");
    tasks.pop();
  }
}

}  // namespace

LAB_EXAMPLE("PriorityQueue",
            "std::priority_queue: max-heap, min-heap, custom priority") {
  maxAndMinHeap();
  customComparator();
}
