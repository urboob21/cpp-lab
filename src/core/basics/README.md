# Basics

| Example          | Topic                                                                |
|------------------|----------------------------------------------------------------------|
| `ControlFlow`    | if/else, if with initializer, switch and `[[fallthrough]]`, loops, break/continue/goto |
| `Initialization` | default, value, direct, copy, list and aggregate initialization      |
| `Operations`     | arithmetic, logical (short-circuit) and bitwise operators            |
| `TypeQualifier`  | `const`, `constexpr`, `consteval`, `constinit`, `mutable`, `volatile` |
| `Attributes` | [[nodiscard]], [[maybe_unused]], [[fallthrough]], [[deprecated]], [[likely]], [[assume]] *(draft)* |

## 1. Initialization

```cpp
int a;             // default initialization: indeterminate value, reading it is UB
int b{};           // value initialization: 0
int c(42);         // direct initialization
int d = 42;        // copy initialization
int e{42};         // list initialization: rejects narrowing (int e{4.2} does not compile)
Point p{.x = 1};   // designated initializer (C++20, aggregates only)
```

- Prefer braces `{}`: they always initialize and reject narrowing.
- `T obj();` does not create an object. It declares a function (the "most vexing parse").
- `std::vector<int>{3, 7}` holds the two elements 3 and 7, while `std::vector<int>(3, 7)` holds
  three 7s.

## 2. Operators

- Integer division truncates toward zero. `%` keeps the sign of the left operand.
- Unsigned arithmetic wraps around. **Signed overflow is undefined behavior.**
- `&&` and `||` short-circuit: the right side runs only if needed.
- Prefix `++x` returns the new value; postfix `x++` returns a copy of the old one.

## 3. const vs constexpr vs consteval vs constinit

| Keyword     | Guarantees                                                        |
|-------------|-------------------------------------------------------------------|
| `const`     | the value cannot change after initialization (may be known only at run time) |
| `constexpr` | the value is computed at compile time; constexpr functions *may* run there |
| `consteval` | the function *must* run at compile time (C++20)                    |
| `constinit` | a static variable is initialized at compile time but stays mutable (C++20) |

Use `constexpr` instead of `#define` for constants: it has a type, a scope, and shows up in the
debugger.
