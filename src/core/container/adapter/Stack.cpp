// -----------------------------------------------------------------------------
// std::stack - last in, first out (LIFO)
//
//   template<class T, class Container = std::deque<T>> class stack;
//
// A container ADAPTER over std::deque (default), std::vector or std::list.
// push() and pop() work on the "top" (the back of the underlying container),
// top() reads it. There are no iterators.
//
// Typical uses: undo history, expression evaluation, depth-first search.
//
// Reference: https://en.cppreference.com/w/cpp/container/stack
// -----------------------------------------------------------------------------

#include <stack>
#include <string>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void basics() {
  LOG_SECTION("push / pop / top");
  std::stack<int> stack;

  for (int value = 0; value < 4; ++value) {
    stack.push(value);  // bottom -> top: 0 1 2 3
  }
  LOG_S("size = " << stack.size() << ", top = " << stack.top());

  stack.pop();  // removes 3
  LOG_S("after pop(): top = " << stack.top());

  std::string drained;
  while (!stack.empty()) {
    drained += std::to_string(stack.top()) + " ";
    stack.pop();
  }
  LOG_S("drained in LIFO order: " << drained);
}

/// Classic stack use case: are the brackets balanced?
bool isBalanced(const std::string& text) {
  std::stack<char, std::vector<char>> open;  // stack on top of std::vector
  for (const char c : text) {
    if (c == '(' || c == '[' || c == '{') {
      open.push(c);
    } else if (c == ')' || c == ']' || c == '}') {
      char expected = '(';
      if (c == ']') {
        expected = '[';
      } else if (c == '}') {
        expected = '{';
      }
      if (open.empty() || open.top() != expected) {
        return false;
      }
      open.pop();
    }
  }
  return open.empty();
}

void balancedBrackets() {
  LOG_SECTION("Use case: balanced brackets");
  for (const std::string text : {"{[()()]}", "([)]", "((())"}) {
    LOG_S(text << " -> " << (isBalanced(text) ? "balanced" : "not balanced"));
  }
}

}  // namespace

LAB_EXAMPLE("Stack", "std::stack: LIFO container adapter") {
  basics();
  balancedBrackets();
}
