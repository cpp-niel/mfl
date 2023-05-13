cmake_minimum_required (VERSION 3.14)

project (fuzz_tests)

add_executable (fuzz_tests
        main.cpp
        )

set_property(TARGET fuzz_tests PROPERTY CXX_STANDARD 20)

target_include_directories(fuzz_tests PUBLIC
        ./
        ./../fonts_for_tests/include
        ./../../include
        )

target_link_libraries(fuzz_tests PUBLIC
        mfl_options
        mfl_warnings
        fonts_for_tests
        mfl
        -coverage
        -fsanitize=fuzzer,undefined,address
        )

target_compile_options(fuzz_tests PRIVATE -fsanitize=fuzzer,undefined,address)