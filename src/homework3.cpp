#include <iostream>

#include <homework3.hpp>
#include <opengl_helper.hpp>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"


GLfloat vertex_array[] = {1.0, 2.0, 1.0, 2.0, 1.0, 2.0};

int main() {
  GLFWwindow* window;
  int width = 800, height = 600;
  bool show_circle = false;
  float point_spacing = 0.01;

  auto initial_window = [&] {
    window = glfwCreateWindow(width, height, "homework3", NULL, NULL);
    helper::assert_true(window != NULL, "Failed to create GLFW windows");
    glfwMakeContextCurrent(window);
  };

  helper::initial_opengl(initial_window, window);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  GLuint simple_shader_program = helper::create_program_with_shader(
      "../shader/simple.vs.glsl", "../shader/simple.fs.glsl");


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
      ImGui::SliderFloat2("Vertex 1", vertex_array, -1, 1);
      ImGui::SliderFloat2("Vertex 2", vertex_array + 2, -1, 1);
      ImGui::SliderFloat2("Vertex 3", vertex_array + 4, -1, 1);
    } else {
      ImGui::SliderFloat2("certer 1", vertex_array, -1, 1);
    }
    ImGui::SliderFloat("Point Space", &point_spacing, 0, 1);

    ImGui::End();

    glfwGetWindowSize(window, &width, &height);
    // glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels); //draw pixel

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
