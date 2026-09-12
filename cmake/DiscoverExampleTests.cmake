# ----------------------------------------------------------------------------------------
# Executed by ctest (not by cmake): asks the lab executable for the ids of all
# non-interactive examples and registers one smoke test per example.
#
# Expected variables (set by cpplab_discover_example_tests()):
#   CPPLAB_EXECUTABLE              path of cpp_lab_project
#   CPPLAB_TEST_WORKING_DIRECTORY  working directory of the tests
# ----------------------------------------------------------------------------------------

if(EXISTS "${CPPLAB_EXECUTABLE}")
    execute_process(
        COMMAND "${CPPLAB_EXECUTABLE}" --list-ids
        OUTPUT_VARIABLE example_ids
        ERROR_VARIABLE discovery_error
        RESULT_VARIABLE discovery_result
    )
else()
    set(discovery_result "executable not found (build the project first)")
endif()

if(NOT discovery_result EQUAL 0)
    # Surface the problem as a failing test instead of silently running nothing.
    add_test(example:discovery "${CPPLAB_EXECUTABLE}" --list-ids)
    set_tests_properties(example:discovery PROPERTIES LABELS "example")
    message(WARNING "Example discovery failed: ${discovery_result} ${discovery_error}")
else()
    string(REPLACE "\n" ";" example_ids "${example_ids}")
    foreach(example_id IN LISTS example_ids)
        if(example_id)
            add_test("example:${example_id}" "${CPPLAB_EXECUTABLE}" --plain --run "${example_id}")
            set_tests_properties("example:${example_id}" PROPERTIES
                LABELS "example"
                TIMEOUT 60
                WORKING_DIRECTORY "${CPPLAB_TEST_WORKING_DIRECTORY}"
            )
        endif()
    endforeach()
endif()
