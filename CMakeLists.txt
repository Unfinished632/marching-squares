cmake_minimum_required(VERSION 3.16)



set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization

project(MarchingSquares)

# STATIC stuff (Windows)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(BUILD_FOR_WIN TRUE)
endif()
option(STATIC_BUILD "Build a static binary." ${BUILD_FOR_WIN})

if (STATIC_BUILD)
        set(CMAKE_EXE_LINKER_FLAGS "-static")
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" CONFIG)
        set(BUILD_SHARED_LIBS OFF)
endif()

add_subdirectory(thirdparty/imgui-docking)
add_subdirectory(thirdparty/SDL)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 23)

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES} )


target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE SDL2-static imgui)