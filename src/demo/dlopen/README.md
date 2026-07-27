# dlopen demo

Minimal demo of the **host process + `dlopen` plugin + shared-lib bridge** pattern

## Layout

```text
dlopen_plugin_demo/
├── CMakeLists.txt
├── bridge.h/.cpp    # shared bridge API + singleton
├── sample_app       # SHARED lib with main()
├── main.cpp         # executable that dlopen + dlsym(main)
└── README.md
```

---
**Expected flow:**
1. `demo_dlopen` loads `libsample_app.so`
2. finds symbol `main` via `dlsym`
3. background thread marks bridge ready
4. plugin waits, then write/read a fake register through `libbridge.so`
5. both sides share **one** bridge singleton (because bridge is **SHARED**)

```text
demo:     host  --dlopen-->  libsample_app.so  --calls-->  libbridge.so
```

## Why bridge must be SHARED

If `bridge` were STATIC, `host` and `libsample_app.so` would each get a private singleton -> `bridge_wait_ready()` would hang forever (plugin never sees host’s `ready` flag).

Try the broken variant (optional experiment):

```cmake
# in CMakeLists.txt, change:
add_library(bridge STATIC bridge.cpp)
```

Rebuild & run => plugin will block on `bridge_wait_ready()`.

----
EOF
----