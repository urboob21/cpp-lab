# Linkage

| Example                  | Files                                        |
|--------------------------|----------------------------------------------|
| `Linkage`                | `Linkage.cpp`, `External.h/.cpp`, `Internal.cpp` |
| `sharing/Sharing`        | `sharing/Sharing.cpp` and the three `constants` variants |
| `OneDefinitionRule`      | `OneDefinitionRule.cpp` - the ODR, `inline` and templates *(draft)* |
| `Modules`                | `Modules.cpp` - C++20 `export module` / `import` *(draft)* |

## 1. Translation units and the linker

Each `.cpp` file is compiled on its own into an object file (a *translation unit*). The linker then
combines the object files and connects every use of a name with its definition.

| Linkage  | Visible                     | Typical entities                                  |
|----------|-----------------------------|---------------------------------------------------|
| none     | only in its block           | local variables                                   |
| internal | only in its `.cpp` file     | `static` globals, `const`/`constexpr` globals, anything in an anonymous namespace |
| external | in the whole program        | non-static functions and globals, `extern` variables, `inline` variables |

**One Definition Rule (ODR):** an entity with external linkage must have exactly one definition in
the whole program. Inline functions and variables are the exception: they may be defined in
several files if every definition is identical.

Common linker errors:

- `undefined reference to ...`: something is declared but never defined, or its internal linkage
  hides it from other files.
- `multiple definition of ...`: a non-inline variable or function is defined in a header that is
  included by several `.cpp` files.

## 2. Sharing global constants

| Approach                             | Copies     | Constant expression elsewhere | Recompile on change |
|--------------------------------------|------------|-------------------------------|---------------------|
| `inline constexpr` in a header (C++17) | one        | yes                           | all includers       |
| `constexpr` in a header              | one per file | yes                         | all includers       |
| `extern const` + definition in `.cpp` | one        | no                            | only that `.cpp`    |

The `Sharing` example prints the address of each constant as seen from two different files. This
shows that internal constants are copied into every file, while inline constants are shared.
