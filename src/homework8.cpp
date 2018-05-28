#include <iostream>           // basic input output
#include <memory>             // for make_shared

#include <homework8.hpp>      // header file
#include <opengl_helper.hpp>  // helper library

// glm library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DEBUG

// screen size
constexpr unsigned int kScreenWidth = 800;
constexpr unsigned int kScreenHeight = 600;
// one has vec3 and total  4 points
constexpr size_t kEachPointsSize = 3;
constexpr size_t kTotalPointsNum = 4;
constexpr size_t kMoursePointsSize = kEachPointsSize * kTotalPointsNum;

// Mouse Input
GLfloat mouse_x, mouse_y;
size_t points_count = 0;
std::unique_ptr<GLfloat[]> mouse_points(new GLfloat[kMoursePointsSize]);
// ImGui
int total_points = 1000;

// global values
GLuint points_vao[3];
// store window size
int width = kScreenWidth, height = kScreenHeight;


int main() {
  // the GLFW window
  GLFWwindow* window;
  
  // dirty work initial
  auto initial_window = [&window] {
    window = glfwCreateWindow(width, height, "homework8", NULL, NULL);
    helper::assert_true(window != NULL, "Failed to create GLFW windows");
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
  };

  // imgui
  auto create_imgui = []() {
    ImGui::Begin("Menu");
    ImGui::Text("Welcome");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::InputInt("Total Points", &total_points);
    ImGui::InputFloat3("First  Point", mouse_points.get() + kEachPointsSize * 0);
    ImGui::InputFloat3("Second Point", mouse_points.get() + kEachPointsSize * 1);
    ImGui::InputFloat3("Third  Point", mouse_points.get() + kEachPointsSize * 2);
    ImGui::InputFloat3("Fourth Point", mouse_points.get() + kEachPointsSize * 3);
    ImGui::End();
  };


  //
  // main part
  //

  // initial opengl
  helper::InitialOpenGL(initial_window, window);
  // enable depth
  glEnable(GL_DEPTH_TEST);

  glfwSetMouseButtonCallback(window, MouseButtonCallback);

  // create shader program
  GLuint simple_shader_program = 
      helper::CreatProgramWithShader("../resources/shaders/simple.vs.glsl",
                                     "../resources/shaders/simple.fs.glsl");

  // vertices
  std::vector<GLfloat> vertices{};

  auto bezier_points = [](int i) {
    float t = float(i) / total_points;
    float b0 = pow(1 - t, 3.0);
    float b1 = 3.0 * t * pow(1 - t, 2.0);
    float b2 = 3.0 * t * t * (1 - t);
    float b3 = t * t * t;
    float x = mouse_points.get()[kEachPointsSize * 0] * b0 +
              mouse_points.get()[kEachPointsSize * 1] * b1 + 
              mouse_points.get()[kEachPointsSize * 2] * b2 + 
              mouse_points.get()[kEachPointsSize * 3] * b3;
    float y = mouse_points.get()[kEachPointsSize * 0 + 1] * b0 +
              mouse_points.get()[kEachPointsSize * 1 + 1] * b1 +
              mouse_points.get()[kEachPointsSize * 2 + 1] * b2 +
              mouse_points.get()[kEachPointsSize * 3 + 1] * b3;
    float z = 0.0;
    return std::vector<GLfloat> {x, y, z};
  };


  auto initial_vertices = [&vertices, bezier_points]() {
    // clear
    vertices.clear();

    for (int i = 0; i < kTotalPointsNum; i++) {
      vertices.insert(vertices.end(), mouse_points.get() + kEachPointsSize * i, mouse_points.get() + kEachPointsSize * (i + 1));
    }

    for (int i = 0; i < total_points; i++) {
      auto each_point = bezier_points(i);
      vertices.insert(vertices.end(), each_point.begin(), each_point.end());
    }
  };


  // vaos
  auto set_plane =[&vertices, initial_vertices](GLuint VAO, GLuint VBO, GLuint EBO) {
    initial_vertices();
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(),
                 vertices.data(), GL_STATIC_DRAW);
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  };


  // main part
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    ProcessInput(window);
    glfwGetWindowSize(window, &width, &height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplGlfwGL3_NewFrame();
    create_imgui();

    // can draw line
    if (points_count == kTotalPointsNum) {
      glUseProgram(simple_shader_program);
      helper::SetVAO(points_vao[0], points_vao[1], points_vao[2], set_plane);
      glDrawArrays(GL_LINES, 0, 2);
      glDrawArrays(GL_LINES, 1, 2);
      glDrawArrays(GL_LINES, 2, 2);
      glDrawArrays(GL_POINTS, kTotalPointsNum, total_points + kTotalPointsNum);
    }

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  helper::exit_program();
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void ProcessInput(GLFWwindow* window) {
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  // pass
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
  mouse_x = xpos;
  mouse_y = ypos;
}


void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
    if (points_count >= 0 && points_count < kTotalPointsNum)
    {
      mouse_points.get()[points_count * kEachPointsSize + 0] = float(mouse_x - (width / 2)) / (width / 2);
      mouse_points.get()[points_count * kEachPointsSize + 1] = float((height / 2) - mouse_y) / (height / 2);
      mouse_points.get()[points_count * kEachPointsSize + 2] = 0.0;
      points_count++;
#ifdef DEBUG
      std::cout << mouse_points.get()[points_count * kEachPointsSize + 0] << "  " 
                << mouse_points.get()[points_count * kEachPointsSize + 1] << std::endl;
#endif
    }
  }
}