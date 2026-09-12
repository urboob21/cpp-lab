// -----------------------------------------------------------------------------
// Type traits (<type_traits>), SFINAE and C++20 concepts
//
// Type traits answer questions about types at compile time:
//   std::is_integral_v<T>, std::is_same_v<A, B>, std::is_pointer_v<T>, ...
// and transform types:
//   std::remove_const_t<T>, std::decay_t<T>, std::conditional_t<cond, A, B>
//
// Constraining templates:
//   - C++11/17: SFINAE with std::enable_if_t and the "detection idiom"
//     (std::void_t) - powerful but hard to read.
//   - C++20: concepts and requires clauses say the same thing clearly and give
//     far better error messages.
//
// Reference: https://en.cppreference.com/w/cpp/header/type_traits
// -----------------------------------------------------------------------------

#include <concepts>
#include <cstdlib>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <vector>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// Human-readable type name (typeid(T).name() is mangled on GCC/Clang).
template <typename T>
std::string typeName() {
#ifdef __GNUG__
  int status = 0;
  const std::unique_ptr<char, void (*)(void*)> demangled(
      abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status),
      std::free);
  return status == 0 && demangled ? demangled.get() : typeid(T).name();
#else
  return typeid(T).name();
#endif
}

void standardTraits() {
  LOG_SECTION("Querying and transforming types");
  LOG_S(std::boolalpha << "is_integral_v<int>          = "
                       << std::is_integral_v<int>);
  LOG_S(std::boolalpha << "is_floating_point_v<float>  = "
                       << std::is_floating_point_v<float>);
  LOG_S(std::boolalpha << "is_pointer_v<int*>          = "
                       << std::is_pointer_v<int*>);
  LOG_S(std::boolalpha << "is_same_v<int, const int>   = "
                       << std::is_same_v<int, const int>);
  LOG_S("remove_const_t<const int>   = "
        << typeName<std::remove_const_t<const int>>());
  LOG_S(
      "decay_t<const int&>         = " << typeName<std::decay_t<const int&>>());
  LOG_S("decay_t<int[3]>             = " << typeName<std::decay_t<int[3]>>());
  LOG_S("conditional_t<true, A, B>   = "
        << typeName<std::conditional_t<true, long, short>>());

  static_assert(std::is_trivially_copyable_v<int>, "checked at compile time");
}

// --- detection idiom (pre-C++20) --------------------------------------------

/// HasAndAssign<T>::value is true if `t &= t` compiles.
template <typename T, typename = void>
struct HasAndAssign : std::false_type {};

template <typename T>
struct HasAndAssign<
    T, std::void_t<decltype(std::declval<T&>() &= std::declval<T>())>>
    : std::true_type {};

struct WithAndAssign {
  WithAndAssign& operator&=(const WithAndAssign&) { return *this; }
};

struct WithoutAndAssign {};

/// SFINAE: this overload only exists for integral types.
template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
std::string describeSfinae(T value) {
  return "integral " + std::to_string(value);
}

template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
std::string describeSfinae(T value) {
  return "floating point " + std::to_string(value);
}

void sfinae() {
  LOG_SECTION("Detection idiom and std::enable_if (C++11/17)");
  LOG_S(std::boolalpha << "HasAndAssign<int>              = "
                       << HasAndAssign<int>::value);
  LOG_S(std::boolalpha << "HasAndAssign<WithAndAssign>    = "
                       << HasAndAssign<WithAndAssign>::value);
  LOG_S(std::boolalpha << "HasAndAssign<WithoutAndAssign> = "
                       << HasAndAssign<WithoutAndAssign>::value);
  LOG_S("describeSfinae(42)  -> " << describeSfinae(42));
  LOG_S("describeSfinae(2.5) -> " << describeSfinae(2.5));
  // describeSfinae("text");  // error: no matching function (hard-to-read message)
}

// --- concepts (C++20) --------------------------------------------------------

template <typename T>
concept AndAssignable = requires(T& a, const T& b) {
  { a &= b } -> std::same_as<T&>;
};

template <std::integral T>
std::string describeConcept(T value) {
  return "integral " + std::to_string(value);
}

template <std::floating_point T>
std::string describeConcept(T value) {
  return "floating point " + std::to_string(value);
}

template <typename Container>
  requires requires(const Container& c) { c.size(); }
std::size_t countElements(const Container& container) {
  return container.size();
}

void concepts() {
  LOG_SECTION("The same with C++20 concepts");
  LOG_S(std::boolalpha << "AndAssignable<int>              = "
                       << AndAssignable<int>);
  LOG_S(std::boolalpha << "AndAssignable<WithAndAssign>    = "
                       << AndAssignable<WithAndAssign>);
  LOG_S(std::boolalpha << "AndAssignable<WithoutAndAssign> = "
                       << AndAssignable<WithoutAndAssign>);
  LOG_S("describeConcept(42)  -> " << describeConcept(42));
  LOG_S("describeConcept(2.5) -> " << describeConcept(2.5));
  LOG_S("countElements(vector{1, 2, 3}) -> "
        << countElements(std::vector<int>{1, 2, 3}));
  // countElements(42);  // error: "the required expression 'c.size()' is invalid"
}

}  // namespace

LAB_EXAMPLE("TypeTraits",
            "type traits, detection idiom, enable_if and C++20 concepts") {
  standardTraits();
  sfinae();
  concepts();
}
