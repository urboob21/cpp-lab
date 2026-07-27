#include "bridge.h"

#include <cstdint>
#include <iostream>

extern "C" int main(const int argc, const char* argv[]) {
  std::cout << "[plugin] main() started (argc=" << argc << ")\n";
  for (int i = 0; i < argc; ++i) {
    std::cout << "[plugin]   argv[" << i << "] = " << argv[i] << "\n";
  }

  std::cout << "[plugin] waiting for host via " << bridge_name() << "...\n";
  bridge_wait_ready();

  bridge_write_reg(0x1000, 0xA5A5A5A5U);
  const std::uint32_t v = bridge_read_reg(0x1000);
  std::cout << "[plugin] round-trip value = 0x" << std::hex << v << std::dec
            << "\n";

  std::cout << "[plugin] main() done\n";
  return 0;
}