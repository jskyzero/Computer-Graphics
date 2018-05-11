add_executable (out homework7.cpp 
                    imgui_impl_glfw_gl3.cpp)

target_link_libraries (out glad glfw imgui assimp ${OPENGL_gl_LIBRARY})