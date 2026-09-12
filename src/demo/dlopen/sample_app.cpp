// The "plugin": a shared library that contains its own main().
//
// It is never started as a process. The host loads it with dlopen() and calls
// main() through a function pointer. The C++ compiler never mangles the name
// `main`, so dlsym(handle, "main") can find it.

#include <cstdint>
#include <iostream>

#include "bridge.h"

int main(int argc, char* argv[]) {
  std::cout << "[plugin] main() started (argc=" << argc << ")\n";
  for (int i = 0; i < argc; ++i) {
    std::cout << "[plugin]   argv[" << i << "] = " << argv[i] << "\n";
  }

  std::cout << "[plugin] waiting for host via " << bridge_name() << "...\n";
  bridge_wait_ready();  // blocks until the host thread calls bridge_set_ready()

  bridge_write_reg(0x1000, 0xA5A5A5A5U);
  const std::uint32_t value = bridge_read_reg(0x1000);
  std::cout << "[plugin] round-trip value = 0x" << std::hex << value << std::dec
            << "\n";

  std::cout << "[plugin] main() done\n";
  return 0;
}
