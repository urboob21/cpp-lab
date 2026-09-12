// Host program of the dlopen demo (see README.md in this folder).
//
//   1. dlopen()  loads libsample_app.so at run time
//   2. dlsym()   looks up the symbol "main" inside it
//   3. a background thread initializes the shared bridge and marks it ready
//   4. the plugin's main() runs and talks to the host through libbridge.so

#include <dlfcn.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "bridge.h"

// CMake passes the absolute path of the plugin; the fallback expects it in the
// current working directory.
#ifndef SAMPLE_APP_PATH
#define SAMPLE_APP_PATH "./libsample_app.so"
#endif

namespace {

using AppMain = int (*)(int, char**);

/// Closes a dlopen() handle automatically (RAII for a C resource).
struct LibraryCloser {
  void operator()(void* handle) const {
    if (handle != nullptr) {
      dlclose(handle);
    }
  }
};

}  // namespace

int main(int argc, char* argv[]) {
  const char* plugin_path = argc > 1 ? argv[1] : SAMPLE_APP_PATH;

  std::cout << "[host] starting (bridge=" << bridge_name() << ")\n";
  std::cout << "[host] dlopen: " << plugin_path << "\n";

  // RTLD_LAZY  : resolve function symbols on first use
  // RTLD_GLOBAL: make the plugin's symbols available to libraries loaded later
  const std::unique_ptr<void, LibraryCloser> library(
      dlopen(plugin_path, RTLD_LAZY | RTLD_GLOBAL));
  if (!library) {
    std::cerr << "[host] dlopen failed: " << dlerror() << "\n";
    return 1;
  }

  dlerror();  // clear any old error: dlsym may legitimately return nullptr
  // dlsym returns void*; converting it to a function pointer is allowed on POSIX.
  const auto app_main = reinterpret_cast<AppMain>(dlsym(library.get(), "main"));
  if (const char* error = dlerror(); error != nullptr || app_main == nullptr) {
    std::cerr << "[host] dlsym(main) failed: "
              << (error != nullptr ? error : "null") << "\n";
    return 1;
  }

  // Initialize on another thread, then notify the plugin through the bridge.
  std::thread init_thread([] {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    bridge_write_reg(0x0000, 0x1U);  // fake "device present"
    bridge_set_ready();
  });

  char arg0[] = "sample_app";
  char arg1[] = "hello";
  char* app_argv[] = {arg0, arg1, nullptr};
  constexpr int kAppArgc = 2;

  std::cout << "[host] calling plugin main()\n";
  const int rc = app_main(kAppArgc, app_argv);
  std::cout << "[host] plugin returned " << rc << "\n";

  init_thread.join();
  std::cout << "[host] done\n";
  return rc;
}  // `library` is closed here, after the plugin has finished
