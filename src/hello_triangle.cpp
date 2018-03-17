#include <hello_triangle.hpp>


// typedef unsigned int GLenum;
// typedef unsigned char GLboolean;
// typedef unsigned int GLbitfield;
// typedef signed char GLbyte;
// typedef short GLshort;
// typedef int GLint;
// typedef int GLsizei;
// typedef unsigned char GLubyte;
// typedef unsigned short GLushort;
// typedef unsigned int GLuint;
// typedef float GLfloat;
// typedef float GLclampf;
// typedef double GLdouble;
// typedef double GLclampd;
// typedef void GLvoid;
//
// int, float VS. GLint, GLfloat
// https://www.opengl.org/discussion_boards/showthread.php/149111-int-float-VS-GLint-GLfloat

GLfloat vertices[] = {
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.0f,  0.5f, 0.0f
};

const GLchar * vertex_shader_source = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}";

const GLchar * fragment_shader_source = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}";



int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW windows" << std::endl;
    exit_program(-1);
  }

  std::cout << "Success to create GLFW windows" << std::endl;
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialze GLAD" << std::endl;
    exit_program(-1);
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  //使用glGenBuffers函数和一个缓冲ID生成一个VBO对象
  GLuint VBO;
  glGenBuffers(1, &VBO);
  // 使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中
  // 第四个参数指定了我们希望显卡如何管理给定的数据
  // GL_STATIC_DRAW ：数据不会或几乎不会改变。
  // GL_DYNAMIC_DRAW：数据会被改变很多。
  // GL_STREAM_DRAW ：数据每次绘制时都会改变。
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


  // 创建一个着色器对象，注意还是用ID来引用的。
  // 用glCreateShader创建这个着色器
  // 把需要创建的着色器类型(顶点着色器)以参数形式提供给glCreateShader
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  // 把这个着色器源码附加到着色器对象上，然后编译它
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  test_shader_success(vertex_shader);
  // 编译片段着色器
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  test_shader_success(fragment_shader);

  // 创建一个程序，并返回新创建程序对象的ID引用
  GLuint shader_program = glCreateProgram();
  // 把之前编译的着色器附加到程序对象上
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  // 用glLinkProgram链接它们
  glLinkProgram(shader_program);
  // test success
  // glGetProgramiv glGetProgramInfoLog


  // 激活这个程序对象
  glUseProgram(shader_program);
  // 删除着色器对象
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // 创建一个VAO
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  // 1. 绑定VAO
  glBindVertexArray(VAO);
  // 2. 把顶点数组复制到缓冲中供OpenGL使用
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 3. 设置顶点属性指针
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glViewport(0, 0, 800, 600);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);
    
    glClear(GL_COLOR_BUFFER_BIT);
    // 4. 绘制物体
    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);


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


void test_shader_success(GLuint id) {
  GLint isSuccess;
  char info_log[512];
  glGetShaderiv(id, GL_COMPILE_STATUS, &isSuccess);
  if (!isSuccess) {
    glGetShaderInfoLog(id, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::VERTEX::COMPIATION_FAILED\n" << info_log << std::endl;
    exit_program(-1);
  }
}

void exit_program(int exit_code) {
  glfwTerminate();
  exit(exit_code);
}