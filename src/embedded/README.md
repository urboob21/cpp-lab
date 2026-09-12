# Embedded Bare-Metal: Build, Link, Boot, and Debug
#### Env

```bash
$ sudo apt install gcc-arm-none-eabi
$ apt-get install qemu-system
```

## 1. Building Process

```bash
#Sources files      #Toolchain                 #Object files
+-----------+       +------------------+       +-----------+
| startup.c | ----> |                  | ----> | startup.o |
+-----------+       | arm-none-eabi-gcc|       +-----------+
                    |                  |
+-----------+ ----> |                  | ----> +-----------+
|  main.c   |       +------------------+       |  main.o   |
+-----------+                                  +-----------+
                                                      |
#Linker script                                        |
+------------------+                                  |
| linker_script.ld | ---------------------------------+
+------------------+                                  |
                                                      v
                                            +----------------+
                                            |arm-none-eabi-ld| #Linker
                                            +----------------+
                                                      |
                                                      v
                                            +----------------+
                                            |   blink.elf    | #Executable
                                            +----------------+
```
### 1.1. Compiler
A compiler is a software program that translates source code written in a high-level programming language into a lower-level form, such as assembly code, object code, or machine code, while preserving the program's functionality.

```bash
$ gcc # native
$ g++
$ clang
$ arm-none-eabi-gcc # cross
$ aarch64-none-elf-gcc
```

### 1.2. Cross Compiler
A cross compiler is a compiler that runs on one platform (host) but generates executable code for a different platform (target).
An ARM cross compiler can run on a Linux PC and generate binaries that execute on an ARM-based embedded system.

```bash
$ arm-none-eabi-gcc
$ aarch64-none-elf-gcc
```

### 1.3. Toolchain
A toolchain is a collection of software development tools used to build, debug, and analyze software.
A typical embedded ARM toolchain includes:

```bash
$ arm-none-eabi-gcc      # Compiler
$ arm-none-eabi-as       # Assembler
$ arm-none-eabi-ld       # Linker
$ arm-none-eabi-objcopy  # ELF to BIN converter
$ arm-none-eabi-gdb      # Debugger
```

### 1.4. Linker Script
**Linker Script** is a configuration file that tells the linker how to place the program into the mcu memory.
- Define the memory layout: Flash, RAM, Stack, Heap
- Specify available memory regions
- Place the code/data in the correct memory location
- Set the program entry point
- Optimize memory usage
  
Reference: https://sourceware.org/binutils/docs/ld/Scripts.html

#### Concept
The linker combines multiple object files into a single executable file.
Object files are stored in a special format called an `object file format` (such as ELF). Each object file contains several `sections`, and each section has a name and a size (e.g., .text, .data, .bss).
For each loadable output section, the linker defines two addresses:
- VMA (Virtual Memory Address): The address where the section will reside when the program is running.
- LMA (Load Memory Address): The address from which the section is loaded at startup.

#### Format
- Linker scripts are text files. e.g. `stm32_flash.ld`
- Whitespace is generally ignored.
- Include comments in linker scripts just as in C

#### Example
The simplest possible linker script has just one command: `SECTION` to describe the memory layout of the output file.

```ld
/*stm32_flash.ld*/
SECTIONS
{
    . = 0x10000; # the code should be loaded at address 0x10000
    .text : { *(.text)}
    . = 0x8000000; # that the data should start at address 0x8000000
    .data : { *{.data}}
    .bss : { *(.bss) }
}
```
- Symbols:
  - `.`: location counter
  - `.text`, `.data`, and `.bss` are the sections of the output file
  - `*(.text)` means all `.text` input sections in all input files.

### 1.5. Startup Code
A microcontroller does not start executing from `main()` after power-up or reset. Instead, it begins execution at the reset vector, which points to the startup code (typically `Reset_Handler`).
- Setting initial data values in SRAM, for global variables for example.
- Zero initialization of data memory for variables that are uninitialized at load time.
- Initializing the data variables controlling heap memory, for malloc() for example.
- Performing any required system initialization.
- Calling main().

---
### 2. Flashing Process

