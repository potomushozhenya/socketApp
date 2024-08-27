cmake_minimum_required(VERSION 2.8)

project(prog2)

set(SOURCE_EXE main.cpp)

set(SOURCE_LIB prog2.cpp)

add_library(prog2 STATIC ${SOURCE_LIB})	

add_executable(main ${SOURCE_EXE})

target_link_libraries(main prog2)