#include <hello_window.hpp>

int main() {
  glfwInit();
  // 调用glfwInit函数来初始化GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  //创建一个窗口对象，这个窗口对象存放了所有和窗口相关的数据，而且会被GLFW的其他函数频繁地用到
  GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
  // if error
  if (window == NULL) {
    std::cout << "Failed to create GLFW windows" << std::endl;
    glfwTerminate();
    return -1;
  }

  std::cout << "Success to create GLFW windows" << std::endl;
  glfwMakeContextCurrent(window);

  // GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  // 告诉OpenGL渲染窗口的尺寸大小
  glViewport(0, 0, 800, 600);
  // 对窗口注册一个回调函数(Callback Function)，它会在每次窗口大小被调整的时候被调用
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // 渲染循环(Render Loop)，它能在我们让GLFW退出前一直保持运行
  while (!glfwWindowShouldClose(window)) {
    // 输入
    process_input(window);
    // 渲染指令
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // 交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲）
    glfwSwapBuffers(window);
    // 检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，
    // 并调用对应的回调函数（可以通过回调方法手动设置）
    glfwPollEvents();
  }
  // 正确释放/删除之前的分配的所有资源
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
  // 如果没有按下，glfwGetKey将会返回GLFW_RELEASE
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
