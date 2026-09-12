// -----------------------------------------------------------------------------
// Encapsulation
//
// Bundle data with the functions that use it, and hide the data behind a
// small public interface.
//
// Benefits:
//   1. Data hiding    : outside code cannot put the object into an invalid state.
//   2. Invariants     : the class checks every change (e.g. balance >= 0).
//   3. Modularity     : data and related functions live in one place.
//   4. Flexibility    : the implementation can change without touching callers,
//                       as long as the public interface stays the same.
//
// In C++: `private` / `protected` restrict access; `class` members are
// private by default, `struct` members are public by default.
//
// Reference: https://www.learncpp.com/cpp-tutorial/the-benefits-of-data-hiding-encapsulation/
// -----------------------------------------------------------------------------

#include <stdexcept>
#include <string>
#include <utility>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

/// Without encapsulation anyone can break the rules.
struct OpenAccount {
  std::string owner;
  long balance_cents{0};  // nothing stops `balance_cents = -1'000'000;`
};

/// Invariant: the balance is never negative.
class BankAccount {
 public:
  explicit BankAccount(std::string owner) : owner_{std::move(owner)} {}

  void deposit(long cents) {
    if (cents <= 0) {
      throw std::invalid_argument("deposit must be positive");
    }
    balance_cents_ += cents;
  }

  /// Returns false instead of letting the balance go negative.
  bool withdraw(long cents) {
    if (cents <= 0 || cents > balance_cents_) {
      return false;
    }
    balance_cents_ -= cents;
    return true;
  }

  // Getters give read-only access. Returning `const std::string&` avoids a copy.
  const std::string& owner() const { return owner_; }
  long balance() const { return balance_cents_; }

 private:
  std::string owner_;
  long balance_cents_{0};
};

void run() {
  LOG_SECTION("Without encapsulation");
  OpenAccount open{"Bob", 100};
  open.balance_cents = -1'000'000;  // compiles - the object is now invalid
  LOG_S(open.owner << "'s balance: " << open.balance_cents << " cents");

  LOG_SECTION("With encapsulation");
  BankAccount account{"Alice"};
  account.deposit(2'500);
  LOG_S(account.owner() << " deposited 2500, balance: " << account.balance());

  const bool ok = account.withdraw(1'000);
  LOG_S("withdraw(1000) -> " << std::boolalpha << ok
                             << ", balance: " << account.balance());

  const bool too_much = account.withdraw(99'999);
  LOG_S("withdraw(99999) -> " << std::boolalpha << too_much
                              << ", balance unchanged: " << account.balance());

  try {
    account.deposit(-5);
  } catch (const std::invalid_argument& e) {
    LOG_S("deposit(-5) rejected: " << e.what());
  }
  // account.balance_cents_ = -1;  // error: 'balance_cents_' is private
}

}  // namespace

LAB_EXAMPLE("Encapsulation",
            "private data, class invariants, getters and setters") {
  run();
}
