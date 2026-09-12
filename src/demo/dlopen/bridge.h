#pragma once

#include <cstdint>

// Shared-lib bridge: one copy in the process for both host and plugin.
void bridge_write_reg(std::uintptr_t addr, std::uint32_t value);
std::uint32_t bridge_read_reg(std::uintptr_t addr);

void bridge_set_ready();
void bridge_wait_ready();

const char* bridge_name();