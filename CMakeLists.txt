cmake_minimum_required(VERSION 3.14)

# Project metadata
project(cpp_lab_project # ${PROJECT_NAME}
    VERSION 1.0.0
    DESCRIPTION "A C/C++ project uses CMake, GoogleTest, gcc, g++, cppcheck, and lcov, integrated with Docker and GitHub Actions for CI/CD."
    LANGUAGES CXX
)

# Output directories to build/bin
# Executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Shared libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# Static libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler and language configuration
# Require at least C++17 for GoogleTest and modern C++ features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile_commands.json (useful for clang-tidy, clangd, IDEs)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build metadata
# Ensure directory exists for generated headers
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated)

# Generate build timestamp
string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S")

# Generate version header from template
configure_file(
    ${CMAKE_SOURCE_DIR}/include/version.h.in
    ${CMAKE_BINARY_DIR}/generated/version.h
)

# External dependencies (GoogleTest,...)
include(cmake/Dependencies.cmake)

# Compiler warnings (useful for learning/debugging)
add_compile_options(-Wall -Wextra -Wpedantic)

message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

# Code coverage configuration
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Enabling coverage flags")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

# Enable CTest framework (used by GoogleTest)
enable_testing()

# Add project modules
add_subdirectory(src)

# add_subdirectory(tests)