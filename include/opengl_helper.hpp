#ifndef __OPENGL_HELPER__
#define __OPENGL_HELPER__

/*
 * ┌───────────────────────────────────────────────────────────────────────────┐
 * |                                                                           |
 * |                               OpenGL Helper                               |
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * |                                                            jskyzero 2018  |
 * └───────────────────────────────────────────────────────────────────────────┘
 *
 *
 * 1.Introdution
 * OpenGL Helper is a help header file library, It contails many useful
 * functions when you use OpenGl.
 *
 * 2.Requirment
 * Require Library:
 *  GLFW
 *  GLAD
 *  GLM
 *  stb_image
 *  imgui(optional) -> not use please #define NOT_USE_IMGUI
 *
 */


// for glad, initial function pointer address(maybe
#include <glad/glad.h>
// for glfw, a OpenGL, OpenGL ES and Vulkan library
#include <GLFW/glfw3.h>
// for glm, OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/string_cast.hpp>

// for stb_image, a image loader
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifndef NOT_USE_IMGUI
  // for imgui, a c++ ui library
  #include "imgui.h"
  #include "imgui_impl_glfw_gl3.h"
#endif

#include <cstdlib>     // for exit
#include <iostream>    // for std::cout
#include <fstream>     // for std::ifstream
#include <sstream>     // for std::stringstream
#include <functional>  // for function
#include <vector>

#define INFO_LOG_SIZE 512

// namespace helper
namespace helper {

/*
 *
 * COMMON FUNCTIONS
 *
 *
 *
 * */

// return some info
std::string version() { return "OpenGL Helper 0.0.1 by jskyzero"; }

// exit with exit code check
void exit_program(int exit_code) {
  glfwTerminate();
  exit(0);
}
// commen exit
void exit_program() { exit_program(0); }

// exit with error message
void exit_program(std::string exit_message) {
  std::cout << exit_message << std::endl;
  exit_program(-1);
}

// test if error
void assert_true(bool isOK, std::string error_message) {
  if (isOK) return;
  exit_program(error_message);
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

/*
 *
 * OPENGL FUNCTIONS
 *
 *
 *
 * */

// initial opengl
void initial_glfw() {
  assert_true(glfwInit() == GLFW_TRUE, "glfw initial error");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  #ifdef __APPLE__
    glfwWindowHint(
        GLFW_OPENGL_FORWARD_COMPAT,
        GL_TRUE);  // uncomment this statement to fix compilation on OS X
  #endif
}

// initial glad
void initial_glad() {
  assert_true(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 0,
              "glad load errror");
}

#ifndef NOT_USE_IMGUI

// initial imgui
void initial_imgui(GLFWwindow* window) {
  // Setup ImGui binding
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard
  // Controls  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable
  // Gamepad Controls
  ImGui_ImplGlfwGL3_Init(window, true);

  // Setup style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default_CAMERA font. You can
  // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
  // them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
  // need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please
  // handle those errors in your application (e.g. use an assertion, or display
  // an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored
  // into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
  // ImGui_ImplXXXX_NewFrame below will call.
  // - Read 'misc/fonts/README.txt' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // NULL, io.Fonts->GetGlyphRangesJapanese());  IM_ASSERT(font != NULL);
}

#endif

// inital opengl
void InitialOpenGL(std::function<void(void)> set_window, GLFWwindow*& window) {
  initial_glfw();
  set_window();
  initial_glad();
  #ifndef NOT_USE_IMGUI
    initial_imgui(window);
  #endif
  glViewport(0, 0, 800, 600);
}

// set vao
void SetVAO(GLuint& VAO, GLuint& VBO, GLuint& EBO,
             std::function<void(GLuint, GLuint, GLuint)> set_buffer,
             bool is_buffer_need_initial = true) {
  glGenVertexArrays(1, &VAO);
  if (is_buffer_need_initial) glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  set_buffer(VAO, VBO, EBO);
}

typedef PFNGLGETSHADERIVPROC CheckShaderHasErrorFunc;
typedef PFNGLGETSHADERINFOLOGPROC GetShaderErrorMessageFunc;

// check if shader works well
void CheckShaderHaveError(GLuint id, GLenum type,
                             CheckShaderHasErrorFunc check_has_error,
                             GetShaderErrorMessageFunc get_error_info) {
  GLint no_error;
  GLchar info_log[INFO_LOG_SIZE];
  check_has_error(id, type, &no_error);
  if (no_error) return;
  get_error_info(id, INFO_LOG_SIZE, NULL, info_log);
  exit_program(info_log);
}

// compile shader
GLuint CompileShader(GLenum type, std::string path) {
  GLuint shader = glCreateShader(type);
  std::string str = read_string_from_path(path);
  const GLchar* source = str.c_str();
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  CheckShaderHaveError(shader, GL_COMPILE_STATUS, glGetShaderiv,
                          glGetShaderInfoLog);
  return shader;
}

// compile vertex shader
GLuint CompileVertexShader(std::string path) {
  return CompileShader(GL_VERTEX_SHADER, path);
}

// compile fragment shader
GLuint CompileFragmentShader(std::string path) {
  return CompileShader(GL_FRAGMENT_SHADER, path);
}

// create program and link shader
GLuint CreatProgramWithShader(GLuint vertex_shader,
                                  GLuint fragment_shader) {
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  CheckShaderHaveError(program, GL_LINK_STATUS, glGetProgramiv,
                          glGetProgramInfoLog);
  return program;
}

// create program and link shader with file path
GLuint CreatProgramWithShader(std::string vertex_shader_path,
                                  std::string fragment_shader_path) {
  GLuint vertex_shader = CompileVertexShader(vertex_shader_path);
  GLuint fragment_shader = CompileFragmentShader(fragment_shader_path);
  GLuint program = CreatProgramWithShader(vertex_shader, fragment_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  return program;
}

// create texture with file path
void CreateTexture(GLuint& texture, std::string file_path) {
  // load and create a texture
  // -------------------------
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);  // all upcoming GL_TEXTURE_2D
                                          // operations now have effect on this
                                          // texture object
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_REPEAT);  // set texture wrapping to GL_REPEAT (default
                               // wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  // tell stb_image.h to flip loaded texture's on the y-axis.
  stbi_set_flip_vertically_on_load(false);
  // std::string str = read_string_from_path(file_path);
  unsigned char* data =
      stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);
  assert_true(data, "Failed to load texture " + file_path);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
}

