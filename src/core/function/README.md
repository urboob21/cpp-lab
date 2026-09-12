# Functions and callables

| Example           | Topic                                                          |
|-------------------|----------------------------------------------------------------|
| `FunctionPointer` | function pointer syntax, comparators, dispatch tables, member function pointers |
| `Lambda`          | captures, `mutable`, init captures, generic lambdas, closures  |
| `Functional`      | `std::function`, function objects, `std::bind`, `std::invoke`   |
| `PerfectForwarding` | forwarding references, std::forward, reference collapsing and forwarding factories *(draft)* |

Operator overloading is in the [operator_overloading](operator_overloading) folder.

## 1. Kinds of callables

| Callable                   | Can hold state | Type                         |
|----------------------------|----------------|------------------------------|
| free function              | no             | `int (*)(int)`               |
| capture-less lambda        | no             | unique type, converts to a function pointer |
| lambda with captures       | yes            | unique closure type          |
| function object (functor)  | yes            | a class with `operator()`    |
| `std::function<int(int)>`  | yes            | type-erased wrapper for all of the above |

Use templates or `auto` parameters to accept callables with zero overhead. Use `std::function`
when you need to store callables of different types in one place, such as callbacks or command
maps.

## 2. Lambda captures

```cpp
[x]        // copy of x (const inside the body unless the lambda is mutable)
[&x]       // reference to x - must not outlive x!
[=], [&]   // capture everything used, by copy / by reference
[y = f()]  // init capture (C++14), e.g. [p = std::move(ptr)]
[this]     // the current object (by pointer)
```

## 3. Operator overloading

| Operator            | Canonical form                                             | Example |
|---------------------|------------------------------------------------------------|---------|
| `+ - * /`           | non-member, implemented with the member `+= -= *= /=`       | `ArithmeticOperator` |
| `= `                | member, returns `T&`, handles self-assignment              | `AssignmentOperator` |
| `<=> ==`            | `auto operator<=>(const T&) const = default;` (C++20)      | `ComparisonOperator` |
| `<< >>` on streams  | non-member (the stream is the left operand)                | `StreamOperator` |
| `[]`                | member, const and non-const overloads                      | `SubscriptOperator` |
| `()`                | member, turns an object into a function object             | `FunctionCallOperator` |
| `++ --`             | prefix returns `T&`, postfix `T operator++(int)` returns the old value | `InDecOperator` |
| `-> *`              | member, used by smart pointers and iterators               | `ClassMemberAccessOperator` |
| `operator T()`      | conversion operator, prefer `explicit`                     | `TypeCast` |
| `new delete`        | class-specific allocation                                  | `AllocationOperator` |
| `- + ! ~`           | member, returns a new value                                | `UnaryOperator` |

Rule of thumb: an overloaded operator should do what the built-in operator would do. Anything
surprising belongs in a named function.
