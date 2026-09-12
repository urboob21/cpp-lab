# Standard coverage: C++11 -> C++23

What the lab already teaches, and what is still missing. The checklist follows the feature lists of
each standard, cross-checked against the syllabus of
[federico-busato/Modern-CPP-Programming](https://github.com/federico-busato/Modern-CPP-Programming)
(chapters 3-22 map onto the `core/` modules, 26-27 onto `dp/`).

Legend:

| | Meaning |
|---|---|
| **done** | a finished example demonstrates it |
| **draft** | the file exists, lists what it should show and prints that outline; the code is not written yet (marked `[draft]` in the menu) |
| **partial** | touched inside another example, no example of its own |

List every unfinished example with:

```bash
./build/bin/cpp_lab_project --list | grep draft
```

---
## 1. C++11

| Feature | Status | Example |
|---|---|---|
| `auto`, `decltype`, trailing return types | done | `core/datatype/TypeConversions` |
| Range-based `for`, `nullptr`, uniform init | done | `core/basics/ControlFlow`, `core/basics/Initialization` |
| Scoped enums, underlying types | done | `core/datatype/Enum` |
| `constexpr`, `static_assert` | done | `core/basics/TypeQualifier` |
| Lambdas and closures | done | `core/function/Lambda` |
| Rvalue references, `std::move` | done | `core/datatype/Reference` |
| Move constructor / move assignment | **draft** | `core/class/MoveSemantics` |
| Perfect forwarding, reference collapsing | **draft** | `core/function/PerfectForwarding` |
| Variadic templates, parameter packs | done | `core/utils/Variadic` |
| Defaulted / deleted / delegating constructors | done | `core/class/Constructor` |
| `override`, `final`, inheriting constructors | **draft** | `core/class/Inheritance` |
| Smart pointers | done | `core/smart_pointer/{Unique,Shared,Weak}` |
| `std::function`, `std::bind` | done | `core/function/Functional` |
| `std::array`, unordered containers | done | `core/container/...` |
| `std::tuple`, `std::tie` | **draft** | `core/utils/Tuple` |
| `<chrono>` | done | `core/concurrency/Timing`, `core/datetime/Time` |
| `<random>` | **draft** | `core/utils/Random` (used in passing by `core/basics/ControlFlow`) |
| `<regex>` | done | `core/utils/Regex` |
| `<type_traits>` | done | `core/utils/TypeTraits` |
| Threads, mutex, condition variable, futures | done | `core/concurrency/*` |
| `std::atomic`, memory orders | **draft** | `core/concurrency/Atomic` (a counter appears in `RaceCondition`) |
| `thread_local` | done | `core/concurrency/SharingData` |
| `noexcept` | done | `core/exception/ThrowNoexcept` |
| Raw string literals, user-defined literals | **draft** | `core/datatype/UserDefinedLiteral` |
| Alias templates (`using`) | done | `core/datatype/TypeConversions` |
| Attributes (`[[noreturn]]`, ...) | **draft** | `core/basics/Attributes` |
| Function / class templates, specialization | **draft** | `core/template/*` (nine drafts) |

## 2. C++14

| Feature | Status | Example |
|---|---|---|
| Generic lambdas, init captures | done | `core/function/Lambda` |
| `std::make_unique` | done | `core/smart_pointer/Unique` |
| Binary literals, digit separators | done | `core/basics/Operations` |
| Return type deduction, relaxed `constexpr` | **draft** | `core/template/FunctionTemplate` |
| Variable templates | **draft** | `core/template/VariableTemplate` |
| `std::exchange` | partial | used by `core/class/RuleOfThreeFiveZero`, explained in `core/utils/Utility` *(draft)* |
| `std::shared_timed_mutex` | **draft** | `core/concurrency/SharedMutex` |
| `[[deprecated]]` | **draft** | `core/basics/Attributes` |

## 3. C++17

| Feature | Status | Example |
|---|---|---|
| Structured bindings | done | `core/datatype/Struct` |
| `if` / `switch` with initializer | done | `core/basics/ControlFlow` |
| `if constexpr` | partial | `core/basics/TypeQualifier`; in depth in `core/template/Sfinae` *(draft)* |
| Fold expressions | done | `core/utils/Variadic` |
| Class template argument deduction, deduction guides | **draft** | `core/template/ClassTemplate` |
| Inline variables | done | `core/linkage/sharing/Sharing` |
| `std::optional` | done | `core/utils/Optional` |
| `std::variant` | done | `core/datatype/Union` |
| `std::any` | **draft** | `core/utils/Any` |
| `std::string_view` | done | `core/string/StdString` |
| `std::filesystem` | done | `core/filehandle/Directory` |
| `std::byte` | **draft** | `core/datatype/Byte` |
| `std::apply`, `std::make_from_tuple` | **draft** | `core/utils/Tuple` |
| `std::invoke` | done | `core/function/Functional` |
| `std::shared_mutex`, `std::scoped_lock` | **draft** | `core/concurrency/SharedMutex` |
| Parallel algorithms (execution policies) | **draft** | `core/concurrency/ParallelAlgorithm` |
| `reduce`, `transform_reduce`, scans | **draft** | `core/utils/Numeric` |
| Map/set improvements (`try_emplace`, `extract`, `merge`) | **draft** | `core/container/associative/Map` |
| Guaranteed copy elision | **draft** | `core/class/MoveSemantics` |
| `[[nodiscard]]`, `[[maybe_unused]]`, `[[fallthrough]]` | **draft** | `core/basics/Attributes` |

## 4. C++20

| Feature | Status | Example |
|---|---|---|
| Concepts | partial | `core/utils/TypeTraits`; in depth in `core/template/Concepts` *(draft)* |
| Ranges and views | partial | `core/utils/Algorithm`; in depth in `core/utils/Ranges` *(draft)* |
| `std::format` | done | `core/string/StringFormatting`, `core/filehandle/OutputFormatting` |
| Three-way comparison `<=>` | done | `core/function/operator_overloading/ComparisonOperator` |
| Designated initializers | done | `core/basics/Initialization` |
| `consteval`, `constinit` | done | `core/basics/TypeQualifier` |
| `std::span` | partial | `core/datatype/Array`; in depth in `core/utils/Span` *(draft)* |
| `std::jthread`, `std::stop_token` | done | `core/concurrency/ThreadManagement` |
| `std::latch`, `std::barrier`, semaphores | **draft** | `core/concurrency/Synchronization` |
| `std::atomic_ref`, wait/notify | **draft** | `core/concurrency/Atomic` |
| Coroutines (`co_await`, `co_yield`) | **draft** | `core/concurrency/Coroutine` |
| Modules | **draft** | `core/linkage/Modules` (documentation only: modules need extra build support) |
| `std::bit_cast` | done | `core/datatype/Union` |
| `<bit>`: `popcount`, `rotl`, `bit_width` | **draft** | `core/datatype/BitOperations` |
| Calendar and time zones in `<chrono>` | done | `core/datetime/Time` |
| `std::source_location` | partial | used by `lab/Logger`; explained in `core/utils/SourceLocation` *(draft)* |
| `starts_with` / `ends_with`, `erase_if` | done | `core/string/StdString`, `core/container/sequence/Vector` |
| `using enum` | done | `core/datatype/Enum` |
| `std::cmp_less` and friends | **draft** | `core/utils/Utility` |
| Abbreviated function templates (`auto` parameters) | **draft** | `core/template/FunctionTemplate` |
| `[[likely]]`, `[[unlikely]]` | **draft** | `core/basics/Attributes` |

## 5. C++23

| Feature | Status | Example |
|---|---|---|
| `std::expected` | **draft** | `core/utils/Expected` |
| `std::print`, `std::println` | **draft** | `core/filehandle/Print` |
| Deducing `this` (explicit object parameter) | **draft** | `core/class/DeducingThis` |
| `std::mdspan` | **draft** | `core/utils/Mdspan` |
| `std::flat_map`, `std::flat_set` | **draft** | `core/container/adapter/FlatMap` |
| `std::generator` | **draft** | `core/concurrency/Coroutine` |
| `ranges::to`, `zip`, `chunk`, `adjacent` | **draft** | `core/utils/Ranges` |
| `std::to_underlying` | partial | used by `core/datatype/Enum`, listed in `core/utils/Utility` *(draft)* |
| `std::byteswap` | **draft** | `core/datatype/BitOperations` |
| `std::stacktrace` | **draft** | `core/utils/SourceLocation` |
| `[[assume]]`, `if consteval` | **draft** | `core/basics/Attributes` |
| Multidimensional subscript `m[i, j]` | **draft** | `core/utils/Mdspan` |

Several C++23 library features need a recent toolchain (GCC 13/14, Clang 16/18). Each draft names
the version it needs; check before writing the code, and guard with a feature-test macro
(`__cpp_lib_expected`, `__cpp_lib_print`, ...) if the example must build everywhere.

## 6. Design patterns and idioms

The 23 Gang of Four patterns were complete except **Interpreter**, which is now a draft under
`dp/behavioral`. The new `dp/idiom/` folder collects C++-specific patterns: `Pimpl` and
`TypeErasure` *(both drafts)*; RAII, rule of 0/3/5, copy-and-swap, CRTP and the Meyers singleton are
already shown elsewhere (see [src/dp/idiom/README.md](../src/dp/idiom/README.md)).

## 7. Deliberately out of scope

- **C++26**: still moving (reflection, contracts, `std::execution`). Nothing is planned yet.
- **Modules as a build feature**: the lab stays header-based; `core/linkage/Modules` explains the
  syntax and what it would take to switch.
- **Compiler internals and optimisation** (chapters 23-25 of the reference course): benchmarking
  belongs in a separate project, not in a single-process menu of examples.

---
## 8. Filling in a draft

1. Run it: `./build/bin/cpp_lab_project --run core/utils/Expected` - it prints its own outline.
2. Open the file, replace `outline()` with real demonstrations (keep helpers in the anonymous
   namespace, structure the output with `LOG_SECTION`).
3. Remove `lab::kDraft` from `LAB_EXAMPLE` and update the table in the folder's `README.md`.
4. `cmake --build build && ctest --test-dir build -R "example:core/utils/Expected"`.

Conventions and the full checklist: [adding-examples.md](adding-examples.md).