```bash

                                            +----------------+
                                            |   blink.elf    | #Executable
                                            +----------------+
                                                      |
                                                      v
                                            +----------------+
                                            |    OpenOCD     | #Tool to download/Debug
                                            +----------------+
                                                      |
                                                      v
                                            +----------------+
                                            |    ST-LINK     | #Debug probe
                                            +----------------+
                                                      |
                                                      v
                +--------------------------------------------------+
                |                   STM32 FLASH                    |
                |--------------------------------------------------|
                | .isr_vector                                      |
                | .text                                            |
                | .rodata                                          |
                | Initial .data image                              |
                +--------------------------------------------------+
                                      |
                                      | RESET
                                      v
                            +---------------------+
                            |   Reset_Handler()   |
                            +---------------------+
                                      |
                                      +----------------------+
                                      | Copy .data           |
                                      | Flash --> SRAM       |
                                      +----------------------+
                                      v
                +--------------------------------------------------+
                |                    STM32 SRAM                    |
                |--------------------------------------------------|
                | .data                                            |
                | .bss   <-- zero-filled                           |
                |                                                  |
                | Heap (optional)                                  |
                |                                                  |
                | Stack (grows downward)                           |
                +--------------------------------------------------+
                                      |
                                      v
                                 +---------+
                                 | main()  |
                                 +---------+
                                      |
                                      v
                              #Application Runs#
```

### 2.1. Build executable
Compile the source files and link them into the `firmware.elf`

```bash
$ arm-none-eabi-gcc \
    -mcpu=cortex-m3 \
    -mthumb \
    -nostdlib \
    -ffreestanding \
    startup.c main.c \
    -T stm32_flash.ld \
    -Wl,-Map=firmware.map \
    -o build/firmware.elf
```
- **Options:**
    - `-mcpu=cortex-m3` : Generate code for the ARM Cortex-M3 processor.
    - `-mthumb `: Use the Thumb instruction set.
    - `-nostdlib` : Do not link the standard C runtime libraries.
    - `-ffreestanding` : Compile for a freestanding environment (bare-metal system).
    - `-T stm32_flash.ld` : Use the specified linker script.
    - `-Wl,-Map=firmware.map` : Generate a linker map file.
    - `-o build/firmware.elf` : Output executable file.
- **ELF file** does include symbol tables, debug information, or other metadata.
### 2.2. Generate a Binary Image
Convert the ELF executable into a raw binary image that contains only the program data that will be stored in Flash memory.

```bash
$ arm-none-eabi-objcopy \
    -O binary \
    build/firmware.elf \
    build/firmware.bin
```

### 2.3. Boot and Debug with QEMU
QEMU can emulate the `STM32VLDISCOVERY` board and load firmware
#### Start QEMU

```bash
$ qemu-system-arm \
    -M stm32vldiscovery \
    -kernel firmware.bin
```

### Connect with GDB
- Launch QEMU and wait for a debugger connection:
    ```bash
    $ qemu-system-arm \
        -M stm32vldiscovery \
        -kernel firmware.elf \
        -nographic \
        -S \
        -gdb tcp::1234
    ```
  - **Options:**
      `-M stm32vldiscovery` : Emulate the STM32VLDISCOVERY board.
      `-kernel firmware.elf` : Load the firmware image.
      `-nographic` : Disable graphical output and use the terminal.
      `-S` : Pause the CPU at reset.
      `-gdb tcp::1234` : Start a GDB server on port 1234.

- Start GDB using the ELF file so that symbol and debug information are available:
    ```bash
    $ arm-none-eabi-gdb firmware.elf
    ```
- Connect to the QEMU GDB server:
    ```bash
    target remote :1234
    ```

- **Useful commands:**

```bash
monitor reset     # Reset the emulated target
load              # Load firmware into target memory
continue          # Start execution

p counter   # Inspect variables and symbols

break main # set breakpoints
continue
```

### Quit QEMU
When running QEMU with `-nographic`, use:

```text
Ctrl + A, then X

to exit the emulator.

Other useful shortcuts:
Ctrl + A, then C    Open QEMU monitor
Ctrl + A, then H    Show help
Ctrl + A, then X    Exit QEMU
```

---