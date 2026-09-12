# ----------------------------------------------------------------------------------------
# Dependencies - Threads (std::thread on POSIX)
# ----------------------------------------------------------------------------------------
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# ----------------------------------------------------------------------------------------
# Dependencies - GoogleTest (only needed for the unit tests)
# ----------------------------------------------------------------------------------------
if(CPPLAB_BUILD_TESTS)
    # FetchContent allows downloading dependencies at configure time
    include(FetchContent)

    if(POLICY CMP0135)
        cmake_policy(SET CMP0135 NEW)  # use the extraction time as file timestamps
    endif()

    # Declare GoogleTest dependency (pinned commit for reproducible builds)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

    # Make GoogleTest available
    FetchContent_MakeAvailable(googletest)
endif()

# ----------------------------------------------------------------------------------------
# Dependencies - GTK4 / gtkmm-4.0 (optional, only for the apps in src/ap)
# ----------------------------------------------------------------------------------------
set(CPPLAB_GUI_ENABLED OFF)

if(NOT CPPLAB_BUILD_GUI STREQUAL "OFF")
    # Use the package PkgConfig to detect GTK+ headers/library files
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        # Check for gtkmm-4.0 specifically, not just gtk4
        pkg_check_modules(GTKMM QUIET IMPORTED_TARGET gtkmm-4.0)
    endif()

    if(GTKMM_FOUND)
        set(CPPLAB_GUI_ENABLED ON)
        message(STATUS "gtkmm-4.0 ${GTKMM_VERSION} found: building the GUI apps in src/ap")
    elseif(CPPLAB_BUILD_GUI STREQUAL "ON")
        message(FATAL_ERROR "CPPLAB_BUILD_GUI=ON but gtkmm-4.0 was not found. "
                            "Install it with: sudo apt install libgtkmm-4.0-dev")
    else()
        message(STATUS "gtkmm-4.0 not found: skipping the GUI apps in src/ap")
    endif()
endif()
