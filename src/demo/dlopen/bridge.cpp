#include "bridge.h"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <unordered_map>

namespace {

struct BridgeState {
  std::unordered_map<std::uintptr_t, std::uint32_t> regs;
  std::mutex mtx;
  std::condition_variable cv;
  bool ready = false;
};

BridgeState& state() {
  /// skelton
  static BridgeState s;
  return s;
}

}  // namespace

void bridge_write_reg(std::uintptr_t addr, std::uint32_t value) {
  auto& s = state();
  std::lock_guard<std::mutex> lock(s.mtx);
  s.regs[addr] = value;
  std::cout << "[bridge] write 0x" << std::hex << addr << " = 0x" << value
            << std::dec << "\n";
}

std::uint32_t bridge_read_reg(std::uintptr_t addr) {
  auto& s = state();
  std::lock_guard<std::mutex> lock(s.mtx);
  auto it = s.regs.find(addr);
  const std::uint32_t value = (it == s.regs.end()) ? 0U : it->second;
  std::cout << "[bridge] read  0x" << std::hex << addr << " -> 0x" << value
            << std::dec << "\n";
  return value;
}

void bridge_set_ready() {
  auto& s = state();
  {
    std::lock_guard<std::mutex> lock(s.mtx);
    s.ready = true;
  }
  s.cv.notify_all();
  std::cout << "[bridge] host marked ready\n";
}

void bridge_wait_ready() {
  auto& s = state();
  std::unique_lock<std::mutex> lock(s.mtx);
  s.cv.wait(lock, [&] { return s.ready; });
  std::cout << "[bridge] plugin saw ready\n";
}

const char* bridge_name() {
  return "libbridge.so";
}