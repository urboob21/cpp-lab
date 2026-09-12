# Strings

| Example            | Topic                                                         |
|--------------------|---------------------------------------------------------------|
| `CString`          | `char` arrays, `strlen` vs `sizeof`, copy/concat/compare, `strtok`, `strtod` |
| `StdString`        | `std::string` operations, searching, conversions, `std::string_view` |
| `StringFormatting` | `std::format`, concatenation, `std::ostringstream`, `snprintf`, custom formatters |

## 1. Three kinds of strings

| Type                 | Owns the characters | Knows its length | Typical use                |
|----------------------|---------------------|------------------|----------------------------|
| `const char*`        | no                  | no (scans for `'\0'`) | C APIs, string literals |
| `std::string`        | yes                 | yes              | storing and modifying text |
| `std::string_view`   | no                  | yes              | read-only parameters (C++17) |

```cpp
void print(std::string_view text);  // accepts literals, std::string, substrings - without copies
```

A `std::string_view` must not outlive the characters it refers to:
`std::string_view view = std::string("temporary");` dangles immediately.

## 2. C string pitfalls

- `sizeof(pointer)` is the size of the pointer, not the length of the text.
- `strcpy`, `strcat` and `sprintf` do not check buffer sizes: prefer `snprintf`.
- `strncpy` does **not** add `'\0'` when the source is too long.
- Compare with `strcmp`, not `==`, which compares addresses.
- `atoi` returns 0 on error. `strtol` and `strtod` report where parsing stopped.

## 3. Formatting (C++20)

```cpp
std::format("{:>8}|{:<8}|{:^8}", "r", "l", "c");  // alignment
std::format("{:.2f} {:#x} {:08.3f}", 3.14159, 255, 2.5);
std::format("{1} {0}", "world", "hello");        // positional arguments
```

`std::format` is type-safe, and a format string that does not match the arguments fails at compile
time.
