project (playground)
cmake_minimum_required (VERSION 3.1)

set(BUILD_DIR ./)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)

add_subdirectory(../glfw glfw)
add_library(glad ../glad/src/glad.c)
include_directories(../glfw/include)
include_directories(../glad/include)


add_subdirectory(src)