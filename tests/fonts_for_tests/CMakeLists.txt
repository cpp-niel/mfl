cmake_minimum_required (VERSION 3.5)

project (fonts_for_tests)

find_package(freetype CONFIG REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)

add_library (fonts_for_tests
        src/font_face.cpp
        src/freetype.cpp
        src/freetype_error.cpp
        )

set_property(TARGET fonts_for_tests PROPERTY CXX_STANDARD 20)

target_compile_definitions(fonts_for_tests PRIVATE MFL_FONT_DIR="${CMAKE_SOURCE_DIR}/tests/fonts_for_tests/otf/")

target_include_directories(fonts_for_tests PUBLIC ./src ./include ./../../include)

target_link_libraries(fonts_for_tests PUBLIC
        freetype
        harfbuzz::harfbuzz
        project_options
        project_warnings
        )
