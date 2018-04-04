project (homework4)
cmake_minimum_required (VERSION 3.1)

set (CMAKE_CXX_STANDARD 14)
set (OPENGL_PATH /home/jskyzero/workspace/OpenGl)

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
add_library (imgui ${OPENGL_PATH}/imgui/imgui.cpp ${OPENGL_PATH}/imgui/imgui_demo.cpp ${OPENGL_PATH}/imgui/imgui_draw.cpp)

include_directories (${OPENGL_PATH}/glfw/include)
include_directories (${OPENGL_PATH}/glad/include)
include_directories (${OPENGL_PATH}/imgui)
include_directories (${OPENGL_PATH}/glm)
include_directories (include)

add_subdirectory (src)