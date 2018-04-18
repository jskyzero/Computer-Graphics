#include <iostream>  // basic input output
#include <memory>    // for make_shared

#include <homework6.hpp>      // header file
#include <opengl_helper.hpp>  // helper library
// glm library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr unsigned int kScreenWidth = 800;
constexpr unsigned int kScreenHeight = 600;


// camera
auto camera = std::make_shared<helper::Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
// make mouse initial position center
float last_x = kScreenWidth / 2.0f;
float last_y = kScreenHeight / 2.0f;
bool first_move_mouse = true;
// timing
// time between current frame and last frame
float delta_time = 0.0f;
float last_frame = 0.0f;
// ImGui
bool can_mouse_move_camera = false;
glm::vec3 light_position(0.8f, 0.8f, 0.8f);


int main() {
  // the GLFW window
  GLFWwindow* window;
  // save the vao vbo eao
  GLuint box[3], light[3];
  // store window size
  int width = kScreenWidth, height = kScreenHeight;

  // basic v
  std::vector<GLfloat> v{
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,

      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

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

  auto add_normal_verctor = [](std::vector<GLfloat>& v, int n) {
    glm::vec3 sides[3] = {glm::vec3()};
    for (int i = 0; 3 * (n + 3) * i < v.size(); i++) {
      auto index = (i * (3 * (n + 3)));
      auto p = v.data() + index;
      for (int j = 0; j < 3; j++) {
        sides[j] = glm::vec3(p[n * j + 0], p[n * j + 1], p[n * j + 2]);
      }
      auto normal =
          glm::normalize(glm::cross(sides[2] - sides[1], sides[0] - sides[1]));
      std::cout << normal[0] << " " << normal[1] << " " << normal[2]
                << std::endl;
      std::vector<GLfloat> normal_v{normal[0], normal[1], normal[2]};
      for (int j = 0; j < 3; j++) {
        v.insert(v.begin() + (index + (n + 3) * (j + 1) - 3), normal_v.begin(),
                 normal_v.end());
      }
    }
  };
  // dirty work initial
  auto initial_window = [&window, width, height] {
    window = glfwCreateWindow(width, height, "homework6", NULL, NULL);
    helper::assert_true(window != NULL, "Failed to create GLFW windows");
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
  };
  // if we need update vao's vbo / eao
  auto update_box = []() {};
  // initial vao
  auto set_box = [update_box, &v](GLuint VAO, GLuint VBO,
                                                     GLuint EBO) {
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * v.size(),
                 v.data(), GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (void*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    update_box();
  };

  auto set_light = [&v](GLuint VAO, GLuint VBO, GLuint EBO) {
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * v.size(),
                 v.data(), GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (void*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
  };

  // imgui
  auto create_imgui = []() {
    ImGui::Begin("Menu");
    ImGui::Text("Welcome");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  };

  auto update_delta = []() {
    float currentFrame = (float)glfwGetTime();
    delta_time = currentFrame - last_frame;
    last_frame = currentFrame;
  };

  //
  // main part
  //

  helper::InitialOpenGL(initial_window, window);
  // enable depth
  glEnable(GL_DEPTH_TEST);

  // create shader program
  GLuint coordinate_systems_shader_program = helper::CreatProgramWithShader(
      "../resources/shaders/coordinate_systems.vs.glsl",
      "../resources/shaders/coordinate_systems.fs.glsl");
  GLuint light_shader_program =
      helper::CreatProgramWithShader("../resources/shaders/light.vs.glsl",
                                     "../resources/shaders/light.fs.glsl");
  GLuint simple_shader_program =
      helper::CreatProgramWithShader("../resources/shaders/simple.vs.glsl",
                                     "../resources/shaders/simple.fs.glsl");
  // create texture
  GLuint eye_texture, box_texture;
  helper::CreateTexture(eye_texture, "../resources/textures/eye.jpg");
  helper::CreateTexture(box_texture, "../resources/textures/box_texture.jpeg");

  add_normal_verctor(v, 5);
  // set vao
  helper::SetVAO(box[0], box[1], box[2], set_box);
  helper::SetVAO(light[0], light[1], light[2], set_light, false);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    update_delta();
    ProcessInput(window);
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

    // create transformations
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    // set shader
    glUseProgram(coordinate_systems_shader_program);
    // set texture
    helper::SetShaderInt(coordinate_systems_shader_program, "texture1", 0);
    helper::SetShaderInt(coordinate_systems_shader_program, "texture2", 1);

    model = glm::mat4(1.0f);
    model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    projection = glm::perspective(glm::radians(camera->Zoom),
                                  (float)kScreenWidth / (float)kScreenHeight,
                                  0.1f, 100.0f);
    // camera/view transformation
    view = camera->GetViewMatrix();
    // pass transformation matrices to the shader
    helper::SetShaderMat4(coordinate_systems_shader_program, "projection",
                          projection);
    helper::SetShaderMat4(coordinate_systems_shader_program, "view", view);
    helper::SetShaderMat4(coordinate_systems_shader_program, "model", model);
    helper::SetShaderVec3(coordinate_systems_shader_program, "objectColor", 1.0f, 0.5f, 0.31f);
    helper::SetShaderVec3(coordinate_systems_shader_program, "lightColor",  1.0f, 1.0f, 1.0f);
    // render boxes
    glBindVertexArray(box[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glUseProgram(light_shader_program);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);
    model = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera->Zoom),
                                  (float)kScreenWidth / (float)kScreenHeight,
                                  0.1f, 100.0f);
    // camera/view transformation
    view = camera->GetViewMatrix();
    model = glm::translate(model, light_position);
    model = glm::scale(model, glm::vec3(0.2f));
    helper::SetShaderMat4(light_shader_program, "projection",
                          projection);
    helper::SetShaderMat4(light_shader_program, "view", view);
    helper::SetShaderMat4(light_shader_program, "model", model);

    // render boxes
    glBindVertexArray(light[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);


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
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera->ProcessKeyboard(helper::Camera::FORWARD, delta_time);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera->ProcessKeyboard(helper::Camera::BACKWARD, delta_time);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera->ProcessKeyboard(helper::Camera::LEFT, delta_time);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera->ProcessKeyboard(helper::Camera::RIGHT, delta_time);
  // if press left alt, then camera can move
  can_mouse_move_camera = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
  if (can_mouse_move_camera) {
    first_move_mouse = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
  if (!can_mouse_move_camera) return;

  if (first_move_mouse) {
    last_x = (float)xpos;
    last_y = (float)ypos;
    first_move_mouse = false;
  }

  float xoffset = (float)xpos - last_x;
  float yoffset =
      last_y -
      (float)ypos;  // reversed since y-coordinates go from bottom to top

  last_x = (float)xpos;
  last_y = (float)ypos;

  camera->ProcessMouseMovement(-xoffset, -yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  if (!can_mouse_move_camera) return;
  std::cout << yoffset << std::endl;
  camera->ProcessMouseScroll((float)yoffset);
}