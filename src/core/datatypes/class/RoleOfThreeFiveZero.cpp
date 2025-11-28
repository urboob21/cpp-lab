// cppcheck-suppress-file [noCopyConstructor, noOperatorEq, accessMoved]

#include <cstring>
#include <iostream>
#include <utility>

namespace {
namespace Problem {
class Model {
 private:
  char* cstring;

 public:
  explicit Model(const char* s = "") : cstring{nullptr} {
    if (s) {
      cstring = new char[std::strlen(s) + 1];  // allocate
      std::strcpy(cstring, s);                 // populate
    }
  }

  // I. destructor
  ~Model() {
    // delete[] cstring; // [P2] Double call here
    std::cout << "Deleted cstring at: " << static_cast<const void*>(cstring)
              << "\n";
  }

  // [P]
  // What happen if we didn't define
  // II. copy constructor
  // III. copy assignment

  // Helper functions
  const char* c_str() const  // accessor
  {
    return cstring;
  }

  void set_first_char(char ch) {
    if (cstring)
      cstring[0] = ch;
  }
};

void run() {
  {
    std::cout << "\n\nProblem\n";
    Model str1{"str1"};
    Model str2 = str1;  // shallow copy  with "assigment = "
    std::cout << "Before change:\n";
    std::cout << "  str1 = " << str1.c_str() << "\n";
    std::cout << "  str2 = " << str2.c_str() << "\n";

    std::cout << "\n[P1] Shallow copy (shared pointer).\n";
    str2.set_first_char('X');
    std::cout << "\nAfter modifying str2.set_first_char('X'):\n";
    std::cout << "  str1 = " << str1.c_str() << "   <-- also changed!\n";
    std::cout << "  str2 = " << str2.c_str() << "\n";
    std::cout
        << "\n[P2] Decontructor issue when releasing the shared pointer.\n";
  }
}
}  // namespace Problem

// Rule of Three: define
//  - destructor
//  - copy constructor
//  - copy assignment
namespace RoleOfThree {
class Model {
 private:
  char* cstring;

 public:
  explicit Model(const char* s = "") : cstring{nullptr} {
    if (s) {
      cstring = new char[std::strlen(s) + 1];  // allocate
      std::strcpy(cstring, s);                 // populate
    }
  }

  // I. destructor
  ~Model() {
    delete[] cstring;  // [P2] Double call here
    std::cout << "Deleted cstring at: " << static_cast<const void*>(cstring)
              << "\n";
  }

  // II. copy constructor
  Model(const Model& other) : Model(other.cstring) {}

  // III. copy assignment
  Model& operator=(const Model& other) {
    Model temp(other);
    std::swap(cstring, temp.cstring);
    return *this;
  }

  // Helper functions
  const char* c_str() const  // accessor
  {
    return cstring;
  }

  void set_first_char(char ch) {
    if (cstring)
      cstring[0] = ch;
  }
};

void run() {
  {
    std::cout << "\n\nRole of three\n";
    Model str1{"str1"};
    Model str2 = str1;  // shallow copy  with "assigment = "
    std::cout << "Before change:\n";
    std::cout << "  str1 = " << str1.c_str() << "\n";
    std::cout << "  str2 = " << str2.c_str() << "\n";

    str2.set_first_char('X');
    std::cout << "\nAfter modifying str2.set_first_char('X'):\n";
    std::cout << "  str1 = " << str1.c_str() << "   <-- not changed!\n";
    std::cout << "  str2 = " << str2.c_str() << "\n";

    // [P] Role of three not cover this std::move
    Model str{"there"};
    Model str_move = std::move(str);
    std::cout << "\nAfter moving str into str_move:\n";
    std::cout << "  str.c_str()      = " << (str.c_str() ? str.c_str() : "null")
              << "\n";
    std::cout << "  str_move.c_str() = "
              << (str_move.c_str() ? str_move.c_str() : "null") << "\n";
  }
}
}  // namespace RoleOfThree

// Rule of Five: Rule of Three and define
//  - move constructor
//  - move assignment
namespace RoleOfFive {
class Model {
 private:
  char* cstring;

 public:
  explicit Model(const char* s = "") : cstring{nullptr} {
    if (s) {
      cstring = new char[std::strlen(s) + 1];  // allocate
      std::strcpy(cstring, s);                 // populate
    }
  }

