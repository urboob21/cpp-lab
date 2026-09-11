# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this repo is

A C++20 learning lab: many small, self-contained examples (language features, STL, concurrency, design patterns, sockets, a PID controller) compiled into one interactive menu-driven executable, plus a few standalone executables (GTK4 MVC/MVVM apps, a `dlopen` demo) and a bare-metal ARM example that sits outside CMake.

## Build & run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug   # configure
cmake --build build                            # build all targets
./build/bin/cpp_lab_project                    # interactive examples menu (optional: -mode Dev|Uat|Prod)
./scripts/run.sh                               # build + cppcheck + run (needs private/genid.py, which is gitignored)
```

- Configuring needs **gtkmm-4.0** (`libgtkmm-4.0-dev`). `cmake/Dependencies.cmake` marks it `REQUIRED`, so configure fails without it, even if you only want the main executable. GoogleTest is fetched over the network with FetchContent on the first configure.
- Executables go to `build/bin/` and shared libs to `build/lib/`. VS Code tasks/launch use `build/debug` instead.
- Other targets: `ap`, `mvc_ap`, `mvvm_ap` (GTK4 GUIs in `src/ap/`) and `demo_dlopen` (loads `libsample_app.so` at runtime; see `src/demo/dlopen/README.md` for why `bridge` must stay SHARED).
- `src/embedded/` is not part of CMake. Build and boot it in QEMU with `cd src/embedded && ./run.sh [gui|debug]` (requires `gcc-arm-none-eabi` and `qemu-system-arm`).

## Tests

GoogleTest/GMock tests live in `tests/` (the target is `cpp_lab_project_unit_test`; mocks and fakes are in `tests/mock/`). **`add_subdirectory(tests)` is currently commented out in the top-level `CMakeLists.txt`**, so uncomment it before building or running tests.

```bash
ctest --test-dir build --output-on-failure             # all tests
ctest --test-dir build -R 'DivTest'                    # subset (tests registered via gtest_discover_tests)
./build/bin/cpp_lab_project_unit_test --gtest_filter='DivTest.InvalidNumbers'
./scripts/gen_coverage_lcov.sh                         # or gen_coverage_gcovr.sh; CI uses -DENABLE_COVERAGE=ON
```

The test target compiles only the sources listed in `tests/CMakeLists.txt` (currently `src/DeleteMe.cpp`). Add any source under test to that list.

## Static analysis & formatting

CI (`.github/workflows/cpp-build-test-coverage.yml`, on push/PR to `master`, inside the `urboob21/cpp-lab:latest` Docker image) fails on cppcheck findings:

```bash
cppcheck --enable=warning,style,performance,portability --inconclusive --inline-suppr --quiet --error-exitcode=1 ./src ./include
clang-tidy -p build -header-filter='^src/.*' $(find src -name "*.cpp")   # not enforced in CI
clang-format -i <file>                                                     # Google-based style (.clang-format)
```

The `.clang-tidy` naming rules are lower_case variables and namespaces, CamelCase types, and a trailing `_` on class members. Code also uses `kName` for constants and enumerators.

## Architecture: the example registry

The main executable is a plugin-style registry that fills itself during static initialization:

- `include/IExample.h` is the interface. It has `group()`, `name()`, `description()`, and `execute()`.
- `include/ExampleRegistry.h` is a singleton map `group -> name -> factory`. The `REGISTER_EXAMPLE(Class)` macro at the bottom of each example `.cpp` defines a static registrar object that registers the class before `main` runs.
- `src/main.cpp` only renders the menu from the registry. Groups are shown sorted, but examples inside a group follow `unordered_map` order, so menu numbers are unstable and piping choices into stdin is unreliable.

**Adding an example:**
1. Create a `.cpp` that puts its code in an anonymous namespace (often `namespace { namespace problem {...} namespace some_pattern {...} }`, each with its own `run()`).
2. Define an `IExample` subclass and call `REGISTER_EXAMPLE(ThatClass);`.
3. **Add the file to the module's source list** (`CORE_SOURCES`, `DP_SOURCES`, `SOCKET_SOURCES`, or `CONTROLLER_SOURCES` in `src/<module>/CMakeLists.txt`). There is no globbing, and a file missing from the list simply never shows up in the menu, with no error.

Every module's sources link into the same `cpp_lab_project` binary. Keep helpers in anonymous namespaces and give the registered example class a unique name, because many files share names like `Array.cpp` and define their own `run()`. The `group()` string sets the menu category and doesn't always match the directory (for example, `function/operator_overloading/*` registers as `core/overloading_operator`).

Use `LOG(msg)` / `LOG_S(a << b)` from `include/Logger.h` for output. Debug builds add a timestamp, file:line, and function; `NDEBUG` builds print plain lines. `version.h` is generated into `build/generated/` from `include/version.h.in`.

## Docs

`docs/README.md` indexes the per-module READMEs (`src/**/README.md`), which explain each topic and embed the draw.io UML diagrams from `docs/uml/`. When you add a design pattern example, update the matching README (and diagram if one exists).
