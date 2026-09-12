# Utilities

| Example | Topic |
|---|---|
| `Optional` | `std::optional` instead of magic values, null pointers or exceptions |
| `Algorithm` | `<algorithm>`, `<numeric>` and C++20 ranges/views |
| `Regex` | `<regex>`: match, search, capture groups, replace |
| `TypeTraits` | `<type_traits>`, the detection idiom, `enable_if`, C++20 concepts |
| `Variadic` | C-style `va_list` vs variadic templates and fold expressions |

---
## Utilities library

### 1. `<optional>`
There are several ways to represent **"no value"**:
    - `nullptr`
    - sentinel values (`-1`, empty string, etc.)
    - exceptions
    - output parameters or flags

**Drawbacks:**
  - unclear semantics
  - unsafe assumptions
  - poor readability


**`optional`** was introduced to represent data that may be absent when that absence is part of the domain logic.
```cpp
std::optional<std::string> name_opt = std::nullopt;
```

---
### 2. Variadic Functions (**`<cstdarg>` / `<stdarg.h>`**)
Variadic functions are functions that can take a variable number of arguments by declaring an ellipsis (`...`) after the parameter list.

e.g.
int function(int a, ...);

- To access the variadic arguments inside the function body, the following APIs are used:

`va_start` : initializes access to the variadic arguments
`va_arg`   : retrieves the next variadic argument
`va_copy`  : copies a variadic argument list
`va_end`   : ends traversal of the variadic arguments
`va_list`  : stores the information needed by `va_start`, `va_arg`, `va_end`, and `va_copy`

---