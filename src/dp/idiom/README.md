# C++ Idioms

Patterns that are not in the Gang of Four book but that every C++ codebase runs into.

| Example | Topic |
|---|---|
| `Pimpl` | hide the implementation behind a pointer: stable ABI, faster rebuilds *(draft)* |
| `TypeErasure` | store any type that satisfies an interface, without a common base *(draft)* |

> Both examples are still **drafts**: the file lists what it should teach and prints that outline
> when you run it.

---
## Idioms already shown elsewhere in the lab

| Idiom | Where |
|---|---|
| RAII | `core/class/Destructor`, `core/smart_pointer/Unique` |
| Rule of 0/3/5 | `core/class/RuleOfThreeFiveZero` |
| Copy and swap | `core/function/operator_overloading/AssignmentOperator` |
| Non-virtual interface | `core/class/AbstractInterface` *(draft)* |
| CRTP | `core/template/Crtp` *(draft)* |
| Meyers' singleton | `dp/creational/Singleton` |
| SFINAE / detection idiom | `core/utils/TypeTraits`, `core/template/Sfinae` *(draft)* |
