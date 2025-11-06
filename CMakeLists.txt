cmake_minimum_required(VERSION 3.14)

# ----------------------------------------------------------------------------------------
# Project metadata
# ----------------------------------------------------------------------------------------
project(cpp_lab_project # ${PROJECT_NAME}
    VERSION 1.0.0
    DESCRIPTION "The project with GoogleTest and CMake"
    LANGUAGES CXX
)

set(PROJECT_NAME_TEST ${PROJECT_NAME}_unit_test) # name for the unit-test executable

# ----------------------------------------------------------------------------------------
# Compiler and language configuration
# ----------------------------------------------------------------------------------------
# Require C++17 for GoogleTest and modern C++ features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optionally enforce warnings (good for learning/debugging)
add_compile_options(-Wall -Wextra -Wpedantic)

# ----------------------------------------------------------------------------------------
# Dependencies - GoogleTest
# ----------------------------------------------------------------------------------------
# FetchContent allows downloading dependencies at configure time
include(FetchContent)

# Declare GoogleTest dependency
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# Make GoogleTest available
FetchContent_MakeAvailable(googletest)

# Enable CTest framework to run tests with `ctest`
enable_testing()

# ----------------------------------------------------------------------------------------
# Project directories and files
# ----------------------------------------------------------------------------------------
# Header files directory
set(APP_HEADERS 
    "include"
)

# Core source files
set(APP_SOURCES
    "src/DeleteMe.cpp"
)

# Test files
set(APP_TESTS
    "tests/DeleteMeTest.cpp"
)

# ----------------------------------------------------------------------------------------
# Main application target
# ----------------------------------------------------------------------------------------
add_executable(${PROJECT_NAME}
    ${APP_SOURCES}
    "src/main.cpp"
)

# Add header include path
target_include_directories(${PROJECT_NAME}
    PRIVATE ${APP_HEADERS}
)

# Optional: add project-specific compiler options
target_compile_options(${PROJECT_NAME}
    PRIVATE -Wall -Wextra -Wpedantic
)

# ----------------------------------------------------------------------------------------
# Unit testing target
# ----------------------------------------------------------------------------------------
add_executable(${PROJECT_NAME_TEST}
    ${APP_SOURCES}
    ${APP_TESTS}
)

target_include_directories(${PROJECT_NAME_TEST}
    PRIVATE ${APP_HEADERS} # PRIVATE for internal use within a target,
)

# Link GoogleTest to the test executable
target_link_libraries(${PROJECT_NAME_TEST}
    GTest::gtest_main
)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME_TEST})