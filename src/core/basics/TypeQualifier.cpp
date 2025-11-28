// cppcheck-suppress-file [unreadVariable]
#include <iostream>
using namespace std;

namespace {
namespace Constant {
void run() {
  // 1. Name constants
  const double const_var_g{9.8};
#define NAME "Phong"

  // 2. Literals constants
  bool myNameIsAlex{true};  // true is a boolean literal -> type: bool
  double d{3.4};            // 3.4 is a double literal -> type: double
  std::cout
      << "Hello, world!";  // "Hello, world!" is a C-style string literal -> type: const char[14]

  // 3. Constexpr variables
  constexpr double constexpr_var_g{9.8};  // compile-time constant

  // 4. Constant expression
  constexpr double something{
      constexpr_var_g};  // ok: sum is a constant expression
  // constexpr double something2{const_var_g}; // error because const_var_g is not the constexprs
}
}  // namespace Constant

}  // namespace

struct TypeQualifier {
  TypeQualifier() {
    cout << "\n"
         << "\n"
         << "TypeQualifier\n";
    Constant::run();
  }
};

static TypeQualifier autoRunInstance;