  // I. destructor
  ~Model() {
    delete[] cstring;  // [P2] Double call here
    std::cout << "Deleted cstring at: " << static_cast<const void*>(cstring)
              << "\n";
  }

  // II. copy constructor
  Model(const Model& other) : Model(other.cstring) {}

  // III. copy assignment
  Model& operator=(const Model& other) {
    Model temp(other);
    std::swap(cstring, temp.cstring);
    return *this;
  }

  // IV. move constructor
  // noexcept is a specifier that indicates a function will not throw
  // exceptions.
  Model(Model&& other) noexcept
      : cstring(std::exchange(other.cstring, nullptr)){};

  // V. move assignment
  Model& operator=(Model&& other) noexcept {
    std::swap(cstring, other.cstring);
    return *this;
  }

  // Helper functions
  const char* c_str() const  // accessor
  {
    return cstring;
  }

  void set_first_char(char ch) {
    if (cstring)
      cstring[0] = ch;
  }
};

void run() {
  {
    std::cout << "\n\nRole of Five\n";
    Model str1{"str1"};
    Model str2 = str1;  // shallow copy  with "assigment = "
    std::cout << "Before change:\n";
    std::cout << "  str1 = " << str1.c_str() << "\n";
    std::cout << "  str2 = " << str2.c_str() << "\n";

    str2.set_first_char('X');
    std::cout << "\nAfter modifying str2.set_first_char('X'):\n";
    std::cout << "  str1 = " << str1.c_str() << "   <-- not changed!\n";
    std::cout << "  str2 = " << str2.c_str() << "\n";

    // Role of five covers this std::move
    Model str{"there"};
    Model str_move = std::move(str);
    std::cout << "\nAfter moving str into str_move:\n";
    std::cout << "  str.c_str()      = " << (str.c_str() ? str.c_str() : "null")
              << "\n";
    std::cout << "  str_move.c_str() = "
              << (str_move.c_str() ? str_move.c_str() : "null") << "\n";
  }
}
}  // namespace RoleOfFive

// Rule of Zero: use RAII (std::string, std::vector, smart pointers,...), no
// need to define any special functions except constructor
namespace RoleOfZero {
class base_of_five_defaults {
 public:
  base_of_five_defaults(const base_of_five_defaults&) = default;
  base_of_five_defaults(base_of_five_defaults&&) = default;
  base_of_five_defaults& operator=(const base_of_five_defaults&) = default;
  base_of_five_defaults& operator=(base_of_five_defaults&&) = default;
  virtual ~base_of_five_defaults() = default;
};

class Model {
 private:
  std::string cstring;
  // int* ptr;	-> std::unique_ptr<int> ptr; (exclusive ownership)
  // int* arr;	-> std::vector<int> arr; (automatic array management)
  // int* shared;	-> std::shared_ptr<int> shared; (shared ownership)

 public:
  explicit Model(const std::string& str) : cstring{str} {}

  // Helper functions
  const std::string c_str() const  // accessor
  {
    return cstring;
  }

  void set_first_char(char ch) { cstring.at(0) = ch; }
};

void run() {
  {
    std::cout << "\n\nRole of Zero\n";
    Model str1{"str1"};
    Model str2 = str1;  // shallow copy  with "assigment = "
    std::cout << "Before change:\n";
    std::cout << "  str1 = " << str1.c_str() << "\n";
    std::cout << "  str2 = " << str2.c_str() << "\n";

    str2.set_first_char('X');
    std::cout << "\nAfter modifying str2.set_first_char('X'):\n";
    std::cout << "  str1 = " << str1.c_str() << "   <-- not changed!\n";
    std::cout << "  str2 = " << str2.c_str() << "\n";

    Model str{"there"};
    Model str_move = std::move(str);
    std::cout << "\nAfter moving str into str_move:\n";
    std::cout << "  str.c_str()      = " << str.c_str() << "\n";
    std::cout << "  str_move.c_str() = " << str_move.c_str() << "\n";
  }
}
}  // namespace RoleOfZero
}  // namespace
struct RoleOfThreeFiveZeroAutoRuner {
  // Virtual default destructor: Does not break Rule of Three, Five, or Zero
  RoleOfThreeFiveZeroAutoRuner() {
    Problem::run();
    RoleOfThree::run();
    RoleOfFive::run();
    RoleOfZero::run();
  }
};

static RoleOfThreeFiveZeroAutoRuner instance;