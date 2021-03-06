#include <iostream>

#include <homework4.hpp>
#include <opengl_helper.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

int main() {
  GLFWwindow* window;
  GLuint point_vao[3];
  int width = 800, height = 600;
  float input_values[9]{0.0f};
  std::vector<GLfloat> vertices{
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
  // std::vector<glm::vec3> cubePositions{
  //     glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
  //     glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
  //     glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
  //     glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
  //     glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  auto initial_window = [&window, width, height] {
    window = glfwCreateWindow(width, height, "homework4", NULL, NULL);
    helper::assert_true(window != NULL, "Failed to create GLFW windows");
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  };

  auto update_point_vao = []() {};

  auto set_point_vao = [update_point_vao, &vertices](GLuint VAO, GLuint VBO,
                                                     GLuint EBO) {
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(),
                 vertices.data(), GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    update_point_vao();
  };

  auto create_imgui = [&]() {
    ImGui::Begin("Menu");
    ImGui::Text("Welcome");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::SliderFloat2("translate", input_values, -1.0f, 1.0f);
    ImGui::SliderFloat("scaling", input_values + 2, -1.0f, 1.0f);
    ImGui::SliderFloat3("rotate", input_values + 3, -180.0f, 180.0f);
    ImGui::End();
  };

  helper::initial_opengl(initial_window, window);
  // enable depth
  glEnable(GL_DEPTH_TEST);

  GLuint coordinate_systems_shader_program = helper::create_program_with_shader(
      "../resources/shaders/coordinate_systems.vs.glsl",
      "../resources/shaders/coordinate_systems.fs.glsl");
  GLuint simple_shader_program =
      helper::create_program_with_shader("../resources/shaders/simple.vs.glsl",
                                         "../resources/shaders/simple.fs.glsl");
  GLuint eye_texture, box_texture;
  helper::create_texture(eye_texture, "../resources/textures/eye.jpg");
  helper::create_texture(box_texture, "../resources/textures/box_texture.jpeg");

  glUseProgram(coordinate_systems_shader_program);
  helper::set_shader_int(coordinate_systems_shader_program, "texture1", 0);
  helper::set_shader_int(coordinate_systems_shader_program, "texture2", 1);
  helper::set_vao(point_vao[0], point_vao[1], point_vao[2], set_point_vao);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    process_input(window);
    glfwGetWindowSize(window, &width, &height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // also clear the depth buffer now!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplGlfwGL3_NewFrame();
    create_imgui();

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, box_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, eye_texture);
    glUseProgram(coordinate_systems_shader_program);

    // create transformations
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(
        model, glm::vec3(input_values[0], input_values[1], input_values[2]));
    model = glm::rotate(model, glm::radians(input_values[3]), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(input_values[4]), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(input_values[5]), glm::vec3(0.0f, 0.0f, 1.0f));

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)width / (float)height, 0.1f, 100.0f);
    // pass transformation matrices to the shader
    helper::set_shader_mat4(coordinate_systems_shader_program, "projection",
                            projection);
    helper::set_shader_mat4(coordinate_systems_shader_program, "view", view);
    helper::set_shader_mat4(coordinate_systems_shader_program, "model", model);

    // render boxes
    glBindVertexArray(point_vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  helper::exit_program();
}