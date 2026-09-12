# Adding examples

The lab is built so that adding an example takes one file and no build-system changes.

## 1. The quick way

```bash
./scripts/new_example.sh core/utils Span "std::span: a view over contiguous memory"
cmake --build build
./build/bin/cpp_lab_project --run core/utils/Span

# --draft scaffolds a topic you want to write later
./scripts/new_example.sh --draft core/utils Mdspan "std::mdspan: multidimensional views"
```

The script creates `src/core/utils/Span.cpp` from a template. Edit it and rebuild. The build finds
the new file on its own (CMake globs with `CONFIGURE_DEPENDS`).

## 2. Anatomy of an example

```cpp
// -----------------------------------------------------------------------------
// std::span (C++20)
//
// A non-owning view over a contiguous sequence: pointer + size.
//
// Key points:
//   - Replaces "pointer + length" parameters.
//   - Never outlives the data it views.
//
// Reference: https://en.cppreference.com/w/cpp/container/span
// -----------------------------------------------------------------------------

#include <span>
#include <vector>

#include "lab/Example.h"   // LAB_EXAMPLE
#include "lab/Logger.h"    // LOG, LOG_S, LOG_FUNC, LOG_SECTION

namespace {  // everything file-local: no name clashes with other examples

int sum(std::span<const int> values) {
  int total = 0;
  for (const int value : values) {
    total += value;
  }
  return total;
}

void basics() {
  LOG_SECTION("Passing arrays and vectors as std::span");
  const int array[] = {1, 2, 3};
  const std::vector<int> vector{4, 5, 6};
  LOG_S("sum(array) = " << sum(array) << ", sum(vector) = " << sum(vector));
}

}  // namespace

LAB_EXAMPLE("Span", "std::span: a view over contiguous memory") {
  basics();
}
```

### LAB_EXAMPLE

```cpp
LAB_EXAMPLE(name, description [, flags]) { body }
```

| Part          | Meaning                                                                           |
|---------------|-----------------------------------------------------------------------------------|
| `name`        | Short name shown in the menu. It must be unique in its folder and contain no spaces or `/`. |
| `description` | One line shown next to the name and searched by `--list` and the menu search.   |
| `flags`       | Optional, combine with `|`. `lab::kInteractive` marks examples that need a human or another process; `lab::kDraft` marks a scaffold that is not written yet. |
| folder        | Taken from the file's location: `src/core/utils/Span.cpp` becomes group `core/utils`. |
| id            | `<folder>/<name>`, e.g. `core/utils/Span`. Use it with `--run`.                     |

How it works: the macro declares a file-local function and registers its address with
`lab::Registry` from a static initializer, which runs before `main()`. The braces that follow the
macro become the body of that function. Duplicate ids and invalid names are reported when the
program starts.

### Logging

| Macro                     | Output                                                        |
|---------------------------|---------------------------------------------------------------|
| `LOG("text")`             | one line (anything convertible to `std::string_view`)         |
| `LOG_S("x = " << x)`      | anything you can stream into `std::ostream`                    |
| `LOG_FUNC()`              | the full signature of the current function (which overload ran?) |
| `LOG_SECTION("Title")`    | a separator line that structures the output                   |

Debug builds prefix each line with `[time][file:line][function]`. Release builds and `--plain`
print only the message.

## 3. Conventions

- **One topic per file**, named after the topic in `CamelCase.cpp`. Put the file in the folder
  where learners will look for it.
- **Header comment:** what the example teaches, key points and pitfalls, and a reference link.
- **Anonymous namespace** for all helpers. All examples are linked into one program, so two files
  that both define `void run()` or `class Model` at global scope would break the One Definition
  Rule.
- **Sections:** split the example into small functions, each starting with `LOG_SECTION`.
- **Show the problem, then the solution** when a topic fixes a problem (see the design patterns).
- **Never execute undefined behavior on purpose.** Describe it in a comment, or build a safe
  demonstration. The smoke tests also run with AddressSanitizer and UBSan.
- **Clean up:** create files only in `std::filesystem::temp_directory_path()` and remove them.
  Restore global state you change, such as `std::set_terminate`.
- **Be fast and non-interactive** unless the topic needs otherwise. Keep sleeps short. Use
  `std::istringstream` instead of `std::cin`. Mark examples that must wait for a user or a peer
  with `lab::kInteractive`.
- Format with the repository's `.clang-format` (Google-based style).
- **Drafts:** an example that only describes what it should teach keeps `lab::kDraft`. The menu
  shows it as `[draft]`, and running it prints the outline from its header comment. Remove the flag
  in the same commit that writes the code. The open ones are listed in
  [cpp-standards-coverage.md](cpp-standards-coverage.md).

## 4. Testing your example

```bash
cmake --build build
ctest --test-dir build -R example:core/utils/Span --output-on-failure
```

Every non-interactive example automatically becomes a ctest smoke test named `example:<id>`. The
test fails if the example throws an uncaught exception. It also fails under the sanitizer build
if the example leaks memory or executes undefined behavior:

```bash
cmake -S . -B build-asan -DCPPLAB_ENABLE_SANITIZERS=ON
cmake --build build-asan -j && ctest --test-dir build-asan -L example -j
```

Reusable code (not just demonstration output) deserves a real unit test in `tests/`. Any
`tests/**/*Test.cpp` file is picked up automatically (see `tests/controller/PidTest.cpp`).

## 5. A new top-level topic

To start a new module such as `src/algorithms/`:

1. Create the folder and add examples to it.
2. Add one line to `src/CMakeLists.txt`:
   ```cmake
   cpplab_add_example_module(algorithms)
   ```
3. Add a `README.md` to the folder and link it from `docs/README.md`.

## 6. Standalone programs

Programs with their own `main()` do not use `LAB_EXAMPLE`: GUI apps, plugins, and firmware. Give
them their own `CMakeLists.txt`, as in `src/ap` and `src/demo`, and add them with
`add_subdirectory()` in `src/CMakeLists.txt`.
