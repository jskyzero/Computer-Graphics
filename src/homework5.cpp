#include <iostream>  // basic input output

#include <homework5.hpp>      // header file
#include <opengl_helper.hpp>  // helper library
// glm library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// imgui library
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

constexpr glm::vec3 initial_position_k = glm::vec3(-1.5f, 0.5f, -1.5f);

int main() {
  // the GLFW window
  GLFWwindow* window;
  // save the vao vbo eao
  GLuint point_vao[3];
  // store window size
  int width = 800, height = 600;

  // basic vertices
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

  // dirty work initial
  auto initial_window = [&window, width, height] {
    window = glfwCreateWindow(width, height, "homework5", NULL, NULL);
    helper::assert_true(window != NULL, "Failed to create GLFW windows");
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  };
  // if we need update vao's vbo / eao
  auto update_point_vao = []() {};
  // initial vao
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
  // ImGui input values
  bool projection = true, orthographic_projection = true,
       perspective_projection = false, view_changing = false;
  // 多组(left, right, bottom, top, near, far)
  float orth_left = -2.0f, orth_right = 2.0f, orth_bottom = -2.0f,
        orth_top = 2.0f, orth_near = 0.1f, orth_far = 10.0f, pers_fov = 45.0,
        pers_near = 0.1f, pers_far = 10.0f;
  // imgui
  auto create_imgui = [&]() {
    ImGui::Begin("Menu");
    ImGui::Text("Welcome");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::CheckBox("projection", &projection)
    if (projection) {
      ImGui::CheckBox("projection", &orthographic_projection);
      ImGui::CheckBox("projection", &perspective_projection);
    }
    ImGui::CheckBox("projection", &view_changing)
    ImGui::End();
  };

  //
  // main part
  //

  helper::initial_opengl(initial_window, window);
  // enable depth
  glEnable(GL_DEPTH_TEST);

  // create shader program
  GLuint coordinate_systems_shader_program = helper::create_program_with_shader(
      "../resources/shaders/coordinate_systems.vs.glsl",
      "../resources/shaders/coordinate_systems.fs.glsl");
  GLuint simple_shader_program =
      helper::create_program_with_shader("../resources/shaders/simple.vs.glsl",
                                         "../resources/shaders/simple.fs.glsl");
  // create texture
  GLuint eye_texture, box_texture;
  helper::create_texture(eye_texture, "../resources/textures/eye.jpg");
  helper::create_texture(box_texture, "../resources/textures/box_texture.jpeg");
  // set shader
  glUseProgram(coordinate_systems_shader_program);
  // set texture
  helper::set_shader_int(coordinate_systems_shader_program, "texture1", 0);
  helper::set_shader_int(coordinate_systems_shader_program, "texture2", 1);
  // set vao
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
    // model = glm::translate(model, initial_position_k);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // projection = glm::perspective(glm::radians(45.0f),
    //                               (float)width / (float)height, 0.1f,
    //                               100.0f);
    // 多组(left, right, bottom, top, near, far)
    projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 10.0f);
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