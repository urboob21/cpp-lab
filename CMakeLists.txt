cmake_minimum_required(VERSION 3.14)

# ----------------------------------------------------------------------------------------
# Project metadata
# ----------------------------------------------------------------------------------------
project(cpp_lab_project # ${PROJECT_NAME}
    VERSION 1.0.0
    DESCRIPTION "A C/C++ project uses CMake, GoogleTest, gcc, g++, cppcheck, and lcov, integrated with Docker and GitHub Actions for CI/CD."
    LANGUAGES CXX
)

# Build timestamp
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S")

configure_file(
    ${CMAKE_SOURCE_DIR}/include/version.h.in
    ${CMAKE_BINARY_DIR}/generated/version.h
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

message("C Compiler: ${CMAKE_C_COMPILER}")
message("C++ Compiler: ${CMAKE_CXX_COMPILER}")
message("C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message("C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

# Option to enable coverage
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Enabling coverage flags")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()


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
    "${CMAKE_BINARY_DIR}/generated"   # cmake generated headers
)

# Core source files
set(APP_SOURCES
    "src/DeleteMe.cpp"
    "src/core/basics/InitializeVariable.cpp"
    "src/core/basics/Operations.cpp"
    "src/core/basics/ControlFlow.cpp"
    "src/core/datatypes/Fundamental.cpp"
    "src/core/datatypes/CArray.cpp"
    "src/core/datatypes/CReferences.cpp"
    "src/core/datatypes/CPointers.cpp"
    "src/core/datatypes/CEnum.cpp"
    "src/core/datatypes/CStruct.cpp"
    "src/core/datatypes/CUnion.cpp"
    "src/core/datatypes/TypeConVersions.cpp"
    "src/core/datatypes/class/CConstructors.cpp"
    "src/core/datatypes/class/CDestructors.cpp"
    "src/core/datatypes/class/SallowDeepCopying.cpp"
    "src/core/datatypes/class/RoleOfThreeFiveZero.cpp"
    "src/patterns/structural/Adapter.cpp"
    "src/patterns/structural/Bridge.cpp"
    "src/patterns/structural/Proxy.cpp"
    "src/patterns/structural/Composite.cpp"
    "src/patterns/structural/Flyweight.cpp"
    "src/patterns/structural/Facade.cpp"
    "src/patterns/structural/Decorator.cpp"
    "src/patterns/behavioral/ChainOfCommand.cpp"
    "src/patterns/behavioral/Command.cpp"
    "src/patterns/behavioral/Iterator.cpp"
    "src/patterns/behavioral/Mediator.cpp"
    "src/patterns/behavioral/Memento.cpp"
    "src/patterns/behavioral/Visitor.cpp"
    "src/patterns/behavioral/TemplateMethod.cpp"
    "src/patterns/behavioral/Strategy.cpp"
    "src/patterns/behavioral/State.cpp"
    "src/patterns/behavioral/Observer.cpp"
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