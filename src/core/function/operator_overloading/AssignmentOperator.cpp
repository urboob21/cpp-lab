// -----------------------------------------------------------------------------
// Overloading the assignment operators
//
//   T& operator=(const T& other);      copy assignment
//   T& operator=(T&& other) noexcept;  move assignment
//
//   - Assignment changes an EXISTING object; `T b = a;` is not assignment but
//     copy construction.
//   - Return *this by reference so `a = b = c` works like for built-in types.
//   - Handle self-assignment (a = a). The copy-and-swap idiom does it for free.
//   - Usually the compiler-generated versions are right (Rule of Zero).
//
// Reference: https://en.cppreference.com/w/cpp/language/copy_assignment
// -----------------------------------------------------------------------------

#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

class Document {
 public:
  explicit Document(std::string title = "untitled")
      : title_{std::move(title)} {}

  Document(const Document& other) : title_{other.title_} {
    LOG_S("  copy constructor      <- " << title_);
  }

  Document(Document&& other) noexcept : title_{std::move(other.title_)} {
    LOG_S("  move constructor      <- " << title_);
  }

  Document& operator=(const Document& other) {
    LOG_S("  copy assignment       <- " << other.title_);
    if (this != &other) {  // self-assignment guard
      title_ = other.title_;
    }
    return *this;
  }

  Document& operator=(Document&& other) noexcept {
    LOG_S("  move assignment       <- " << other.title_);
    if (this != &other) {
      title_ = std::move(other.title_);
    }
    return *this;
  }

  ~Document() = default;

  const std::string& title() const { return title_; }

 private:
  std::string title_;
};

void run() {
  LOG_SECTION("Construction vs assignment");
  const Document report{"report"};

  LOG("Document copy = report;   (construction, not assignment)");
  const Document copy = report;

  LOG("target = report;          (assignment to an existing object)");
  Document target;
  target = report;

  LOG("target = Document{\"draft\"};  (a temporary: move assignment)");
  target = Document{"draft"};

  LOG_SECTION("Chaining and self-assignment");
  Document a;
  Document b;
  LOG("a = b = report;");
  a = b = report;  // b = report runs first and returns b
  LOG_S("a.title() = " << a.title() << ", copy.title() = " << copy.title());

  LOG("a = a;   (the operator runs, but the self-assignment guard skips the "
      "copy)");
  const Document& same = a;
  a = same;
  LOG_S("a.title() is still " << a.title());
}

}  // namespace

LAB_EXAMPLE("AssignmentOperator",
            "copy/move assignment, chaining, self-assignment") {
  run();
}
