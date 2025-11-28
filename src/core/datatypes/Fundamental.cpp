#include <iostream>
using namespace std;

// 	A basic type built into the core C++ language
void primative() {
  cout << "\n--- Primative Type Examples ---\n";
  // Boolean
  bool isReady = true;
  cout << "bool: " << isReady << "\n";

  // Character
  char c = 'A';
  unsigned char uc = 200;
  wchar_t wc = L'Ω';  // Greek Omega
  // char8_t c8 = u8'A';   // UTF-8
  char16_t c16 = u'ß';   // UTF-16
  char32_t c32 = U'中';  // UTF-32
  cout << "char: " << c << "\n";
  cout << "unsigned char: " << static_cast<int>(uc) << "\n";
  wcout << L"wchar_t: " << wc << L"\n";
  // cout << "char8_t: " << static_cast<char>(c8) << "\n";
  cout << "char16_t: (UTF-16 code) " << static_cast<int>(c16) << "\n";
  cout << "char32_t: (UTF-32 code) " << static_cast<int>(c32) << "\n";

  // Integer
  short s = -10;
  int i = 42;
  unsigned int ui = 100;
  long l = 123456L;
  long long ll = 9876543210LL;
  cout << "short: " << s << "\n";
  cout << "int: " << i << "\n";
  cout << "unsigned int: " << ui << "\n";
  cout << "long: " << l << "\n";
  cout << "long long: " << ll << "\n";

  // Floating Point
  float f = 3.14f;
  double d = 2.718281828;
  long double ld = 1.6180339887L;
  cout << "float: " << f << "\n";
  cout << "double: " << d << "\n";
  cout << "long double: " << ld << "\n";

  // Void
  cout << "void: (no data type, used for functions)\n";

  // pointer
  int const* ptr = nullptr;
  cout << "nullptr_t: " << ptr << "\n";
}

// A struct that runs code when its object is created
struct Fundamental {
  Fundamental() {
    cout << "\n"
         << "\n"
         << "Fundamental\n";

    primative();
  }
};

// All global and static objects are constructed before main() begins.
static Fundamental autoRunInstance;
