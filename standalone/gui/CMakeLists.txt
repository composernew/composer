#######################################################
### Elements                                        ###
#######################################################
FetchContent_Declare(
        elements
        GIT_REPOSITORY https://github.com/cycfi/elements.git
        GIT_TAG 3d0e59d437b8ac6efd17beb2fbd92adc4dbbc8c3
)

option(ELEMENTS_BUILD_EXAMPLES OFF)

FetchContent_GetProperties(elements)
if(NOT elements_POPULATED)
    FetchContent_Populate(elements)
    set(ELEMENTS_ROOT ${elements_SOURCE_DIR})
    get_filename_component(ELEMENTS_ROOT "${ELEMENTS_ROOT}" ABSOLUTE)
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${ELEMENTS_ROOT}/cmake")
    include(ElementsConfigCommon)
    add_subdirectory(${ELEMENTS_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/elements EXCLUDE_FROM_ALL)
endif()

#######################################################
### Composer                                        ###
#######################################################
# Elements options
set(ELEMENTS_APP_PROJECT "Composer")
set(ELEMENTS_APP_TITLE "Composer")
set(ELEMENTS_APP_COPYRIGHT "Copyright (c) 2020")
set(ELEMENTS_APP_ID "com.composer.composer")
set(ELEMENTS_APP_VERSION "0.1")
set(ELEMENTS_APP_DEPENDENCIES composer)

set(ELEMENTS_RESOURCES
        ${ELEMENTS_ROOT}/resources/fonts/elements_basic.ttf
        ${ELEMENTS_ROOT}/resources/fonts/OpenSans-Light.ttf
        ${ELEMENTS_ROOT}/resources/fonts/OpenSans-Regular.ttf
        ${ELEMENTS_ROOT}/resources/fonts/OpenSans-SemiBold.ttf
        ${ELEMENTS_ROOT}/resources/fonts/OpenSans-Bold.ttf
        ${ELEMENTS_ROOT}/resources/fonts/OpenSansCondensed-Light.ttf
        ${ELEMENTS_ROOT}/resources/fonts/Roboto-Light.ttf
        ${ELEMENTS_ROOT}/resources/fonts/Roboto-Regular.ttf
        ${ELEMENTS_ROOT}/resources/fonts/Roboto-Medium.ttf
        ${ELEMENTS_ROOT}/resources/fonts/Roboto-Bold.ttf)

file(COPY ${ELEMENTS_RESOURCES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/resources")

if (APPLE)
    add_executable(composer MACOSX_BUNDLE main.cpp application.h application.cpp ${ELEMENTS_RESOURCES})
    target_link_options(composer PRIVATE -framework AppKit)
    set(MACOSX_BUNDLE_BUNDLE_NAME ${ELEMENTS_APP_TITLE})
    set(MACOSX_BUNDLE_COPYRIGHT ${ELEMENTS_APP_COPYRIGHT})
    set(MACOSX_BUNDLE_GUI_IDENTIFIER ${ELEMENTS_APP_ID})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${ELEMENTS_APP_VERSION})
    set_source_files_properties(
            ${ELEMENTS_RESOURCES}
            ${ELEMENTS_APP_RESOURCES}
            PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources
    )
    set_target_properties(
            composer
            PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST ${ELEMENTS_ROOT}/resources/macos/plist.in
    )
elseif (UNIX AND NOT APPLE)
    add_executable(composer main.cpp application.h application.cpp ${ELEMENTS_RESOURCES})
elseif (WIN32)
    add_executable(composer WIN32 main.cpp application.h application.cpp ${ELEMENTS_RESOURCES})
    if (MSVC)
        target_link_options(composer PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup shcore.lib)
        if (CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits?
            set(CAIRO_DLL ${ELEMENTS_ROOT}/lib/external/cairo/lib/x64/cairo.dll)
            set(FREETYPE_DLL ${ELEMENTS_ROOT}/lib/external/freetype/win64/freetype.dll)
            set(FONTCONFIG_DLL ${ELEMENTS_ROOT}/lib/external/fontconfig/x64/fontconfig.dll)
            set(ICONV_DLL ${ELEMENTS_ROOT}/lib/external/fontconfig/x64/libiconv.dll)
            set(XML2 ${ELEMENTS_ROOT}/lib/external/fontconfig/x64/libxml2.dll)
        else()
            set(CAIRO_DLL ${ELEMENTS_ROOT}/lib/external/cairo/lib/x86/cairo.dll)
            set(FREETYPE_DLL ${ELEMENTS_ROOT}/lib/external/freetype/win32/freetype.dll)
            set(FONTCONFIG_DLL ${ELEMENTS_ROOT}/lib/external/fontconfig/x86/fontconfig.dll)
            set(ICONV_DLL ${ELEMENTS_ROOT}/lib/external/fontconfig/x86/libiconv.dll)
            set(XML2 ${ELEMENTS_ROOT}/lib/external/fontconfig/x86/libxml2.dll)
        endif()
        file(COPY ${CAIRO_DLL} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
        file(COPY ${FREETYPE_DLL} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
        file(COPY ${FONTCONFIG_DLL} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
        file(COPY ${ICONV_DLL} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
        file(COPY ${XML2} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
        set_property(
                TARGET composer
                PROPERTY
                VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    endif()
endif()

target_link_libraries(composer PRIVATE elements composerlib)
target_compile_options(composer PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

if(IPO_SUPPORTED AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(composer PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

