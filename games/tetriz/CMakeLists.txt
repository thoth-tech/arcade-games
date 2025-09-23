cmake_minimum_required(VERSION 3.22)
project(tetriz)

set(CMAKE_CXX_STANDARD 20)

configure_file( ${PROJECT_SOURCE_DIR}/tetriz.map ${PROJECT_BINARY_DIR})

aux_source_directory(. src)
add_executable(tetriz ${src})

if(MSVC)
    target_compile_options(tetriz PRIVATE "/utf-8")
    set_property(TARGET tetriz PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
