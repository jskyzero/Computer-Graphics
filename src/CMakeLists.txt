add_executable (create_window create_window.cpp)
target_link_libraries (create_window glad glfw ${OPENGL_gl_LIBRARY})