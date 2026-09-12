# Streams and files

| Example | Topic |
|---|---|
| `IOStream` | `cout`/`cerr`/`clog`, redirecting `cin`, stream states, input validation |
| `StringStream` | building and parsing strings with `<sstream>` |
| `OutputFormatting` | `<iomanip>` flags and manipulators, sticky state, `std::format` |
| `FileIO` | writing, appending and reading text files |
| `BinaryFileHandling` | binary records, serializing strings, random access |
| `Directory` | `std::filesystem`: paths, creating, listing, removing |
| `TerminalColor` | ANSI escape codes for colors and text effects |
| `Print` | formatted output without iostreams, and how it compares with printf and cout *(draft)* |

---
## 1. I/O Streams
- It is a part of the STL.
- I/O is implemented with `streams`.
<br>

- **stream** is a sequence of bytes that can be accessed sequentially. It may produce or consume amounts data over time.
	- **input stream**: used to hold input data from a data producer.
	- **output stream**: used to hold output data for a particular data consumer.
	- *e.g.* when writing/input data to an device, the device may not be ready to accept that data, so the data will sit in the stream. 
	- [C++] <=> streams <=> [os]
<br>

- **I/O in C++**: we can use the STL classes to deal with streams
	- **input stream**: `istream` & extraction operation (`>>`) is used to remove values from the stream
	- **output stream**: `ostream` & insertio operation (`<<`) is used to put vlaues in the stream.\
	- `iostream` can handle both i & o
<br>

- **Standard streams**:
	- `cin`: an `istream` object tied to the standard input
	- `cout`: an `ostream` object tied to the standard output
	- `cerr`: an `ostream` object tied to the standard error - unbuffered output
	- `clog`: an `ostream` object tied to the standard error - buffered output

## 2. Input with istream
- Use `extraction operator (>>)` to read information from an input stream. It skips **whitespace (blanks, tabs, and newlines)**. Use `get(), getLine()` to not discard the whitespace.
- `manipulator` is an object that is used to modify a stream when applied with the `extraction (>>)` or `insertion (<<)` operators (`<iomanip>`).

## 1.3. Output with ostream
- There are two ways to change the formatting options:
  - `flags`: as boolean variable can be turned on and off e.g. `std::ios::showpos`, which live in the `std::ios` class
  - `manipulators`: are objects placed in a stream that affect the way things are input and output. e.g. `std::boolalpha`, which are live in the `std namespace` of the `<iomanip>` class
  - We also can use the `member functions` in `std::ostream` class
