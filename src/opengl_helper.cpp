#include <opengl_helper.hpp>

#include <cstdlib>   // for exit
#include <fstream>   // for std::ifstream
#include <iostream>  // for std::cout
#include <sstream>   // for std::stringstream

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

namespace helper {
std::string get_info() { return "OpenGL Helper by jskyzero"; }

// initial opengl
void initial_glfw() {
  assert_true(glfwInit() == GLFW_TRUE, "glfw initial error");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef IS_MAC_OS
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

// initial glad
void initial_glad() {
  assert_true(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 0,
              "glad load errror");
}

// initial imgui
void initial_imgui(GLFWwindow * window) {
  // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    ImGui_ImplGlfwGL3_Init(window, true);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
}

// inital opengl
void initial_opengl(std::function<void(void)> set_window, GLFWwindow* & window) {
  initial_glfw();
  set_window();
  initial_glad();
  initial_imgui(window);
  glViewport(0, 0, 800, 600);
}

// set vao
void set_vao(GLuint & VAO, GLuint & VBO, GLuint & EBO,
             std::function<void(GLuint, GLuint, GLuint)> set_buffer) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  set_buffer(VAO, VBO, EBO);
}


// check if shader works well
void check_shader_have_error(GLuint id, GLenum type,
                             CheckShaderHasErrorFunc check_has_error,
                             GetShaderErrorMessageFunc get_error_info) {
  GLint no_error;
  GLchar info_log[INFO_LOG_SIZE];
  check_has_error(id, type, &no_error);
  if (no_error) return;
  get_error_info(id, INFO_LOG_SIZE, NULL, info_log);
  exit_program(info_log);
}


// compile vertex shader
GLuint compile_vertex_shader(std::string path) {
  return compile_shader(GL_VERTEX_SHADER, path);
}

// compile fragment shader
GLuint compile_fragment_shader(std::string path) {
  return compile_shader(GL_FRAGMENT_SHADER, path);
}

// compile shader
GLuint compile_shader(GLenum type, std::string path) {
  GLuint shader = glCreateShader(type);
  std::string str = read_string_from_path(path);
  const GLchar* source = str.c_str();
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  check_shader_have_error(shader, GL_COMPILE_STATUS, glGetShaderiv, glGetShaderInfoLog);
  return shader;
}

// create program and link shader
GLuint create_program_with_shader(GLuint vertex_shader, GLuint fragment_shader) {
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  check_shader_have_error(program, GL_LINK_STATUS, glGetProgramiv, glGetProgramInfoLog);
  return program;
}

// create program and link shader with file path
GLuint create_program_with_shader(std::string vertex_shader_path,
                                  std::string fragment_shader_path) {
  GLuint vertex_shader = compile_vertex_shader(vertex_shader_path);
  GLuint fragment_shader = compile_fragment_shader(fragment_shader_path);
  GLuint program = create_program_with_shader(vertex_shader, fragment_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  return program;
}


// read string from path
std::string read_string_from_path(std::string path) {
  std::ifstream in;
  std::stringstream ss;
  // ensure ifstream objects can throw exceptions:
  in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open files
    in.open(path.c_str());
    // read file's buffer contents into streams
    ss << in.rdbuf();
    // close file handlers
    in.close();
  } catch (std::ifstream::failure e) {
    std::string error_message = "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
    error_message += "\nFile path: ";
    error_message += path;
    exit_program(error_message);
  }
  // convert stream into string
  return ss.str();
}

// test if error
void assert_true(bool isOK, std::string error_message) {
  if (isOK) return;
  exit_program(error_message);
}

// commen exit
void exit_program() { exit_program(0); }
// exit with exit code check
void exit_program(int exit_code) {
  glfwTerminate();
  exit(0);
}

// exit with error message
void exit_program(std::string exit_message) {
  std::cout << exit_message << std::endl;
  exit_program(-1);
}

}  // namespace helper