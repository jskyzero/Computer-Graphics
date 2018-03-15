project (HardWay)
cmake_minimum_required (VERSION 3.1)

set (OPENGL_PATH /home/jskyzero/workspace/OpenGl)

# GLFW
set (GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package (OpenGL REQUIRED)

add_subdirectory (${OPENGL_PATH}/glfw glfw)
add_library (glad ${OPENGL_PATH}/glad/src/glad.c)

include_directories (${OPENGL_PATH}/glfw/include)
include_directories (${OPENGL_PATH}/glad/include)
include_directories (include)

add_subdirectory (src)