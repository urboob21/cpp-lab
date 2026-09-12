# cpp-lab

A C/C++ learning lab: ~100 small, runnable examples of modern C++ (C++20), design patterns,
concurrency, sockets and embedded basics - built with CMake, GoogleTest, cppcheck and lcov, with
Docker and GitHub Actions for CI/CD.

Every example lives in one file, explains itself in a header comment, and can be run on its own.

---
## 1. Quick start

```bash
./scripts/run.sh          # build + static analysis + tests + interactive menu
```

or step by step:

```bash
cmake -S . -B build
cmake --build build -j
./build/bin/cpp_lab_project           # interactive menu
./build/bin/cpp_lab_project --help    # command line
```

**Project hierarchy**
```
include/lab/   framework API: LAB_EXAMPLE, LOG..., registry, menu, runner
src/
  lab/         framework implementation
  main.cpp     entry point: command line and menu
  core/        C++ language and standard library examples
  dp/          design patterns (behavioral, creational, structural)
  controller/  control algorithms (PID)
  socket/      POSIX TCP sockets
  ap/          architecture patterns (MVC, MVVM) as GTK4 apps
  demo/        standalone demos (dlopen plugin)
  embedded/    bare-metal ARM example (own build script)
tests/         GoogleTest unit tests and a gtest/gmock primer
cmake/         CMake modules and helpers
docs/          documentation and UML diagrams
scripts/       build, coverage and scaffolding scripts
```

---
## 2. Using the lab

### 2.1. Interactive menu

The menu mirrors the folders below `src/`:

```
------------------------------------------------------------------------
 cpp-lab / core / smart_pointer   (3 examples)
------------------------------------------------------------------------
   1. Shared                      std::shared_ptr: reference counting, ...
   2. Unique                      std::unique_ptr: ownership, moves, ...
   3. Weak                        std::weak_ptr: lock/expired, ...
------------------------------------------------------------------------
 <number> open    0 back    <text> search    ? help    q quit
```

Type a number to open a folder or run an example, `0` to go back, `q` to quit, or any text to
search ids and descriptions.

### 2.2. Command line

```bash
./build/bin/cpp_lab_project --list                        # every example
./build/bin/cpp_lab_project --list pointer                # filter id or description
./build/bin/cpp_lab_project --run core/smart_pointer/Weak # run one example by id
./build/bin/cpp_lab_project --run weak                    # a filter matching exactly one
./build/bin/cpp_lab_project --run-all dp/behavioral       # all examples in a folder
./build/bin/cpp_lab_project --plain --run-all             # no log prefixes, no colors
```

Debug builds prefix every log line with `[time][file:line][function]` so you can see where output
comes from. Release builds (`-DCMAKE_BUILD_TYPE=Release`) and `--plain` print only the message.

### 2.3. Interactive examples

Examples marked `[interactive]` need a human or a second process and are skipped by `--run-all`
and by the tests. For instance, run `socket/simple_tcp/SimpleTCPServer`, then in another terminal:

```bash
telnet localhost 8080     # or: nc localhost 8080, or run socket/simple_tcp/SimpleTCPClient
```

Type lines to have them echoed, `Q` to disconnect, `SHUTDOWN` to stop the server.
`socket/simple_tcp/LoopbackEcho` shows the same client/server exchange inside one process.

---
## 3. Adding an example

```bash
./scripts/new_example.sh core/utils Span "std::span: a view over contiguous memory"
cmake --build build && ./build/bin/cpp_lab_project --run core/utils/Span
```

By hand, a complete example is:

```cpp
#include "lab/Example.h"
#include "lab/Logger.h"

namespace {
void demo() {
  LOG_SECTION("Basics");
  LOG_S("1 + 2 = " << 1 + 2);
}
}  // namespace

LAB_EXAMPLE("Span", "std::span: a view over contiguous memory") { demo(); }
```

No CMake edit is needed: every `.cpp` in a module folder is compiled, and the menu group comes
from the folder. See [docs/adding-examples.md](docs/adding-examples.md) for the conventions and
[docs/README.md](docs/README.md) for the topic index.

---
## 4. Dependencies

- **g++ >= 13** (or clang++ >= 16) with C++20 support, **CMake >= 3.16**, **git**
- Optional: `libgtkmm-4.0-dev` (GUI apps), `cppcheck`, `clang-tidy`, `clang-format`, `lcov` or
  `gcovr`, `valgrind`, `gdb`, `gcc-arm-none-eabi` + `qemu-system-arm` (bare-metal example)

```bash
sudo apt-get update
sudo apt-get install -y g++ cmake ninja-build git cppcheck clang-tidy clang-format \
                        lcov python3-gcovr valgrind gdb libgtkmm-4.0-dev
```

GoogleTest is downloaded automatically at configure time (needs network on the first configure;
use `-DCPPLAB_BUILD_TESTS=OFF` to skip it).

---
## 5. Build options

