include_directories (../include)

add_executable (test.out test.cpp)
target_link_libraries (test.out glad glfw ${OPENGL_gl_LIBRARY})