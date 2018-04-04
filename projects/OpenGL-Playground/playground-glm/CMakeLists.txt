project (playground)
cmake_minimum_required (VERSION 3.1)

set(BUILD_DIR ./)

add_subdirectory(../glm glm)

include_directories(../glm)

add_executable (out main.cpp )
target_link_libraries (out glm)