| Option | Default | Meaning |
|---|---|---|
| `CMAKE_BUILD_TYPE` | `Debug` | `Debug`, `Release`, `RelWithDebInfo` |
| `CPPLAB_BUILD_TESTS` | `ON` | unit tests and one smoke test per example |
| `CPPLAB_BUILD_DEMOS` | `ON` | standalone demos in `src/demo` |
| `CPPLAB_BUILD_GUI` | `AUTO` | GTK4 apps in `src/ap`: `AUTO`, `ON`, `OFF` |
| `CPPLAB_ENABLE_SANITIZERS` | `OFF` | AddressSanitizer + UndefinedBehaviorSanitizer |
| `CPPLAB_WARNINGS_AS_ERRORS` | `OFF` | `-Werror` |
| `ENABLE_COVERAGE` | `OFF` | `--coverage` for lcov/gcovr |

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCPPLAB_BUILD_GUI=OFF
```

---
## 6. Testing

```bash
ctest --test-dir build -j --output-on-failure     # everything
ctest --test-dir build -L unit                    # GoogleTest unit tests
ctest --test-dir build -L example                 # run every example once
ctest --test-dir build -R 'example:dp/'           # only the design patterns
./build/bin/cpp_lab_project_unit_test --gtest_filter='Registry*'
```

- Unit tests live in `tests/`; any `*Test.cpp` file is picked up automatically. `tests/gtest_primer`
  doubles as a GoogleTest/GoogleMock tutorial (see [tests/README.md](tests/README.md)).
- Smoke tests (`example:<id>`) are generated from the executable itself, so a new example is
  tested as soon as it exists.

**Sanitizers** catch memory errors, leaks and undefined behavior:

```bash
cmake -S . -B build-asan -DCPPLAB_ENABLE_SANITIZERS=ON
cmake --build build-asan -j && ctest --test-dir build-asan -j
```

**Memory leaks with valgrind**:

```bash
valgrind --leak-check=full ./build/bin/cpp_lab_project --plain --run-all core/class
```

---
## 7. Static analysis, formatting and coverage

```bash
# cppcheck (-I include is required so the LAB_EXAMPLE macro can be expanded)
cppcheck --enable=warning,style,performance,portability --inconclusive --inline-suppr --quiet \
         --error-exitcode=1 -I include --suppressions-list=.cppcheck-suppressions ./src ./include

# clang-tidy (configured by .clang-tidy)
clang-tidy -p build $(git ls-files 'src/*.cpp')

# clang-format (Google-based style, .clang-format)
git ls-files '*.cpp' '*.h' | xargs clang-format -i

# coverage reports
./scripts/gen_coverage_lcov.sh      # or ./scripts/gen_coverage_gcovr.sh
```

Intentional findings in teaching examples are listed in `.cppcheck-suppressions`.

---
## 8. The other programs

| Program | Run | Documentation |
|---|---|---|
| GTK4 apps | `./build/bin/ap`, `mvc_ap`, `mvvm_ap` | [src/ap/README.md](src/ap/README.md) |
| dlopen plugin demo | `./build/bin/demo_dlopen` | [src/demo/dlopen/README.md](src/demo/dlopen/README.md) |
| Bare-metal ARM (QEMU) | `cd src/embedded && ./run.sh` | [src/embedded/README.md](src/embedded/README.md) |

---
## 9. Debugging with VS Code

1. Install the *C/C++ Extension Pack* and `gdb` (`sudo apt install gdb`).
2. Press **F5**. `.vscode/launch.json` builds `build/debug` through the task in
   `.vscode/tasks.json` and starts `build/debug/bin/cpp_lab_project` under gdb.

| Shortcut | Action |
|---|---|
| F5 | start debugging |
| Ctrl + F5 | run without debugging |
| Ctrl + Shift + D | open the debug panel |
| Ctrl + Shift + B | build |

To debug a single example without the menu, set `"args": ["--run", "core/smart_pointer/Weak"]` in
`launch.json`.

---
## 10. Docker

```bash
# Build the image (swap DOCKER_USERNAME for your account)
docker build -t DOCKER_USERNAME/cpp-lab .
docker image ls

# Open a shell inside the image
docker run -it --rm DOCKER_USERNAME/cpp-lab:latest /bin/bash

# Push it (CI uses urboob21/cpp-lab:latest)
docker push DOCKER_USERNAME/cpp-lab
```

`-i` keeps stdin open, `-t` allocates a terminal, `--rm` removes the container when it exits.

---
## 11. Troubleshooting

| Symptom | Fix |
|---|---|
| `gtkmm-4.0 not found` | `sudo apt install libgtkmm-4.0-dev`, or configure with `-DCPPLAB_BUILD_GUI=OFF` |
| FetchContent cannot download GoogleTest | configure with `-DCPPLAB_BUILD_TESTS=OFF` |
| `Does not match the generator used previously` | delete the `build/` directory and configure again |
| `registry error: duplicate example id ...` | two `LAB_EXAMPLE` names collide in one folder |
| cppcheck reports `syntax error` on `LAB_EXAMPLE` | add `-I include` so the macro can be expanded |
| `push access denied` when pushing the image | `docker login` first |

---
## 12. Evaluating the executable

```bash
size ./build/bin/cpp_lab_project
```

```
   text    data     bss     dec     hex filename
 xxxxxx    xxxx    xxxx  xxxxxx   xxxxx ./build/bin/cpp_lab_project
```

- `.text`: executable code, including inlined and template code, constants and string literals
- `.data`: initialized global and static variables
- `.bss`: uninitialized global and static variables