- To switch a flag on, use `self()` function.
- e.g. Refer to the `OutputFormatting.cpp` and [28.3 — Output with ostream and ios
](https://www.learncpp.com/cpp-tutorial/output-with-ostream-and-ios/)

## 1.4. Stream classes for string <sstream>
- String streams provide a buffer to hold data but are not connected to an I/O channel.
- The primary uses of the string stream:
  - Display data later then or proccess i/o data.
  - Get data into a string stream
  - Get data from a string stream
  - Convertion strings/numbers
  - Clear a string stream

- e.g:

```cpp
#include <sstream>
#include <string>
int main() {
  std::stringstream os{};
  // input
  os << "0xF";
  std::cout << os.str();
  
  os.str("0x1 0x2");
  std::cout << os.str();

  // output
  std::string bytesStr  = os.str();
  std::cout << bytesStr;

  os.str("0x0 0xF 0xE 0x2");
  os >> bytesStr;
  std::cout <<bytesStr;

  // conversions
  int byte_low = 0xFFF;
  int byte_high = 0x001;
  os.clear();
  os << byte_low << ' ' << byte_high;
  std::cout << os.str();
}
```

## 1.5. Validation
https://www.learncpp.com/cpp-tutorial/stream-states-and-input-validation/

## 1.6. File I/O <fstream>
- There are three basics file I/O classes:
  - `ifstream`: derived from `istream`
  - `ofstream`: derived from `ostream`
  - `fstream`: derived from `stream`
<br>

- **important:** We have to explicitly setup to use file streams:
  - 1. **Open a file: instantiate an object of file I/O class, with the name of the file as a param**
  - 2. **Use the insertion (<<) or extraction (>>) to r/w**
  - 3. **Close a file: call the close() or let file I/O go out of scope.**
  - 4. **To delete a file, simply use the remove() function.**
<br>

- **File output:**

```cpp
#include <fstream>
#include <iostream>
#include <string>

static void fileOutput() {
  std::ofstream outfile{"grs_bytes.csv"};
  if (!outfile || !outfile.is_open()) { // !error || !open
    std::cerr << "[E] Cannot create the output file";
    return;
  }

  // Put bytes data to the file
  // put string
  std::string elfBytes{
      R"""(
        time_s,
        gsr_value 0.0, 45.27761157741693 0.005, 41.69912812397066 0.01,
        38.13110177547114 0.015, 35.32162785580394 0.02,
        31.75617843363382 0.025, 28.352875321528607 0.03,
        25.23210282654006 0.035, 21.769688905641132 0.04,
        17.99031153059391 0.045, 15.073732055666543 0.05,
        15.13550182371759 0.055, 14.69547985289048 0.06,
        14.867397107985468 0.065, 14.982082556093832 0.07,
        14.893751010484861 0.075, 14.877034044202343 0.08,
        14.820590581790071 0.085, 15.1065350504897 0.09,
        15.152287796727098 0.095, 14.764395300078201 0.1,
        15.118189654760348 0.105, 15.473255351586635 0.11,
        14.913896402347113
    \n )"""};
  outfile << elfBytes;

  elfBytes =
      "0x0 0x0"
      "0x0 0x0"
      "0x0 0x0";
  outfile << elfBytes;

  elfBytes =
      "0xF 0xA \
              0xE 0xB \
              0x0 0x0";

  outfile << elfBytes;

  outfile.put('E');  // put char
  outfile.close();
}
```
<br>

- **File input:**

```cpp
static void fileInput() {
  std::ifstream inFile{"grs_bytes.csv"};
  if (!inFile.is_open()) {
    std::cerr << "Error to open file";
  }

  std::string inputStr{};
  std::cout << "========" << std::endl;
  while (
      inFile >>
      inputStr) {  //  Note that ifstream returns a 0 if we’ve reached the end of the file (EOF)
    std::cout << inputStr;
  }
  std::cout << "========" << std::endl;

  // not skip whitespace
  inFile.close();
  inFile.open("grs_bytes.csv"); // explicitly call open()
  inputStr.clear();
  while (std::getline(inFile, inputStr)) {
    std::cout << inputStr << std::endl;
  }
  std::cout << "========" << std::endl;

  inFile.close();
}
```
<br>

- **File Mode:**
  - `app`: opens the file in append mode
  - `ate`: seeks to the end of the file before reading/writing
  - `binary`: opens the file in binary mode (instead of text mode)
  - `in`: opens the file in read mode (default for ifstream)
  - `out`: opens the file in write mode (default for ofstream)
  - `trunc`: erases the file if it already exists
<br>

## 1.7 Ramdom File I/O
- **File pointer:** Each file stream class contains a file pointer that is used to keep track of the current read/write position within the file
- **Ramdom access** with `seekg() & seekp()`
- **IOS seek flags:**
	- `beg`: the offset is relative to the beginning of the file (default)
	- `cur`: the offset is relative to the current location of the file pointer
	- `end`: the offset is relative to the end of the file

> In programming, a newline (‘\n’) is actually an abstraction.
On Windows, a newline is represented as sequential CR (carriage return) and LF (line feed) characters (thus taking 2 bytes of storage).
On Unix, a newline is represented as a LF (line feed) character (thus taking 1 byte of storage).

> // assume iofile is an object of type fstream
iofile.seekg(iofile.tellg(), std::ios::beg); // seek to current file position
<br>

## 1.8 Binary File
- e.g.

```cpp
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/*
 * Class Account
 * Represent a bank account with:
 *  - code    : account ID
 *  - name    : account holder name
 *  - balance : current balance
 */
class Account
{
    // Allow operator<< to access private members
    friend ostream& operator<<(ostream&, const Account&);

private:
    int code;        // Account ID
    string name;     // Account holder name
    double balance;  // Account balance

public:
    // Constructor with default values
    Account(int c = 0, const string& n = "", double b = 0)
        : code(c), name(n), balance(b) {}

    // Getter for account code
    int getCode() const { return code; }

    // Write object to binary file
    ostream& write(ostream&) const;

    // Read object from binary file
    istream& read(istream&);
};

/*
 * Write entire object to binary stream
 * WARNING:
 * This method is unsafe because std::string contains dynamic memory.
 * Writing raw memory of the object is not portable and not recommended
 * for real-world applications.
 */
ostream& Account::write(ostream& os) const
{
    os.write(reinterpret_cast<const char*>(this), sizeof(Account));
    return os;
}

/*
 * Read entire object from binary stream
 */
istream& Account::read(istream& is)
{
    is.read(reinterpret_cast<char*>(this), sizeof(Account));
    return is;
}

/*
 * Overload << operator to print Account info
 */
ostream& operator<<(ostream& os, const Account& acc)
{
    os << "Code   : " << acc.code << endl;
    os << "Name   : " << acc.name << endl;
    os << "Balance: " << acc.balance << endl;
    return os;
}

int main()
{

    // Create two accounts dynamically
    Account* accounts[] = {
        new Account(1, "Whitney Elizabeth Houston", 2500),
        new Account(2, "Michael Jackson", 5000)
    };

     // Open binary file for append
    fstream outf("data.bin", ios::out | ios::app | ios::binary);

    // Write both accounts to file
    for (int i = 0; i < 2; ++i)
    {
        if (!accounts[i]->write(outf))
            cerr << "Error in writing!" << endl;
    }

    outf.close();

    cout << "Account #2" << endl;


    // Read account with code = 2
    Account temp(2);

    fstream inf("data.bin", ios::in | ios::binary);

    // Move read pointer to correct position
    // (code - 1) * sizeof(Account)
    inf.seekg((temp.getCode() - 1) * sizeof(Account));

    if (!temp.read(inf))
        cerr << "Error in reading!" << endl;
    else
        cout << temp << endl;

    inf.close();

    // Free allocated memory
    delete accounts[0];
    delete accounts[1];

    return 0;
}
```