# ----------------------------------------------------------------------------------------
# lab: the small framework behind the examples
#   - Registry     : examples register themselves with LAB_EXAMPLE (lab/Example.h)
#   - Logger       : LOG / LOG_S / LOG_FUNC / LOG_SECTION (lab/Logger.h)
#   - Menu, Runner : interactive menu and command-line runner used by main.cpp
# ----------------------------------------------------------------------------------------

string(TIMESTAMP CPPLAB_BUILD_TIME "%Y-%m-%d %H:%M:%S")
configure_file(
    ${PROJECT_SOURCE_DIR}/include/lab/version.h.in
    ${PROJECT_BINARY_DIR}/generated/lab/version.h
    @ONLY
)

add_library(cpplab_lab STATIC
    CommandLine.cpp
    Logger.cpp
    Menu.cpp
    Registry.cpp
    Runner.cpp
)
add_library(cpplab::lab ALIAS cpplab_lab)

target_include_directories(cpplab_lab
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}/generated
)

target_link_libraries(cpplab_lab
    PUBLIC
        cpplab::options
        Threads::Threads
)

# Lets the registry turn __FILE__ into a repository-relative path (menu groups).
target_compile_definitions(cpplab_lab PRIVATE CPPLAB_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