// set int
void SetShaderInt(GLuint id, const std::string& name, int value) {
  glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
// set mat4
void SetShaderMat4(GLuint id, const std::string& name, const glm::mat4& mat) {
  // std::cout << glm::to_string(mat) << std::endl;
  glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}



// Default camera values
const float kDefaultCameraYaw         = -90.0f;
const float kDefaultCameraPitch       =  0.0f;
const float kDefaultCameraSpeed       =  2.0f;
const float kDefaultCameraSensivity =  0.1f;
const float kDefaultCameraZoom        =  45.0f;

// An abstract camera class that processes input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
 public:
  // Defines several possible options for camera movement. Used as abstraction
  // to stay away from window-system specific input methods
  enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };
  // Camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // Euler Angles
  float Yaw;
  float Pitch;
  // Camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  // Constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
         float yaw = kDefaultCameraYaw, float pitch = kDefaultCameraPitch)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(kDefaultCameraSpeed),
        MouseSensitivity(kDefaultCameraSensivity),
        Zoom(kDefaultCameraZoom) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
  }
  // Constructor with scalar values
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
         float yaw, float pitch)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(kDefaultCameraSpeed),
        MouseSensitivity(kDefaultCameraSensivity),
        Zoom(kDefaultCameraZoom) {
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
  }

  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
  }

  // Processes input received from any keyboard-like input system. Accepts input
  // parameter in the form of camera defined ENUM (to abstract it from windowing
  // systems)
  void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD) Position += Front * velocity;
    if (direction == BACKWARD) Position -= Front * velocity;
    if (direction == LEFT) Position -= Right * velocity;
    if (direction == RIGHT) Position += Right * velocity;
  }

  // Processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset,
                            GLboolean constrainPitch = true) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
      if (Pitch > 89.0f) Pitch = 89.0f;
      if (Pitch < -89.0f) Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
  }

  // Processes input received from a mouse scroll-wheel event. Only requires
  // input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset) {
    if (Zoom >= 1.0f && Zoom <= 45.0f) Zoom -= yoffset;
    if (Zoom <= 1.0f) Zoom = 1.0f;
    if (Zoom >= 45.0f) Zoom = 45.0f;
  }

 private:
  // Calculates the front vector from the Camera's (updated) Euler Angles
  void UpdateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(
        Front, WorldUp));  // Normalize the vectors, because their length gets
                           // closer to 0 the more you look up or down which
                           // results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
  }
};

}  // namespace helper

#endif