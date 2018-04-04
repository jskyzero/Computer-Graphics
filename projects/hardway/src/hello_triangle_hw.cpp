#include <hello_triangle_hw.hpp>

GLfloat vertices[] = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};

GLuint indices1[] = { // 注意索引从0开始! 
    1, 2, 3  // 第二个三角形
};

GLuint indices2[] = { // 注意索引从0开始! 
    0, 1, 3, // 第一个三角形
};

const GLchar* vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}";

const GLchar* fragment_shader_source1 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}";

const GLchar* fragment_shader_source2 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "  FragColor = vec4(0.2f, 0.5f, 1.0f, 1.0f);\n"
    "}";

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "HW Triangle", NULL, NULL);
  test_error(window != NULL, "Failed to create GLFW windows");
  glfwMakeContextCurrent(window);

  glfwMakeContextCurrent(window);
  test_error(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), 
            "Failed to initialze GLAD");

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  test_shader_success(vertex_shader,GL_COMPILE_STATUS, glGetShaderiv, glGetShaderInfoLog);
  GLuint fragment_shader1 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader1, 1, &fragment_shader_source1, NULL);
  glCompileShader(fragment_shader1);
  test_shader_success(fragment_shader1,GL_COMPILE_STATUS, glGetShaderiv, glGetShaderInfoLog);
  GLuint fragment_shader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader2, 1, &fragment_shader_source2, NULL);
  glCompileShader(fragment_shader2);
  test_shader_success(fragment_shader2,GL_COMPILE_STATUS, glGetShaderiv, glGetShaderInfoLog);


  GLuint shader_program1 = glCreateProgram();
  glAttachShader(shader_program1, vertex_shader);
  glAttachShader(shader_program1, fragment_shader1);
  glLinkProgram(shader_program1);
  test_shader_success(shader_program1, GL_LINK_STATUS, glGetProgramiv, glGetProgramInfoLog);

    GLuint shader_program2 = glCreateProgram();
  glAttachShader(shader_program2, vertex_shader);
  glAttachShader(shader_program2, fragment_shader2);
  glLinkProgram(shader_program2);
  test_shader_success(shader_program2, GL_LINK_STATUS, glGetProgramiv, glGetProgramInfoLog);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader1);
  glDeleteShader(fragment_shader2);

  GLuint VAO1, VBO1, EBO1;
  glGenVertexArrays(1, &VAO1);
  glGenBuffers(1, &VBO1);
  glGenBuffers(1, &EBO1);

  glBindVertexArray(VAO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);


  GLuint VAO2, VBO2, EBO2;
  glGenVertexArrays(1, &VAO2);
  glGenBuffers(1, &VBO2);
  glGenBuffers(1, &EBO2);

  glBindVertexArray(VAO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  glViewport(0, 0, 800, 600);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program1);
    glBindVertexArray(VAO1);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glUseProgram(shader_program2);
    glBindVertexArray(VAO2);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    glfwPollEvents();
    glfwSwapBuffers(window);

  }

  exit_program(0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void test_shader_success(GLuint id, int type, PFNGLGETSHADERIVPROC f, PFNGLGETSHADERINFOLOGPROC f2) {
  GLint isSuccess;
  char info_log[512];
  f(id, type, &isSuccess);
  if (!isSuccess) {
    f2(id, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::VERTEX::COMPIATION_FAILED\n"
              << info_log << std::endl;
    exit_program(-1);
  }
}

void exit_program(int exit_code) {
  glfwTerminate();
  exit(exit_code);
}

void exit_program(int exit_code, std::string message) {
  std::cout << message << std::endl;
  glfwTerminate();
  exit(exit_code);
}

void test_error(bool isOK, std::string message) {
  if (isOK) return;
  exit_program(-1, message);
}