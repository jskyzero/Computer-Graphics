#include <iostream>

#include <homework3.hpp>
#include <opengl_helper.hpp>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

int main() {
  GLFWwindow* window;
  GLuint point_vao[3];
  std::vector<GLfloat> vertexs{
      0.0,  0.5, 0.0,  // top
      -0.5, 0.0, 0.0,  // left
      0.5,  0.0, 0.0   // down
  };
  std::vector<GLfloat> points{};

  int width = 800, height = 600;
  bool show_circle = false, fill = false;
  float point_spacing = 0.001, radius = 0.5;

  auto initial_window = [&] {
    window = glfwCreateWindow(width, height, "homework3", NULL, NULL);
    helper::assert_true(window != NULL, "Failed to create GLFW windows");
    glfwMakeContextCurrent(window);
  };

  auto update_point_vao = [&points]() {
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points.size(),
                 points.data(), GL_STREAM_DRAW);
  };

  auto set_point_vao = [update_point_vao](GLuint VAO, GLuint VBO, GLuint EBO) {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    update_point_vao();
  };

  auto create_imgui = [&]() {
    ImGui::Begin("Menu");
    ImGui::Text("Welcome");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Checkbox("show circle", &show_circle);
    ImGui::Checkbox("fill color", &fill);
    if (!show_circle) {
      ImGui::SliderFloat2("Vertex 1", vertexs.data(), -1, 1);
      ImGui::SliderFloat2("Vertex 2", vertexs.data() + 3, -1, 1);
      ImGui::SliderFloat2("Vertex 3", vertexs.data() + 6, -1, 1);
    } else {
      ImGui::SliderFloat2("Certer 1", vertexs.data(), -1, 1);
      ImGui::SliderFloat("Radius", &radius, 0.001, 1);
    }
    ImGui::SliderFloat("Point Space", &point_spacing, 0.001, 0.1);
    ImGui::End();
  };

  helper::initial_opengl(initial_window, window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  GLuint simple_shader_program = helper::create_program_with_shader(
      "../shader/simple.vs.glsl", "../shader/simple.fs.glsl");
  helper::set_vao(point_vao[0], point_vao[1], point_vao[2], set_point_vao);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    process_input(window);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplGlfwGL3_NewFrame();
    create_imgui();

    glfwGetWindowSize(window, &width, &height);

    // clear last time points
    points.clear();
    glBindVertexArray(point_vao[0]);
    glUseProgram(simple_shader_program);
    if (!show_circle) {
      if (fill) fill_triangle(points, vertexs.data(), vertexs.data() + 3, vertexs.data() + 6, point_spacing);
      else {
        plot_line(points, vertexs.data(), vertexs.data() + 3, point_spacing);
        plot_line(points, vertexs.data(), vertexs.data() + 6, point_spacing);
        plot_line(points, vertexs.data() + 3, vertexs.data() + 6, point_spacing);
      }
      
    } else {
      if (fill) fill_circle(points, vertexs.data(), radius, point_spacing);
      else plot_circle(points, vertexs.data(), radius, point_spacing);
    }
    update_point_vao();

    glPointSize(point_spacing * std::min(width, height));
    glDrawArrays(GL_POINTS, 0, points.size() / 3);

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  helper::exit_program();
}