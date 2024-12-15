cmake_minimum_required(VERSION 3.16)



set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization

project(MarchingSquares)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 23)

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES} )


target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE SDL2)