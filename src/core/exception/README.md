# Exceptions

| Example         | Topic                                                            |
|-----------------|------------------------------------------------------------------|
| `BasicHandle`   | try/catch/throw, custom exceptions, catch order, rethrow, nested exceptions |
| `ThrowNoexcept` | `noexcept` specifier and operator, `std::terminate`, `noexcept` moves in `std::vector` |

## 1. The mechanism

```cpp
try {
  mightThrow();
} catch (const MyError& e) {      // most specific first
  ...
} catch (const std::exception& e) {
  ...
} catch (...) {                   // anything else
  throw;                          // rethrow the current exception unchanged
}
```

When an exception is thrown, the stack **unwinds**: destructors of all fully constructed local
objects run until a matching `catch` is found. This makes RAII the foundation of exception
safety.

## 2. Guidelines

- Throw by value, catch by `const&`. This avoids copies and object slicing.
- Derive your exceptions from `std::exception`, usually `std::runtime_error` or
  `std::logic_error`.
- Use exceptions for exceptional situations. For an expected "no result", `std::optional` is often
  clearer (see `core/utils/Optional`).
- Never let an exception escape a destructor. Destructors are `noexcept` by default.
- Mark move constructors and move assignment `noexcept`; containers rely on it (see the
  `ThrowNoexcept` example).

## 3. Exception safety guarantees

| Guarantee | Meaning                                                             |
|-----------|---------------------------------------------------------------------|
| no-throw  | the operation never throws (`noexcept`)                              |
| strong    | on failure the state is unchanged ("commit or rollback", e.g. copy-and-swap) |
| basic     | on failure no resources leak and objects stay valid, but may have changed |
| none      | anything can happen - avoid                                          |
