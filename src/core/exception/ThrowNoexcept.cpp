// -----------------------------------------------------------------------------
// noexcept
//
//   void f() noexcept;          promise: f never lets an exception escape
//   void g() noexcept(false);   may throw (the default for most functions)
//   noexcept(expression)        compile-time operator: can this expression throw?
//
//   - If an exception tries to leave a noexcept function, std::terminate() is
//     called immediately - there is no unwinding to the caller.
//   - Destructors are noexcept by default.
//   - Mark move constructors noexcept: std::vector only MOVES its elements
//     during reallocation when that cannot throw; otherwise it copies them.
//   - `throw()` is the old spelling, removed in C++20.
//
// Reference: https://en.cppreference.com/w/cpp/language/noexcept_spec
// -----------------------------------------------------------------------------

#include <exception>
#include <stdexcept>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void mayThrow() {
  throw std::runtime_error("mayThrow failed");
}

/// Handles everything inside, so it can honestly promise noexcept.
void neverThrows() noexcept {
  try {
    mayThrow();
  } catch (const std::exception& e) {
    LOG_S("neverThrows handled the error internally: " << e.what());
  }
}

void noexceptOperator() {
  LOG_SECTION("noexcept specifier and operator");
  neverThrows();
  LOG_S(std::boolalpha << "noexcept(neverThrows()) = "
                       << noexcept(neverThrows()));
  LOG_S(std::boolalpha << "noexcept(mayThrow())    = " << noexcept(mayThrow()));
  LOG_S(std::boolalpha << "noexcept(1 + 1)         = " << noexcept(1 + 1));
}

void terminateHandler() {
  LOG_SECTION("What happens when noexcept is broken");
  // If mayThrow() were called without try/catch inside a noexcept function,
  // the program would call std::terminate(). A terminate handler can log
  // before the process ends, but it must not return.
  const std::terminate_handler previous = std::set_terminate([] {
    // would run on terminate, e.g. log and std::abort()
  });
  LOG("installed a terminate handler (not triggered here, it would end the "
      "lab)");
  std::set_terminate(previous);  // restore: handlers are process-wide state
}

/// Counts how a std::vector treats its elements when it reallocates.
struct Counters {
  int copies = 0;
  int moves = 0;
};

template <bool IsNoexceptMove>
class Element {
 public:
  explicit Element(Counters& counters) : counters_{&counters} {}
  Element(const Element& other) : counters_{other.counters_} {
    ++counters_->copies;
  }
  Element(Element&& other) noexcept(IsNoexceptMove)
      : counters_{other.counters_} {
    ++counters_->moves;
  }
  Element& operator=(const Element&) = default;
  Element& operator=(Element&&) = default;
  ~Element() = default;

 private:
  Counters* counters_;
};

template <bool IsNoexceptMove>
Counters growVector() {
  Counters counters;
  std::vector<Element<IsNoexceptMove>> elements;
  for (int i = 0; i < 100; ++i) {
    // NOLINTNEXTLINE(performance-inefficient-vector-operation): no reserve on purpose
    elements.emplace_back(counters);  // reallocates several times
  }
  return counters;
}

void moveIfNoexcept() {
  LOG_SECTION("Why move constructors should be noexcept");
  const Counters with_noexcept = growVector<true>();
  const Counters without_noexcept = growVector<false>();
  LOG_S("move ctor noexcept    : " << with_noexcept.moves << " moves, "
                                   << with_noexcept.copies << " copies");
  LOG_S("move ctor may throw   : " << without_noexcept.moves << " moves, "
                                   << without_noexcept.copies
                                   << " copies  <- slower!");
  LOG("std::vector copies to keep its strong exception guarantee when a move "
      "could throw.");
}

}  // namespace

LAB_EXAMPLE("ThrowNoexcept",
            "noexcept specifier/operator, std::terminate, noexcept moves in "
            "std::vector") {
  noexceptOperator();
  terminateHandler();
  moveIfNoexcept();
}
