# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this repo is

A C++20 learning lab: ~100 small, self-contained examples (language features, STL, concurrency,
design patterns, sockets, a PID controller) compiled into one menu-driven executable, plus
standalone programs (GTK4 MVC/MVVM apps, a `dlopen` plugin demo) and a bare-metal ARM example that
is built outside CMake.

## Build, run, test

```bash
cmake -S . -B build                 # Debug by default
cmake --build build -j
./build/bin/cpp_lab_project         # interactive menu
ctest --test-dir build -j --output-on-failure
./scripts/run.sh                    # build + cppcheck + tests + menu
```

Command line of the app: `--list [filter]`, `--run <id|filter>`, `--run-all [filter]`,
`--list-ids`, `--plain` (no log prefixes/colors), `--mode dev|uat|prod`, `--version`, `--help`.
Example ids look like `core/smart_pointer/Weak`.

CMake options: `CPPLAB_BUILD_TESTS` (ON), `CPPLAB_BUILD_DEMOS` (ON), `CPPLAB_BUILD_GUI`
(AUTO/ON/OFF - gtkmm-4.0 is optional), `CPPLAB_BUILD_DOCS` (ON, needs doxygen),
`CPPLAB_DOCS_WARNINGS_AS_ERRORS` (OFF), `CPPLAB_ENABLE_SANITIZERS` (OFF, ASan+UBSan),
`CPPLAB_WARNINGS_AS_ERRORS` (OFF), `ENABLE_COVERAGE` (OFF).

Tests: `-L unit` are the GoogleTest tests in `tests/`, `-L example` are auto-generated smoke tests
that run every non-interactive example once (`example:<id>`). Both must stay green, also in a
sanitizer build.

## Architecture: the lab framework

`include/lab/` + `src/lab/` are a small framework; everything else is examples.

- `LAB_EXAMPLE("Name", "description" [, lab::kInteractive]) { ... }` (lab/Example.h) declares a
  file-local function and registers it in `lab::Registry` from a static initializer.
- The **menu group comes from the file path**: `src/core/smart_pointer/Weak.cpp` becomes the id
  `core/smart_pointer/Weak`. `CPPLAB_SOURCE_DIR` (a compile definition) turns `__FILE__` into a
  repository-relative path.
- `lab::Registry` keeps examples sorted, rejects duplicate ids and invalid names; `main.cpp`
  reports registry errors and exits non-zero.
- `lab::runMenu` (Menu.cpp) builds a folder tree from the ids: numbers navigate, `0` goes back,
  text searches, `q` quits. `lab::runExample` / `lab::runAll` (Runner.cpp) print the banner, catch
  exceptions and restore `std::cout` formatting.
- Logging: `LOG("text")`, `LOG_S("x = " << x)`, `LOG_FUNC()` (current function signature),
  `LOG_SECTION("Title")`. Debug builds prefix `[time][file:line][function]`; Release and `--plain`
  print the bare message.
- `lab/version.h` is generated from `include/lab/version.h.in`.

## Drafts

`LAB_EXAMPLE(name, description, lab::kDraft)` marks a scaffold: the header comment lists what the
example should teach, `outline()` prints that list, and the menu shows `[draft]`. They still build
and run as ctest smoke tests. `docs/cpp-standards-coverage.md` is the audit of C++11 - C++23
against the lab and tracks every open draft; update it (and the folder README) when a draft is
finished and `lab::kDraft` is removed.

## Adding an example

```bash
./scripts/new_example.sh core/utils Span "std::span: a view over contiguous memory"
```

No CMake change is needed: `cpplab_add_example_module()` globs each module folder with
`CONFIGURE_DEPENDS`, and the ctest smoke test is discovered from `--list-ids` at test time. A new
top-level module needs one line in `src/CMakeLists.txt`. Full conventions:
`docs/adding-examples.md`.

House rules for example code (see also `docs/adding-examples.md`):

