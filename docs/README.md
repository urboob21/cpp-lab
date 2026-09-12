# CPP Lab Docs

Every folder below `src/` is a group in the lab's menu. The README in each folder explains the
topic, and the examples next to it show it in code. Run any example with
`./build/bin/cpp_lab_project --run <id>`, or browse them with the interactive menu.

- [Adding examples](adding-examples.md): how the lab works and how to add your own examples

---
## 1. Core (C++ language and standard library)

1. [Basics](../src/core/basics/README.md): control flow, initialization, operators, const/constexpr
2. [Data types](../src/core/datatype/README.md): fundamental types, pointers, references, enums, structs, unions, conversions
3. [Classes](../src/core/class/README.md): constructors, destructors, Rule of 3/5/0, relationships, friends, virtual
4. [Functions](../src/core/function/README.md): function pointers, lambdas, std::function, operator overloading
5. [Linkage](../src/core/linkage/README.md): internal/external linkage, sharing global constants
6. [Smart pointers](../src/core/smart_pointer/README.md): unique_ptr, shared_ptr, weak_ptr
7. [Strings](../src/core/string/README.md): C strings, std::string, string_view, formatting
8. [Containers](../src/core/container/README.md): sequence, associative, unordered, adapters
9. [Exceptions](../src/core/exception/README.md): try/catch/throw, noexcept
10. [File handling](../src/core/filehandle/README.md): streams, files, filesystem, formatting
11. [Date and time](../src/core/datetime/README.md): `<ctime>` and `<chrono>`
12. [Concurrency](../src/core/concurrency/README.md): threads, mutexes, condition variables, futures
13. [Utilities](../src/core/utils/README.md): optional, algorithms, regex, type traits, variadics

---
## 2. Design Patterns

1. [Behavioral](../src/dp/behavioral/README.md)
2. [Structural](../src/dp/structural/README.md)
3. [Creational](../src/dp/creational/README.md)

---
## 3. Architecture Patterns

1. [MVC/MVVM](../src/ap/README.md) (GTK4 applications)

---
## 4. Other

1. [Controller (PID)](../src/controller/README.md)
2. [Socket](../src/socket/README.md)
3. [dlopen plugin demo](../src/demo/dlopen/README.md)
4. [Embedded bare-metal ARM](../src/embedded/README.md)
5. [Google Test](../tests/README.md)
