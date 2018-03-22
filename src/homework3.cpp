#include <iostream>

#include <homework3.hpp>
#include <opengl_helper.hpp>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"


int main() {
  GLFWwindow* window;
  int width = 800, height = 600;

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

    ImGui::End();


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