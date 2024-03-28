#######################################################
### Catch2 - Unit tests                             ###
#######################################################
# Find or get Catch2
# Note that CPM will try to "find_package(Catch2)" before downloading it
# See the option CPM_USE_LOCAL_PACKAGES in ROOT/CMakeLists.txt
# This is important: see One Definition Rule (ODR)
CPMAddPackage(
        NAME Catch2
        GITHUB_REPOSITORY catchorg/Catch2
        VERSION 2.12.1
        OPTIONS
        "CATCH_USE_VALGRIND OFF"
        "CATCH_BUILD_TESTING OFF"
        "CATCH_BUILD_EXAMPLES OFF"
        "CATCH_BUILD_EXTRA_TESTS OFF"
        "CATCH_ENABLE_COVERAGE OFF"
        "CATCH_ENABLE_WERROR ON"
        "CATCH_INSTALL_DOCS OFF"
        "CATCH_INSTALL_HELPERS OFF"
)
include(${Catch2_SOURCE_DIR}/contrib/Catch.cmake)

#######################################################
### Tests                                           ###
#######################################################

add_executable(ut_midi_stream ut_midi_stream.cpp)
target_link_libraries(ut_midi_stream PUBLIC composermidi Catch2)

if (MSVC)
    set_target_properties(ut_midi_stream PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
elseif()
    set_target_properties(ut_midi_stream PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

get_target_property(RESOURCES_DIR ut_midi_stream RUNTIME_OUTPUT_DIRECTORY)

configure_file("resources/twinkle_harmonic.mid" "${RESOURCES_DIR}/resources/twinkle_harmonic.mid" COPYONLY)
configure_file("resources/twinkle.midi" "${RESOURCES_DIR}/resources/twinkle.midi" COPYONLY)
