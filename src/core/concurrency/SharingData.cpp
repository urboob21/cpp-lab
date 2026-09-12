// -----------------------------------------------------------------------------
// Passing data to threads
//
//   - std::thread COPIES (or moves) its arguments into the new thread, even if
//     the function takes a reference.
//   - Use std::ref(x) to really pass a reference. The caller must then make
//     sure `x` outlives the thread.
//   - Lambdas choose explicitly: [x] copies, [&x] references.
//   - thread_local gives every thread its own instance of a variable.
//
// Reference: https://en.cppreference.com/w/cpp/thread/thread/thread
// -----------------------------------------------------------------------------

#include <functional>
#include <string>
#include <thread>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void incrementByValue(int value) {
  ++value;  // changes the thread's own copy
  LOG_S("  inside thread: value = " << value);
}

void incrementByReference(int& value) {
  ++value;  // changes the caller's variable
  LOG_S("  inside thread: value = " << value);
}

void arguments() {
  LOG_SECTION("Arguments are copied unless wrapped in std::ref");
  int number = 10;

  std::thread by_value(incrementByValue, number);
  by_value.join();
  LOG_S("after incrementByValue:     number = " << number << "  (unchanged)");

  // std::thread by_ref(incrementByReference, number);  // does not compile:
  //   a copied int (an rvalue) cannot bind to `int&`
  std::thread by_ref(incrementByReference, std::ref(number));
  by_ref.join();
  LOG_S("after incrementByReference: number = " << number << "  (changed)");
}

void lambdas() {
  LOG_SECTION("Lambda captures");
  std::string message = "hello";

  std::thread copies([message]() mutable {  // own copy of message
    message += " (modified copy)";
    LOG_S("  copy in thread: " << message);
  });
  copies.join();
  LOG_S("after capture by value:     message = " << message);

  std::thread references([&message] { message += " world"; });
  references.join();
  LOG_S("after capture by reference: message = " << message);
}

thread_local int t_calls = 0;  // one independent counter per thread

void countCalls(const char* name) {
  for (int i = 0; i < 3; ++i) {
    ++t_calls;
  }
  LOG_S("  " << name << " sees t_calls = " << t_calls);
}

void threadLocalStorage() {
  LOG_SECTION("thread_local");
  std::thread a(countCalls, "thread A");
  std::thread b(countCalls, "thread B");
  a.join();
  b.join();
  LOG_S("main thread sees t_calls = " << t_calls << "  (never touched here)");
}

}  // namespace

LAB_EXAMPLE(
    "SharingData",
    "passing arguments to threads: copies, std::ref, captures, thread_local") {
  arguments();
  lambdas();
  threadLocalStorage();
}
