#include <iostream>

#include <homework3.hpp>
#include <opengl_helper.hpp>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

std::vector<GLfloat> vertexs {0.0, 0.5, 0.0, -0.5, 0.0, 0.0, 0.5, 0.0, 0.0};
// GLfloat vertex_array[] = {1.0, 2.0, 1.0, 2.0, 1.0, 2.0};

int main() {
  GLFWwindow* window;
  int width = 800, height = 600;
  bool show_circle = false;
  float point_spacing = 0.01;
  GLuint point_vao[3], Circle[3];

  auto initial_window = [&] {
    window = glfwCreateWindow(width, height, "homework3", NULL, NULL);
    helper::assert_true(window != NULL, "Failed to create GLFW windows");
    glfwMakeContextCurrent(window);
  };

  auto update_point_vao = []() {
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexs.size(), vertexs.data(), GL_STREAM_DRAW);
  };

  auto set_point_vao = [update_point_vao](GLuint VAO, GLuint VBO,
                                                        GLuint EBO) {
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexs.size(), vertexs.data(), GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    update_point_vao();
    glPointSize(10);
  };

  helper::initial_opengl(initial_window, window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  GLuint simple_shader_program = helper::create_program_with_shader(
      "../shader/simple.vs.glsl", "../shader/simple.fs.glsl");
  helper::set_vao(point_vao[0], point_vao[1], point_vao[2],
                  set_point_vao);


  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    process_input(window);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplGlfwGL3_NewFrame();
    ImGui::Begin("Menu");
    ImGui::Text("Welcome");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Checkbox("show circle", &show_circle);
    if (!show_circle) {
      ImGui::SliderFloat2("Vertex 1", vertexs.data(), -1, 1);
      ImGui::SliderFloat2("Vertex 2", vertexs.data() + 3, -1, 1);
      ImGui::SliderFloat2("Vertex 3", vertexs.data() + 6, -1, 1);
    } else {
      ImGui::SliderFloat2("certer 1", vertexs.data(), -1, 1);
    }
    ImGui::SliderFloat("Point Space", &point_spacing, 0, 1);

    ImGui::End();

    glfwGetWindowSize(window, &width, &height);
    glUseProgram(simple_shader_program);
    // glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels); //draw pixel
    glBindVertexArray(point_vao[0]);
    update_point_vao();
    glDrawArrays(GL_POINTS, 0, 3);
    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  helper::exit_program();
}

void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
