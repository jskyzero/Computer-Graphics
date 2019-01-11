project (HardWay)
cmake_minimum_required (VERSION 3.1)

IF (WIN32)
  MESSAGE(STATUS "OS Version: Windows")
  set (OPENGL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../OpenGL-Playground)
ELSEIF (APPLE)
  MESSAGE(STATUS "OS Version: Apple systens.")
  set (OPENGL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../OpenGL-Playground)
ELSEIF (UNIX)
  MESSAGE(STATUS "OS Version: UNIX-like OS's.")
  set (OPENGL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../OpenGL-Playground)
ENDIF()

set (CMAKE_CXX_FLAGS "-Wall")
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

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