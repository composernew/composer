#######################################################
### Dependencies                                    ###
#######################################################
# Threads
find_package(Threads)

# Infra library
FetchContent_Declare(
        infra
        GIT_REPOSITORY https://github.com/cycfi/infra.git
        GIT_TAG f3e33fe0340d77f9ce4ace1fc6f1a569e033e173
)
FetchContent_MakeAvailable(infra)

# Q library
FetchContent_Declare(
        Q
        GIT_REPOSITORY https://github.com/cycfi/q
        GIT_TAG 3e246461b40ded09b34a51192a3c0e7184290655
)

set(Q_BUILD_EXAMPLES OFF)
set(Q_BUILD_TEST OFF)

FetchContent_GetProperties(Q)
if(NOT q_POPULATED)
    FetchContent_Populate(Q)
    set(q_ROOT ${q_SOURCE_DIR})
    get_filename_component(q_ROOT "${q_ROOT}" ABSOLUTE)
    add_subdirectory(${q_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/q EXCLUDE_FROM_ALL)
endif()

# Midifile library
FetchContent_Declare(
        midifile
        GIT_REPOSITORY https://github.com/craigsapp/midifile.git
        GIT_TAG ee165a3f418684febf16b5b06de5e1dd7e9613c1
)

FetchContent_GetProperties(midifile)
if(NOT midifile_POPULATED)
    FetchContent_Populate(midifile)
    set(midifile_ROOT ${midifile_SOURCE_DIR})
    get_filename_component(midifile_ROOT "${midifile_ROOT}" ABSOLUTE)
    add_subdirectory(${midifile_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/midifile)
endif()

target_include_directories(midifile PUBLIC "${midifile_SOURCE_DIR}/include")

# Composerlib library
FetchContent_Declare(
        composerlib
        GIT_REPOSITORY https://github.com/composernew/composerlib.git
        GIT_TAG 798ad10b67b3056074d882aebd20f1e424ac537b
)

FetchContent_GetProperties(composerlib)
if (NOT composerlib_POPULATED)
    FetchContent_Populate(composerlib)
    add_subdirectory(${composerlib_SOURCE_DIR} ${composerlib_BINARY_DIR} EXCLUDE_FROM_ALL)
    set(composerlib_BUILD_PARETO_WITH_PMR_BY_DEFAULT OFF)
endif ()

#######################################################
### Library                                         ###
#######################################################
add_library(composermidi
        composermidi/harmony.cpp
        composermidi/harmony.h
        composermidi/synth.h
        composermidi/synth.cpp
        composermidi/midi_stream.cpp
        composermidi/midi_stream.h
        composermidi/midi_processor.cpp
        composermidi/midi_processor.h
        composermidi/midi_io.cpp
        composermidi/midi_io.h
)

target_include_directories(composermidi
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
               $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(composermidi PUBLIC Threads::Threads libq libqio cycfi::infra midifile composerlib)
target_pedantic_options(composermidi)
target_exception_options(composermidi)
