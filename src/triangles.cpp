/*
 * trangles.cpp
 *
 * the first code in Opengl Programming Guide
 * 为了方便表述，接下来还是使用中文注释，上面那句话的意思就是，这是我们写的最初的代码。
 *
 */

#include <iostream>
// 这是自己写的一个OpenGL的辅助函数库，大概就是封装了很多有用或者能用上的函数。
#include <opengl_helper.hpp>


// 缓存偏移
#define BUFFER_OFFSET(offset) ((void *)(offset))

// 还请注意，C++的枚举型变量默认从0开始，所以最后一个的值刚好就是大小。
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffers, NumBuffers };
enum Attrib_IDs { vPosition = 0 };
// VAO数组
GLuint VAOs[NumVAOs];
// 缓存数组
GLuint Buffers[NumBuffers];
// 顶点个数
const GLuint kNumVertices = 6;


void init(void) {
  static const GLfloat vertices[kNumVertices][2] = {
    { -0.90f, -0.90f }, // 三角形 其一
    {  0.85f, -0.90f },
    { -0.90f,  0.85f },
    {  0.90f, -0.85f }, // 三角形 其二
    {  0.90f,  0.90f },
    { -0.85f,  0.90f },
  };
  std::cout << "isOK" << std::endl;
  // 创建buffer
  glCreateBuffers(NumBuffers, Buffers);
  std::cout << "isOK" << std::endl;
  
  // 转移数据
  glNamedBufferStorage(Buffers[ArrayBuffers], sizeof(vertices), vertices, 0);
  // 加载shader
  GLuint program = helper::CreatProgramWithShader(
    "../resources/shaders/triangles.vert.glsl",
    "../resources/shaders/triangles.frag.glsl");
  glUseProgram(program);
  // 设置VAO相关
  glGenVertexArrays(NumVAOs, VAOs);
  glBindVertexArray(VAOs[Triangles]);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffers]);
  // 设置shader参数属性
  glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);
}

void display(void) {
  // clear color
  static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  glClearBufferfv(GL_COLOR, 0, black);
  // draw triangle
  glBindVertexArray(VAOs[Triangles]);
  glDrawArrays(GL_TRIANGLES, 0, kNumVertices);
}

static void glfwError(int id, const char* description)
{
  std::cout << description << std::endl;
}



int main() {
  glfwSetErrorCallback(&glfwError);
  helper::assert_true(glfwInit() == GLFW_TRUE, "glfw initial error");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  
  // create window
  GLFWwindow * window = glfwCreateWindow(640, 480, "Triangles", NULL, NULL);
  helper::assert_true(window != NULL, "Failed to create GLFW windows");
  // set context
  glfwMakeContextCurrent(window);
  // glad initial
  helper::initial_glad();

  // initial
  init();
  
  // main loop
  while (!glfwWindowShouldClose(window)) {
    display();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // exit
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}