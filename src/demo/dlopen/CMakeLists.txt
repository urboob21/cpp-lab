# bridge: SHARED library used by both the host and the plugin, so the whole process shares
# one bridge singleton (see README.md for what breaks when it is STATIC).
add_library(bridge SHARED bridge.cpp)
target_include_directories(bridge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bridge PRIVATE cpplab::options Threads::Threads)

# sample_app: the "plugin", a shared library that is only loaded at run time with dlopen().
add_library(sample_app SHARED sample_app.cpp)
target_link_libraries(sample_app PRIVATE bridge cpplab::options)

# demo_dlopen: the host executable. It links the bridge but NOT the plugin. The plugin path
# is compiled in, so the demo works from any working directory.
add_executable(demo_dlopen main.cpp)
target_link_libraries(demo_dlopen PRIVATE bridge cpplab::options Threads::Threads ${CMAKE_DL_LIBS})
target_compile_definitions(demo_dlopen PRIVATE SAMPLE_APP_PATH="$<TARGET_FILE:sample_app>")
add_dependencies(demo_dlopen sample_app)

if(CPPLAB_BUILD_TESTS)
    add_test(NAME demo:dlopen COMMAND demo_dlopen)
    set_tests_properties(demo:dlopen PROPERTIES LABELS "demo" TIMEOUT 30)
endif()
