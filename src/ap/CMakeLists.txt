# ----------------------------------------------------------------------------------------
# GTK4 (gtkmm-4.0) apps that compare three ways to structure the same small UI:
#   ap       - everything in one window class (tightly coupled)
#   mvc_ap   - Model / View / Controller
#   mvvm_ap  - Model / View / ViewModel
# Only built when gtkmm-4.0 is available (see CPPLAB_BUILD_GUI in the top-level CMakeLists).
# ----------------------------------------------------------------------------------------

# Interface target with the gtkmm include paths, libraries and compile flags.
# PkgConfig::GTKMM is an imported target, so its headers are treated as system headers and
# do not trigger our -Wall -Wextra -Wpedantic warnings.
add_library(gtk4-settings INTERFACE)
target_link_libraries(gtk4-settings INTERFACE PkgConfig::GTKMM cpplab::options)

# ap executable
add_executable(ap simple_ap.cpp)
target_link_libraries(ap PRIVATE gtk4-settings)

add_subdirectory(mvc)
add_subdirectory(mvvm)
