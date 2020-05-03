cmake_minimum_required (VERSION 3.5)

find_package(unofficial-cairo REQUIRED)

project (approval_tests)

add_executable (approval_tests
        docs.cpp
        framework/svg_comparator.cpp
        main.cpp
        mfl.cpp
        renderer/render_formulas.cpp
        renderer/svg_renderer.cpp
        framework/approve.cpp framework/approve.hpp)

set_property(TARGET approval_tests PROPERTY CXX_STANDARD 20)

target_compile_definitions(approval_tests PRIVATE MFL_FONT_DIR="${PROJECT_SOURCE_DIR}/otf/")

target_include_directories(approval_tests PUBLIC
        ./
        ./../fonts_for_tests/include
        ./../../include
        ./../../src
        )

target_link_libraries(approval_tests PUBLIC
        unofficial::cairo::cairo
        project_options
        project_warnings
        fonts_for_tests
        mfl
        )
