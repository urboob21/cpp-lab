# ----------------------------------------------------------------------------------------
# Main application target
# ----------------------------------------------------------------------------------------

# Add project submodules
add_subdirectory(core)
add_subdirectory(controller)
add_subdirectory(dp)
add_subdirectory(socket)
add_subdirectory(ap)
add_subdirectory(demo)

# Header files directory
set(APP_HEADERS
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/generated # cmake generated headers
)

# Define the main application executable
add_executable(${PROJECT_NAME}
    main.cpp
    ${CORE_SOURCES}
    ${SOCKET_SOURCES}
    ${CONTROLLER_SOURCES}
    ${DP_SOURCES}
)

# Add header include paths
target_include_directories(${PROJECT_NAME}
    PRIVATE ${APP_HEADERS}
)

# Optional: add project-specific compiler options
target_compile_options(${PROJECT_NAME}
    PRIVATE -Wall -Wextra -Wpedantic
)