- Put helpers in an anonymous namespace; all examples link into one binary, so global names would
  clash (ODR).
- Header comment: what it teaches, key points/pitfalls, a cppreference link.
- Split into small functions, each starting with `LOG_SECTION`.
- Never execute undefined behavior; describe it instead. The smoke tests run under ASan/UBSan.
- Write files only under `std::filesystem::temp_directory_path()` and delete them; restore global
  state (e.g. `std::set_terminate`).
- Keep examples fast and non-interactive; mark the ones that need a user or a peer with
  `lab::kInteractive`.

## Static analysis and formatting

```bash
cppcheck --enable=warning,style,performance,portability --inconclusive --inline-suppr --quiet \
  --error-exitcode=1 -I include --suppressions-list=.cppcheck-suppressions ./src ./include
git ls-files '*.cpp' '*.h' | xargs clang-format -i
clang-tidy -p build $(git ls-files 'src/*.cpp')     # configured by .clang-tidy, not enforced in CI
```

- `-I include` is required, otherwise cppcheck cannot expand `LAB_EXAMPLE` and reports syntax
  errors. Intentional findings (teaching demos) are listed in `.cppcheck-suppressions`; note that a
  line containing only `#` breaks that file.
- `.clang-format` is Google-based with `Standard: c++20`. Do not set it back to `c++11`: the
  formatter then mangles digit separators such as `1'000'000` and splits `operator<=>`.
- `.clang-tidy` has `WarningsAsErrors: "*"`, and its `Checks:` value is a YAML folded scalar, so
  it cannot contain comments - a `#` inside the list silently becomes part of a check name.
  Explanations therefore live in the comment block above it.
- Naming (`.clang-tidy`): `lower_case` variables and namespaces, `CamelCase` types, trailing `_` on
  private members, `kName` for constants.

## API documentation

`cmake --build build --target docs` runs Doxygen (`cmake/Docs.cmake` fills `docs/Doxyfile.in` into
`build/Doxyfile`) and writes `build/docs/html/`. The target exists only when doxygen is installed;
`CPPLAB_DOCS_WARNINGS_AS_ERRORS=ON` (used in CI) turns Doxygen warnings into failures, so keep
`build/docs/doxygen-warnings.log` empty. Doxygen parses the Markdown too: a fenced code block needs
a blank line before it, and a bare `<header>` in prose is read as an HTML tag - write `` `<random>` ``.
Public headers in `include/lab/` are documented with `///`; examples keep plain `//` comments and
are read through the source browser.

## CI

- `.github/workflows/cpp-build-test-coverage.yml` runs on push/PR to `master` inside
  `urboob21/cpp-lab:latest`: cppcheck, build with coverage, `ctest`, lcov summary, a Release job
  with `CPPLAB_WARNINGS_AS_ERRORS=ON`, and a sanitizer job (`CPPLAB_ENABLE_SANITIZERS=ON`).
- `.github/workflows/docs.yml` builds the Doxygen site (warnings are errors) and deploys it to
  GitHub Pages from `master`. `scripts/publish_wiki.sh` refreshes the wiki Home page that links to
  it; see `docs/doxygen.md`.

## Other programs

- `src/ap/` GTK4 apps (`ap`, `mvc_ap`, `mvvm_ap`), built only when gtkmm-4.0 is found.
- `src/demo/dlopen/` host + plugin; the plugin path is compiled in via `SAMPLE_APP_PATH`, and
  `bridge` must stay a SHARED library (see its README).
- `src/embedded/` bare-metal ARM firmware: `cd src/embedded && ./run.sh [gui|debug]`
  (needs `gcc-arm-none-eabi` and `qemu-system-arm`).

## Docs

`docs/README.md` indexes the per-folder READMEs (`src/**/README.md`), which explain each topic and
embed the draw.io UML diagrams in `docs/uml/`. When adding or renaming an example, update the
README of its folder. `docs/doxygen.md` covers the generated API site.
