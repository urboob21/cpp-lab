#include <stdint.h>

/// @brief Symbols defined by the linker script.
extern uint32_t _sidata;  ///< Start address of initialized data in Flash (LMA)
extern uint32_t _sdata;   ///< Start address of .data section in RAM (VMA)
extern uint32_t _edata;   ///< End address of .data section in RAM
extern uint32_t _sbss;    ///< Start address of .bss section
extern uint32_t _ebss;    ///< End address of .bss section
extern uint32_t _estack;  ///< Initial stack pointer (top of RAM)

extern int main(void);

void Reset_Handler(void);
void Default_Handler(void);

/// @brief Weak aliases allow users to override handlers bydefining their own implementation with the same name.
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

/// @brief Interrupt Vector Table
__attribute__((section(".isr_vector"))) const void* vector_table[] = {
    &_estack,  // Initial SP
    Reset_Handler,

    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,

    0,
    0,
    0,
    0,

    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
};

/// @brief Executed immediately after reset.
/// Responsible for initializing the C runtime environment
void Reset_Handler(void) {
  // Copy initialized data (.data) * from Flash to RAM.
  const uint32_t* src = &_sidata;
  uint32_t* dst = &_sdata;

  while (dst < &_edata) {
    *dst++ = *src++;
  }

  dst = &_sbss;

  // Clear uninitialized data (.bss)
  while (dst < &_ebss) {
    *dst++ = 0;
  }

  // Jump to the application entry point
  main();

  // main() should never return
  while (1) {}
}

/// @brief Default handler for unimplemented exceptions.
void Default_Handler(void) {
  while (1) {}
}