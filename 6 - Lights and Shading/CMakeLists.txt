project (homework6)
cmake_minimum_required (VERSION 3.1)

set (CMAKE_CXX_STANDARD 14)

IF (WIN32)
  MESSAGE(STATUS "OS Version: Windows")
  set (OPENGL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../projects/OpenGL-Playground)
ELSEIF (APPLE)
  MESSAGE(STATUS "OS Version: Apple systens.")
  set (OPENGL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../projects/OpenGL-Playground)
ELSEIF (UNIX)
  MESSAGE(STATUS "OS Version: UNIX-like OS's.")
  set (OPENGL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../projects/OpenGL-Playground)
ENDIF()

# if (UNIX)
#   set (OPENGL_PATH ../OpenGL)
# endif (UNIX)

# if (MSVC)
#   set (OPENGL_PATH ../OpenGL)
# endif (MSVC)

# in linux, you can uncomment this warn setting
# set (CMAKE_CXX_FLAGS "-Wall")

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/)

# GLFW
set (GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package (OpenGL REQUIRED)

add_subdirectory (${OPENGL_PATH}/glfw glfw)
add_subdirectory (${OPENGL_PATH}/glm glm)
add_library (glad ${OPENGL_PATH}/glad/src/glad.c)
add_library (imgui ${OPENGL_PATH}/imgui/imgui.cpp 
                   ${OPENGL_PATH}/imgui/imgui_demo.cpp 
                   ${OPENGL_PATH}/imgui/imgui_draw.cpp)

include_directories (${OPENGL_PATH}/glfw/include)
include_directories (${OPENGL_PATH}/glad/include)
include_directories (${OPENGL_PATH}/imgui)
include_directories (${OPENGL_PATH}/glm)
include_directories (include)

add_subdirectory (src)