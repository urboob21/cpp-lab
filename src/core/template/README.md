# Templates

| Example | Topic |
|---|---|
| `FunctionTemplate` | argument deduction, explicit arguments, overloading, `auto` parameters *(draft)* |
| `ClassTemplate` | member functions, default arguments, CTAD and deduction guides *(draft)* |
| `Specialization` | full and partial specialization, and why functions overload instead *(draft)* |
| `NonTypeParameter` | non-type template parameters, `auto` NTTPs, compile-time strings *(draft)* |
| `Concepts` | `requires` clauses and expressions, standard concepts, subsumption *(draft)* |
| `Sfinae` | `enable_if`, `void_t`, and how `if constexpr` and concepts replace them *(draft)* |
| `Crtp` | static polymorphism and mixins without virtual calls *(draft)* |
| `VariableTemplate` | variable templates, `constexpr` and `inline` variables *(draft)* |
| `TwoPhaseLookup` | dependent names, `typename`/`template`, explicit instantiation *(draft)* |

> Every example in this folder is still a **draft**: the file lists what it should teach and prints
> that outline when you run it. See [docs/adding-examples.md](../../../docs/adding-examples.md).

---
## 1. What a template is

A template is not code - it is a recipe the compiler uses to *generate* code. Nothing is compiled
until the template is **instantiated** with concrete arguments:

```cpp
template <typename T>
T max(T a, T b) { return a < b ? b : a; }

max(1, 2);        // instantiates max<int>
max(1.0, 2.0);    // instantiates max<double>
max(1, 2.0);      // error: T cannot be both int and double - deduction conflict
```

Consequences that surprise newcomers:

- Template definitions normally live in **headers**: the compiler needs the body at the point of
  instantiation (the alternative is explicit instantiation, see `TwoPhaseLookup`).
- Errors appear at instantiation, often deep inside the standard library. **Concepts** (C++20) move
  those errors back to the call site - that is their main practical benefit.
- Every distinct set of arguments produces distinct code, which grows the binary. That is the
  trade-off for having no runtime dispatch.

## 2. Compile time vs run time

| Question | Templates | Virtual functions |
|---|---|---|
| When is the type chosen? | compile time | run time |
| Cost per call | none (inlinable) | one indirect call |
| Can the set of types grow after compilation? | no | yes (also with `dlopen`) |
| Binary size | one copy per type | one implementation |

`Crtp` shows the template answer to polymorphism, `core/class/AbstractInterface` the virtual one,
and `dp/idiom/TypeErasure` the hybrid that `std::function` uses.

## 3. Where to look next

- `core/utils/TypeTraits` - `<type_traits>`, the detection idiom and concepts in practice
- `core/utils/Variadic` - parameter packs and fold expressions
- `core/function/PerfectForwarding` - forwarding references, which only exist in templates
