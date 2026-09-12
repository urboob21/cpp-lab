# ----------------------------------------------------------------------------------------
# cpplab::options - compile/link settings shared by every target of the project.
# Link it instead of using add_compile_options() so third-party code (GoogleTest) is not
# affected by our warnings, coverage or sanitizer flags.
# ----------------------------------------------------------------------------------------

add_library(cpplab_options INTERFACE)
add_library(cpplab::options ALIAS cpplab_options)

target_compile_features(cpplab_options INTERFACE cxx_std_20)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Compiler warnings (useful for learning/debugging)
    target_compile_options(cpplab_options INTERFACE -Wall -Wextra -Wpedantic)

    if(CPPLAB_WARNINGS_AS_ERRORS)
        target_compile_options(cpplab_options INTERFACE -Werror)
    endif()

    # Code coverage configuration (see scripts/gen_coverage_*.sh)
    if(ENABLE_COVERAGE)
        message(STATUS "Enabling coverage flags")
        # -fprofile-update=atomic: the RaceCondition example increments a counter from
        # several threads on purpose, which would also corrupt gcov's own counters.
        target_compile_options(cpplab_options INTERFACE --coverage -O0 -g -fprofile-update=atomic)
        target_link_options(cpplab_options INTERFACE --coverage)
    endif()

    # Sanitizers catch memory errors and undefined behavior at run time:
    #   cmake -S . -B build-asan -DCPPLAB_ENABLE_SANITIZERS=ON
    #   cmake --build build-asan && ./build-asan/bin/cpp_lab_project --run-all
    if(CPPLAB_ENABLE_SANITIZERS)
        message(STATUS "Enabling AddressSanitizer + UndefinedBehaviorSanitizer")
        target_compile_options(cpplab_options INTERFACE
            -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=undefined)
        target_link_options(cpplab_options INTERFACE -fsanitize=address,undefined)
    endif()
endif()
