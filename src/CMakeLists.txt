add_executable (out homework6.cpp 
                    opengl_helper.cpp 
                    imgui_impl_glfw_gl3.cpp)
target_link_libraries (out glad glfw imgui ${OPENGL_gl_LIBRARY})