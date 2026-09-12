# A learning path through the lab

The menu is organized by topic; this page suggests an order. Run an example with

```bash
./build/bin/cpp_lab_project --run <id>
```

and read its source next to the output - the header comment of each file says what it teaches.

---
## Stage 1 - the language you write every day

| # | Example | Take away |
|---|---------|-----------|
| 1 | `core/basics/Initialization` | prefer `{}`; know what stays uninitialized |
| 2 | `core/basics/Operations` | integer division, unsigned wrap-around, short-circuit |
| 3 | `core/basics/ControlFlow` | if with initializer, range-for, why `goto` is rare |
| 4 | `core/datatype/Fundamental` | sizes are minimums; use `<cstdint>` when width matters |
| 5 | `core/datatype/Struct` | aggregates, designated initializers, padding |
| 6 | `core/datatype/Enum` | `enum class` over plain `enum` |
| 7 | `core/basics/TypeQualifier` | const vs constexpr vs consteval |

## Stage 2 - memory and indirection

| # | Example | Take away |
|---|---------|-----------|
| 8 | `core/datatype/Pointer` | `&`, `*`, the three const positions, pointer arithmetic |
| 9 | `core/datatype/Reference` | references cannot be null or re-seated; `std::move` is a cast |
| 10 | `core/datatype/Array` | arrays decay to pointers and lose their size |
| 11 | `core/smart_pointer/Unique` | ownership belongs in the type system |
| 12 | `core/smart_pointer/Shared` | shared ownership costs a control block |
| 13 | `core/smart_pointer/Weak` | break reference cycles |

## Stage 3 - your own types

| # | Example | Take away |
|---|---------|-----------|
| 14 | `core/class/Encapsulation` | invariants live behind a small interface |
| 15 | `core/class/Constructor` | initializer lists, delegating, explicit |
| 16 | `core/class/Destructor` | destruction order and RAII |
| 17 | `core/class/ShallowDeepCopying` | copying a pointer is not copying the data |
| 18 | `core/class/RuleOfThreeFiveZero` | write five, or better none |
| 19 | `core/class/Binding` | hiding vs overriding, virtual dispatch |
| 20 | `core/class/Relationship` | composition, aggregation, association |
| 21 | `core/function/operator_overloading/ArithmeticOperator` | operators behave like the built-in ones |
| 22 | `core/function/operator_overloading/ComparisonOperator` | `<=>` writes the comparisons for you |

## Stage 4 - the standard library

| # | Example | Take away |
|---|---------|-----------|
| 23 | `core/string/StdString` | `std::string` and `std::string_view` |
| 24 | `core/string/StringFormatting` | `std::format` beats streams and `printf` |
| 25 | `core/container/sequence/Vector` | capacity, reallocation, invalidation |
| 26 | `core/container/unordered/UnorderedMap` | `operator[]` inserts; `find`/`contains` do not |
| 27 | `core/container/associative/Set` | sorted, unique, `lower_bound` |
| 28 | `core/utils/Algorithm` | name the loop: algorithms and ranges |
| 29 | `core/function/Lambda` | captures decide lifetime |
| 30 | `core/function/Functional` | `std::function` for stored callbacks |
| 31 | `core/utils/Optional` | say "no value" in the type |

## Stage 5 - failure, files and time

| # | Example | Take away |
|---|---------|-----------|
| 32 | `core/exception/BasicHandle` | throw by value, catch by const reference |
| 33 | `core/exception/ThrowNoexcept` | `noexcept` moves make containers fast |
| 34 | `core/filehandle/IOStream` | stream states and input validation |
| 35 | `core/filehandle/FileIO` | text files with RAII |
| 36 | `core/filehandle/Directory` | `std::filesystem` |
| 37 | `core/datetime/Time` | `<chrono>` and the C time API |

## Stage 6 - concurrency

| # | Example | Take away |
|---|---------|-----------|
| 38 | `core/concurrency/ThreadManagement` | join or `std::jthread`, never a dangling thread |
| 39 | `core/concurrency/SharingData` | arguments are copied unless you say otherwise |
| 40 | `core/concurrency/RaceCondition` | see updates get lost, then fix it |
| 41 | `core/concurrency/ConditionVariable` | wait with a predicate |
| 42 | `core/concurrency/FuturePromise` | results and exceptions across threads |

## Stage 7 - design patterns

Start with the ones you will recognize immediately:
`dp/creational/FactoryMethod`, `dp/structural/Adapter`, `dp/structural/Decorator`,
`dp/behavioral/Strategy`, `dp/behavioral/Observer`, `dp/behavioral/Command`.

Then the rest of `dp/creational`, `dp/structural` and `dp/behavioral`. Each example first shows
the problem the pattern solves; several end with the modern C++ alternative (`std::function`,
`std::variant`, ranges).

## Stage 8 - systems and applications

| Example / program | Take away |
|---|---|
| `core/linkage/Linkage`, `core/linkage/sharing/Sharing` | what the linker sees |
| `socket/simple_tcp/LoopbackEcho` | sockets end to end in one process |
| `socket/simple_tcp/SimpleTCPServer` + `SimpleTCPClient` | the same across two processes |
| `socket/multiple_client/MultiTCPServer` | a thread per connection |
| `controller/pid/PIDSim` | a control loop |
| `./build/bin/mvc_ap`, `./build/bin/mvvm_ap` | the same GUI with two architectures |
| `./build/bin/demo_dlopen` | loading a plugin at run time |
| `src/embedded` | a firmware image booting in QEMU |

## Stage 9 - what is not written yet

Examples marked `[draft]` in the menu are scaffolds: the file lists what the topic should show and
prints that outline, but the code is missing. Templates (`core/template/*`), move semantics,
coroutines, `std::expected` and the rest of the open topics are listed per standard in
[cpp-standards-coverage.md](cpp-standards-coverage.md).

```bash
./build/bin/cpp_lab_project --list | grep draft
```

Filling one in is the best exercise the lab has to offer.

---
When something surprises you, change the example and rebuild - that is what the lab is for.
Then write your own: see [adding-examples.md](adding-examples.md).
