# ----------------------------------------------------------------------------------------
# Helpers to build the example modules
# ----------------------------------------------------------------------------------------

# cpplab_add_example_module(<directory>)
#
# Compiles every .cpp file below <directory> (relative to the current CMakeLists.txt) into
# an OBJECT library and appends the target to CPPLAB_EXAMPLE_TARGETS in the caller's scope.
#
# There is no list of files to maintain: a new example is picked up automatically the next
# time you build (CONFIGURE_DEPENDS re-runs the glob when files are added or removed).
#
# OBJECT libraries are used on purpose: examples are only referenced through static
# registration objects, and a STATIC library would let the linker drop those objects.
function(cpplab_add_example_module directory)
    file(GLOB_RECURSE sources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/${directory}/*.cpp"
    )
    if(NOT sources)
        message(WARNING "Example module '${directory}' contains no .cpp files")
        return()
    endif()

    string(MAKE_C_IDENTIFIER "cpplab_examples_${directory}" target)
    add_library(${target} OBJECT ${sources})
    target_link_libraries(${target} PUBLIC cpplab::lab)

    set(CPPLAB_EXAMPLE_TARGETS ${CPPLAB_EXAMPLE_TARGETS} ${target} PARENT_SCOPE)
endfunction()

# cpplab_discover_example_tests(<executable>)
#
# Registers one ctest test per non-interactive example ("example:<id>"). The list is
# queried from the executable (--list-ids) each time ctest runs, like
# gtest_discover_tests(), so new examples need no CMake changes.
#
#   ctest --test-dir build -L example            # all example smoke tests
#   ctest --test-dir build -R example:dp/        # only the design patterns
function(cpplab_discover_example_tests executable)
    set(include_file "${CMAKE_CURRENT_BINARY_DIR}/${executable}_example_tests.cmake")
    file(GENERATE
        OUTPUT "${include_file}"
        CONTENT "set(CPPLAB_EXECUTABLE \"$<TARGET_FILE:${executable}>\")
set(CPPLAB_TEST_WORKING_DIRECTORY \"${CMAKE_BINARY_DIR}\")
include(\"${PROJECT_SOURCE_DIR}/cmake/DiscoverExampleTests.cmake\")
"
    )
    set_property(DIRECTORY APPEND PROPERTY TEST_INCLUDE_FILES "${include_file}")
endfunction()
