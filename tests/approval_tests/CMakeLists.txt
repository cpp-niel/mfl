cmake_minimum_required (VERSION 3.12)

find_package(unofficial-cairo REQUIRED)
find_package(doctest CONFIG REQUIRED)

project (approval_tests)

add_executable (approval_tests
        docs.cpp
        main.cpp
        mfl.cpp
        renderer/svg_renderer.cpp
        )

set_property(TARGET approval_tests PROPERTY CXX_STANDARD 20)

target_include_directories(approval_tests PUBLIC
        ./
        ./../fonts_for_tests/include
        ./../../include
        ./../../src
        )

target_link_libraries(approval_tests PUBLIC
        project_options
        project_warnings
        fonts_for_tests
        mfl
        unofficial::cairo::cairo
        doctest::doctest
        )
