#include <iostream>           // basic input output
#include <memory>             // for make_shared

#include <homework8.hpp>      // header file
#include <opengl_helper.hpp>  // helper library

// glm library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
size_t total_points = 1000;

// global values
GLuint points_vao[3];
GLuint plane_vao[3];
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
    ImGui::InputFloat3("First  Point", mouse_points.get() + kEachPointsSize * 0);
    ImGui::InputFloat3("Second Point", mouse_points.get() + kEachPointsSize * 1);
    ImGui::InputFloat3("Third  Point", mouse_points.get() + kEachPointsSize * 2);
    ImGui::InputFloat3("Fourth Point", mouse_points.get() + kEachPointsSize * 4);
    ImGui::End();
  };


  //
  // main part
  //

  // initial opengl
  helper::InitialOpenGL(initial_window, window);
  // enable depth
  glEnable(GL_DEPTH_TEST);

  // create shader program
  GLuint simple_shader_program = 
      helper::CreatProgramWithShader("../resources/shaders/model.vs.glsl",
                                     "../resources/shaders/model.fs.glsl");

  // vertices
  std::vector<GLfloat> plane_vertices{
      // Positions          // Normals         // Texture Coords
      25.0f,  -0.5f, 25.0f,  0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
      -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f,  25.0f,
      -25.0f, -0.5f, 25.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f,

      25.0f,  -0.5f, 25.0f,  0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
      25.0f,  -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
      -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f,  25.0f };
  // vaos
  auto set_plane =[&plane_vertices](GLuint VAO, GLuint VBO, GLuint EBO) {
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * plane_vertices.size(),
                 plane_vertices.data(), GL_STATIC_DRAW);
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
  };
  helper::SetVAO(plane_vao[0], plane_vao[1], plane_vao[2], set_plane);

  // main part
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    ProcessInput(window);
    glfwGetWindowSize(window, &width, &height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    // also clear the depth buffer now!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplGlfwGL3_NewFrame();
    create_imgui();

    

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
    }
  }
}