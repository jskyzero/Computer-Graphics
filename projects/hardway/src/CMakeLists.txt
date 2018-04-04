set(file_list hello_window;hello_triangle;hello_triangle_hw;set_pixel)

# forloop
foreach(each_file ${file_list})

add_executable (${each_file} ${each_file}.cpp)
target_link_libraries (${each_file} glad glfw ${OPENGL_gl_LIBRARY})

endforeach (each_file)