# dlopen plugin demo

A minimal **host process + `dlopen` plugin + shared-library bridge** setup, the pattern behind
plugin systems: the host does not know the plugin at link time and loads it at run time.

## Layout

```text
src/demo/dlopen/
├── CMakeLists.txt
├── bridge.h / bridge.cpp   # SHARED library: API + one singleton for the whole process
├── sample_app.cpp          # the "plugin": a SHARED library that contains main()
├── main.cpp                # the host: dlopen + dlsym("main")
└── README.md
```

## Run it

```bash
cmake --build build && ./build/bin/demo_dlopen
ctest --test-dir build -R demo:dlopen        # the same as a test
```

The plugin path is compiled into the host (`SAMPLE_APP_PATH`, set by CMake), so the demo works
from any working directory. Pass another path as the first argument to load a different plugin.

**Expected flow**

1. `demo_dlopen` loads `libsample_app.so` with `dlopen`
2. it looks up the symbol `main` with `dlsym` (C++ never mangles the name `main`)
3. a background thread writes a register through the bridge and marks it ready
4. the plugin waits for that signal, then writes and reads a fake register
5. both sides use **one** bridge singleton, because `bridge` is a SHARED library

```text
host  --dlopen-->  libsample_app.so  --calls-->  libbridge.so
  |                                                  ^
  +--------------------------------------------------+
                  same singleton
```

## Why the bridge must be SHARED

If `bridge` were a STATIC library, the host and the plugin would each get their own copy of the
singleton. `bridge_wait_ready()` in the plugin would then never see the host's `ready` flag and
would block forever.

Try the broken variant:

```cmake
# in CMakeLists.txt, change:
add_library(bridge STATIC bridge.cpp)
```

Rebuild and run: the plugin hangs in `bridge_wait_ready()` (stop it with Ctrl+C).

## Things worth knowing

- `dlopen` flags: `RTLD_LAZY` resolves symbols on first use (`RTLD_NOW` resolves everything at
  load time), `RTLD_GLOBAL` makes the plugin's symbols visible to libraries loaded later.
- `dlerror()` must be cleared before `dlsym`, because `dlsym` may legitimately return `nullptr`.
- The handle is closed by a `std::unique_ptr` with a custom deleter - RAII also works for C APIs.
