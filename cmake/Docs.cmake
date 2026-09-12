# ---------------------------------------------------------------------------
# Docs.cmake - the "docs" target: API documentation generated with Doxygen.
#
#   cmake -S . -B build
#   cmake --build build --target docs
#   xdg-open build/docs/html/index.html
#
# Doxygen is optional: without it the target is simply not created and the
# rest of the build is unaffected. See docs/doxygen.md.
# ---------------------------------------------------------------------------

option(CPPLAB_BUILD_DOCS "Add the 'docs' target (needs doxygen; graphviz is optional)" ON)
option(CPPLAB_DOCS_WARNINGS_AS_ERRORS "Fail the 'docs' target on Doxygen warnings" OFF)

if(NOT CPPLAB_BUILD_DOCS)
    return()
endif()

# Doxygen::doxygen and Doxygen_dot_FOUND come from CMake's FindDoxygen module.
find_package(Doxygen OPTIONAL_COMPONENTS dot)

if(NOT DOXYGEN_FOUND)
    message(STATUS "Doxygen     : not found (no 'docs' target; install doxygen graphviz)")
    return()
endif()

# Values substituted into docs/Doxyfile.in.
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/docs")

# Quoted so that paths with spaces survive; Doxygen accepts a quoted list.
# README.md is the front page; the other markdown files (docs/ and the module
# READMEs under src/ and tests/) become "Related Pages", so the links between
# them keep working inside the generated site.
set(DOXYGEN_INPUT
    "\"${PROJECT_SOURCE_DIR}/README.md\" \\
                         \"${PROJECT_SOURCE_DIR}/docs\" \\
                         \"${PROJECT_SOURCE_DIR}/include\" \\
                         \"${PROJECT_SOURCE_DIR}/src\" \\
                         \"${PROJECT_SOURCE_DIR}/tests\"")

# tests/dummy is scratch code kept as it is written upstream.
set(DOXYGEN_EXCLUDE "\"${PROJECT_SOURCE_DIR}/tests/dummy\"")

if(Doxygen_dot_FOUND)
    set(DOXYGEN_HAVE_DOT YES)
else()
    set(DOXYGEN_HAVE_DOT NO)
endif()

if(CPPLAB_DOCS_WARNINGS_AS_ERRORS)
    set(DOXYGEN_WARN_AS_ERROR FAIL_ON_WARNINGS)
else()
    set(DOXYGEN_WARN_AS_ERROR NO)
endif()

set(cpplab_doxyfile "${CMAKE_BINARY_DIR}/Doxyfile")
configure_file("${PROJECT_SOURCE_DIR}/docs/Doxyfile.in" "${cpplab_doxyfile}" @ONLY)

# Not part of ALL: documentation is generated on demand, not on every build.
add_custom_target(docs
    COMMAND ${CMAKE_COMMAND} -E make_directory "${DOXYGEN_OUTPUT_DIRECTORY}"
    COMMAND Doxygen::doxygen "${cpplab_doxyfile}"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "Generating API documentation with Doxygen -> ${DOXYGEN_OUTPUT_DIRECTORY}/html/index.html"
    VERBATIM)

message(STATUS "Doxygen     : ${DOXYGEN_VERSION} (target 'docs', dot: ${DOXYGEN_HAVE_DOT})")
