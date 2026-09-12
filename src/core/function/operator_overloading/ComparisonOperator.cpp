// -----------------------------------------------------------------------------
// Comparison operators and the C++20 spaceship operator <=>
//
//   auto operator<=>(const T&) const = default;   member-wise three-way compare
//   bool operator==(const T&) const = default;    member-wise equality
//
//   - From <=> the compiler rewrites  <  <=  >  >=  ; from == it rewrites  !=.
//     Before C++20 all six operators had to be written by hand.
//   - <=> returns an ordering category:
//       std::strong_ordering  equal values are indistinguishable (int)
//       std::weak_ordering    equivalent values may differ (case-insensitive text)
//       std::partial_ordering some values are unordered (NaN for double)
//   - A defaulted <=> also implies a defaulted ==.
//
// Reference: https://en.cppreference.com/w/cpp/language/default_comparisons
// -----------------------------------------------------------------------------

#include <algorithm>
#include <cctype>
#include <compare>
#include <string>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Cents {
 public:
  explicit Cents(int cents) : cents_{cents} {}
  int value() const { return cents_; }

  auto operator<=>(const Cents&) const =
      default;  // also gives ==, !=, <, <=, >, >=

 private:
  int cents_;
};

struct Version {
  int major;
  int minor;
  int patch;
  // Compares major first, then minor, then patch - in declaration order.
  auto operator<=>(const Version&) const = default;
};

/// Case-insensitive name: "Alice" and "alice" are equivalent but not identical.
class Name {
 public:
  explicit Name(std::string text) : text_{std::move(text)} {}
  const std::string& text() const { return text_; }

  std::weak_ordering operator<=>(const Name& other) const {
    const auto lower = [](unsigned char c) {
      return std::tolower(c);
    };
    return std::lexicographical_compare_three_way(
        text_.begin(), text_.end(), other.text_.begin(), other.text_.end(),
        [&lower](unsigned char a, unsigned char b) {
          return lower(a) <=> lower(b);
        });
  }
  bool operator==(const Name& other) const { return (*this <=> other) == 0; }

 private:
  std::string text_;
};

const char* describe(std::strong_ordering order) {
  if (order < 0) {
    return "less";
  }
  return order > 0 ? "greater" : "equal";
}

void run() {
  LOG_SECTION("Defaulted <=> on a single member");
  const Cents a{25};
  const Cents b{30};
  LOG_S(std::boolalpha << "a == b " << (a == b) << ", a != b " << (a != b)
                       << ", a < b " << (a < b) << ", a <= b " << (a <= b)
                       << ", a > b " << (a > b) << ", a >= b " << (a >= b));
  LOG_S("a <=> b is " << describe(a <=> b));

  LOG_SECTION("Member-wise ordering");
  std::vector<Version> versions{{1, 10, 0}, {1, 2, 3}, {0, 9, 9}, {1, 2, 10}};
  std::sort(versions.begin(),
            versions.end());  // uses the synthesized operator<
  std::string sorted;
  for (const Version& v : versions) {
    sorted += std::to_string(v.major) + "." + std::to_string(v.minor) + "." +
              std::to_string(v.patch) + "  ";
  }
  LOG_S("sorted versions: " << sorted);

  LOG_SECTION("Custom weak ordering");
  const Name alice{"Alice"};
  const Name lower_alice{"alice"};
  const Name bob{"bob"};
  LOG_S(std::boolalpha << "\"Alice\" == \"alice\": " << (alice == lower_alice)
                       << ", \"alice\" < \"bob\": " << (lower_alice < bob)
                       << ", identical text: "
                       << (alice.text() == lower_alice.text()));
}

}  // namespace

LAB_EXAMPLE(
    "ComparisonOperator",
    "C++20 operator<=> and ==, rewritten comparisons, ordering categories") {
  run();
}
