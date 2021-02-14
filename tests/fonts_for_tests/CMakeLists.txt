cmake_minimum_required (VERSION 3.14)

project (fonts_for_tests)

find_package(freetype CONFIG REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)
find_package(expat CONFIG REQUIRED)
find_package(Fontconfig)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    find_path(LIBUUID_INCLUDE_DIR uuid.h PATH_SUFFIXES uuid)
    find_library(LIBUUID_LIBRARY libuuid.a)
endif()

add_library (fonts_for_tests
        src/font_face.cpp
        src/freetype.cpp
        src/freetype_error.cpp
        )

set_property(TARGET fonts_for_tests PROPERTY CXX_STANDARD 20)

target_compile_definitions(fonts_for_tests PRIVATE MFL_FONT_DIR="${CMAKE_CURRENT_LIST_DIR}/otf/")

target_include_directories(fonts_for_tests PUBLIC ./src ./include ./../../include)

target_link_libraries(fonts_for_tests PUBLIC
        freetype
        Fontconfig::Fontconfig
        harfbuzz::harfbuzz
        expat::expat
        project_options
        project_warnings
        )

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_link_libraries(fonts_for_tests PUBLIC ${LIBUUID_LIBRARY})
endif()
