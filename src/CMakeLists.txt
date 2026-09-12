# ----------------------------------------------------------------------------------------
# lab framework (registry, logger, menu, command line)
# ----------------------------------------------------------------------------------------
add_subdirectory(lab)

# ----------------------------------------------------------------------------------------
# Example modules
# Every .cpp below these folders is compiled into the menu application. To add an example,
# create a file with LAB_EXAMPLE(...) in the right folder - no CMake change is required.
# To add a new top-level topic, create the folder and add one line here.
# ----------------------------------------------------------------------------------------
set(CPPLAB_EXAMPLE_TARGETS)
cpplab_add_example_module(core)        # C++ language and standard library
cpplab_add_example_module(controller)  # control algorithms (PID)
cpplab_add_example_module(dp)          # design patterns
cpplab_add_example_module(socket)      # POSIX TCP sockets

# ----------------------------------------------------------------------------------------
# Main application target
# ----------------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        cpplab::lab
        ${CPPLAB_EXAMPLE_TARGETS}
)

if(CPPLAB_BUILD_TESTS)
    cpplab_discover_example_tests(${PROJECT_NAME})
endif()

# ----------------------------------------------------------------------------------------
# Standalone programs
# ----------------------------------------------------------------------------------------
if(CPPLAB_GUI_ENABLED)
    add_subdirectory(ap)     # MVC / MVVM with GTK4 (separate executables)
endif()

if(CPPLAB_BUILD_DEMOS)
    add_subdirectory(demo)   # dlopen plugin demo
endif()

# src/embedded is a bare-metal ARM example built with its own script (see its README).
