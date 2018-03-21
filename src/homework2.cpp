#include <iostream>

#include <homework2.hpp>
#include <opengl_helper.hpp>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

GLfloat vertices[] = {
    // position         // color
    0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f,  // top
    -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  // left
    0.5f,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f,  // right
    0.0f,  -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,  // down
    -1.0f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f,  // rop left
    1.0f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f,  // top right
};

GLuint indices1[] = {0, 1, 2};
GLuint indices2[] = {1, 2, 3, 0, 1, 4, 0, 2, 5};

int main() {
  GLFWwindow* window;

  auto initial_window = [&window] {
    window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);
    helper::assert_true(window != NULL, "Failed to create GLFW windows");
    glfwMakeContextCurrent(window);
  };

  auto update_triangle_color = []() {
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  };

  auto set_simpile_triangle = [update_triangle_color](GLuint VAO, GLuint VBO,
                                                      GLuint EBO) {
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    update_triangle_color();
  };

  auto set_vao_2 = [](GLuint VAO, GLuint VBO, GLuint EBO) {
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  };

  helper::initial_opengl(initial_window, window);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  GLuint simple_shader_program = helper::create_program_with_shader(
      "../shader/simple.vs.glsl", "../shader/simple.fs.glsl");
  GLuint colorful_shader_program = helper::create_program_with_shader(
      "../shader/colorful.vs.glsl", "../shader/colorful.fs.glsl");
  GLuint simpile_triangle[3];
  helper::set_vao(simpile_triangle[0], simpile_triangle[1], simpile_triangle[2],
                  set_simpile_triangle);

  GLuint VAO2, VBO2, EBO2;
  helper::set_vao(VAO2, VBO2, EBO2, set_vao_2);

  bool show_another_triangles = false, make_triangles_colorful = false,
       show_points_and_lines = false;

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    process_input(window);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplGlfwGL3_NewFrame();
    ImGui::Begin("Menu");
    ImGui::Text("Welcome");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Checkbox("make_triangles_colorful", &make_triangles_colorful);
    if (make_triangles_colorful) {
      ImGui::ColorEdit3("top color", vertices + 3);
      ImGui::ColorEdit3("left color", vertices + 9);
      ImGui::ColorEdit3("right color", vertices + 15);
    }
    ImGui::Checkbox("show_another_triangles", &show_another_triangles);
    ImGui::Checkbox("show_points_and_lines", &show_points_and_lines);
    ImGui::End();

    GLuint shader_program = make_triangles_colorful ? colorful_shader_program
                                                    : simple_shader_program;

    glUseProgram(shader_program);
    glBindVertexArray(simpile_triangle[0]);
    if (make_triangles_colorful) update_triangle_color();
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    if (show_another_triangles) {
      glUseProgram(shader_program);
      glBindVertexArray(VAO2);
      // glDrawArrays(GL_TRIANGLES, 0, 6);
      glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    }

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