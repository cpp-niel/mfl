cmake_minimum_required(VERSION 3.14)
project(mfl)

set(CMAKE_CXX_STANDARD 20)


add_library(project_options INTERFACE)
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)


add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)


include(cmake/StaticAnalyzers.cmake)


add_subdirectory(src)
add_subdirectory(tests/fonts_for_tests)
add_subdirectory(tests/unit_tests)

# Building approval tests causes ICE on msvc
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_subdirectory(tests/approval_tests)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_subdirectory(tests/fuzz_tests)
endif()

include(GNUInstallDirs)

install(TARGETS mfl project_options project_warnings EXPORT mfl-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(EXPORT mfl-export DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mfl NAMESPACE mfl:: FILE mfl-config.cmake)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN *.hpp)
endif()

