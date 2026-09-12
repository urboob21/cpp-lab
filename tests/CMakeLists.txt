# ----------------------------------------------------------------------------------------
# Unit tests (GoogleTest + GoogleMock)
#
# Every *Test.cpp below this folder is compiled automatically - add a file, rebuild.
#
#   ctest --test-dir build -L unit                                    # all unit tests
#   ./build/bin/cpp_lab_project_unit_test --gtest_filter='Registry*'   # a subset
#
# The smoke tests that run every example are registered in src/CMakeLists.txt (label
# "example").
# ----------------------------------------------------------------------------------------

include(GoogleTest)

file(GLOB_RECURSE unit_test_sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*Test.cpp)

add_executable(${PROJECT_NAME}_unit_test
    ${unit_test_sources}
    # Code under test that is not part of the lab library
    ${PROJECT_SOURCE_DIR}/src/controller/pid/pid.cpp
)

target_include_directories(${PROJECT_NAME}_unit_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/src
)

# gmock_main provides main() and links GoogleTest as well
target_link_libraries(${PROJECT_NAME}_unit_test
    PRIVATE
        cpplab::lab
        GTest::gmock_main
)

# Register every TEST() with CTest
gtest_discover_tests(${PROJECT_NAME}_unit_test
    DISCOVERY_TIMEOUT 30
    PROPERTIES LABELS "unit"
)
