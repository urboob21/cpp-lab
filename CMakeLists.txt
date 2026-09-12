cmake_minimum_required(VERSION 3.16)

# Project metadata
project(cpp_lab_project # ${PROJECT_NAME}
    VERSION 1.0.0
    DESCRIPTION "A C/C++ learning lab built with CMake, GoogleTest, gcc/g++, cppcheck and lcov, integrated with Docker and GitHub Actions for CI/CD."
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# ----------------------------------------------------------------------------------------
# Options (pass with -D<OPTION>=<VALUE>)
# ----------------------------------------------------------------------------------------
option(CPPLAB_BUILD_TESTS "Build the unit tests and register one smoke test per example" ON)
option(CPPLAB_BUILD_DEMOS "Build the standalone demos in src/demo" ON)
set(CPPLAB_BUILD_GUI "AUTO" CACHE STRING "Build the GTK4 apps in src/ap: AUTO (if gtkmm-4.0 is found), ON or OFF")
set_property(CACHE CPPLAB_BUILD_GUI PROPERTY STRINGS AUTO ON OFF)
option(ENABLE_COVERAGE "Enable coverage reporting (gcc/clang)" OFF)
option(CPPLAB_ENABLE_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(CPPLAB_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

# ----------------------------------------------------------------------------------------
# Global build settings
# ----------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json (useful for clang-tidy, clangd, IDEs)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to a Debug build: debug info, assertions and log prefixes enabled
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Output directories: executables in build/bin, libraries in build/lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER} (${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION})")
message(STATUS "Build type  : ${CMAKE_BUILD_TYPE}")

include(CompilerOptions)  # cpplab::options - warnings, coverage, sanitizers
include(Dependencies)     # Threads, GoogleTest, gtkmm-4.0
include(CppLab)           # cpplab_add_example_module(), cpplab_discover_example_tests()
include(Docs)             # 'docs' target - Doxygen API documentation (optional)

# Enable CTest (unit tests + example smoke tests)
enable_testing()

# ----------------------------------------------------------------------------------------
# Project modules
# ----------------------------------------------------------------------------------------
add_subdirectory(src)

if(CPPLAB_BUILD_TESTS)
    add_subdirectory(tests)
endif()
