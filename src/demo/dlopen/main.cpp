#include "bridge.h"

#include <dlfcn.h>

#include <chrono>
#include <iostream>
#include <thread>

using app_main_t = int (*)(int, char**);

int main(int argc, char* argv[]) {
  const char* plugin_path = "./libsample_app.so";
  if (argc > 1) {
    plugin_path = argv[1];
  }

  std::cout << "[host] starting (bridge=" << bridge_name() << ")\n";
  std::cout << "[host] dlopen: " << plugin_path << "\n";

  void* handle = dlopen(plugin_path, RTLD_LAZY | RTLD_GLOBAL);
  if (!handle) {
    std::cerr << "[host] dlopen failed: " << dlerror() << "\n";
    return 1;
  }

  dlerror();  // clear
  auto* app_main = reinterpret_cast<app_main_t>(dlsym(handle, "main"));
  const char* err = dlerror();
  if (err != nullptr || app_main == nullptr) {
    std::cerr << "[host] dlsym(main) failed: " << (err ? err : "null") << "\n";
    dlclose(handle);
    return 1;
  }

  // init on another thread, then notify plugin
  std::thread init_thread([] {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    bridge_write_reg(0x0000, 0x1U);  // fake "device present"
    bridge_set_ready();
  });

  char arg0[] = "sample_app";
  char arg1[] = "hello";
  char* app_argv[] = {arg0, arg1, nullptr};
  const int app_argc = 2;

  std::cout << "[host] calling plugin main()\n";
  const int rc = app_main(app_argc, app_argv);
  std::cout << "[host] plugin returned " << rc << "\n";

  init_thread.join();
  dlclose(handle);
  std::cout << "[host] done\n";
  return rc;
}