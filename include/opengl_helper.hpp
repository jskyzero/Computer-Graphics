#ifndef __OPENGL_HELPER__
#define __OPENGL_HELPER__

#include <glad/glad.h>   // for glad
#include <GLFW/glfw3.h>  // for glfw

#include <functional>
#include <string>  // for std::string

// if you are macOS, please uncomment fellow define code
// #define IS_MAC_OS
#define INFO_LOG_SIZE 512

// namespace helper
namespace helper {
// return some info
std::string get_info();

// initial opengl
void initial_glfw();
// initial glad
void initial_glad();
// initial imgui
void initial_imgui(GLFWwindow * window);
// inital opengl
void initial_opengl(std::function<void(void)> set_window, GLFWwindow*  & window);
// set vao
void set_vao(GLuint & VAO, GLuint & VBO, GLuint & EBO,
             std::function<void(GLuint, GLuint, GLuint)> set_buffer);

// check if shader works well
typedef PFNGLGETSHADERIVPROC CheckShaderHasErrorFunc;
typedef PFNGLGETSHADERINFOLOGPROC GetShaderErrorMessageFunc;
void check_shader_have_error(GLuint id, GLenum type,
                             CheckShaderHasErrorFunc check_has_error,
                             GetShaderErrorMessageFunc get_error_info);
// compile vertex shader
GLuint compile_vertex_shader(std::string path);
// compile fragment shader
GLuint compile_fragment_shader(std::string path);
// compile shader
GLuint compile_shader(GLenum type, std::string path);
// create program and link shader
GLuint create_program_with_shader(GLuint vertex_shader, GLuint fragment_shader);
// create program and link shader with file path
GLuint create_program_with_shader(std::string vertex_shader_path,
                                  std::string fragment_shader_path);

// read string from path
std::string read_string_from_path(std::string path);
// test if error
void assert_true(bool isOK, std::string error_message);
// commen exit
void exit_program();
// exit with exit code check
void exit_program(int exit_code);
// exit with error message
void exit_program(std::string exit_message);
}  // namespace helper

#endif