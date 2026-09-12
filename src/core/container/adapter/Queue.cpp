// -----------------------------------------------------------------------------
// std::queue - first in, first out (FIFO)
//
//   template<class T, class Container = std::deque<T>> class queue;
//
// A container ADAPTER: it wraps a sequence container (std::deque by default,
// or std::list) and only exposes queue operations:
//   push() adds at the back, pop() removes from the front,
//   front() / back() access the ends. There are no iterators.
//
// Note: pop() returns void. Read front() first, then pop().
//
// Reference: https://en.cppreference.com/w/cpp/container/queue
// -----------------------------------------------------------------------------

#include <list>
#include <queue>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void basics() {
  LOG_SECTION("push / pop / front / back");
  std::queue<int> queue;

  for (int value = 0; value < 4; ++value) {
    queue.push(value);  // queue: 0 1 2 3
  }
  LOG_S("size = " << queue.size() << ", front = " << queue.front()
                  << ", back = " << queue.back());

  queue.pop();  // removes 0 -> queue: 1 2 3
  LOG_S("after pop(): front = " << queue.front());

  std::string drained;
  while (!queue.empty()) {
    drained += std::to_string(queue.front()) + " ";
    queue.pop();
  }
  LOG_S("drained in FIFO order: " << drained);
}

void otherContainer() {
  LOG_SECTION("A queue on top of std::list");
  std::queue<std::string, std::list<std::string>> tasks;
  tasks.emplace("compile");  // emplace constructs the element in place
  tasks.emplace("test");
  tasks.emplace("deploy");

  while (!tasks.empty()) {
    LOG_S("next task: " << tasks.front());
    tasks.pop();
  }
}

}  // namespace

LAB_EXAMPLE("Queue", "std::queue: FIFO container adapter") {
  basics();
  otherContainer();
}
