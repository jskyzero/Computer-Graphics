#include <iostream>  // basic input output
#include <memory>    // for make_shared

#include <homework7.hpp>      // header file
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
bool can_mouse_move_camera = false;
// timing
// time between current frame and last frame
float delta_time = 0.0f;
float last_frame = 0.0f;
// ImGui
glm::vec3 light_position(0.4f, -0.5f, 0.8f);

// global values
GLuint plane_vao[3];


int main() {
  // the GLFW window
  GLFWwindow* window;
  
  // store window size
  int width = kScreenWidth, height = kScreenHeight;
  
  // dirty work initial
  auto initial_window = [&window, width, height] {
    window = glfwCreateWindow(width, height, "homework7", NULL, NULL);
    helper::assert_true(window != NULL, "Failed to create GLFW windows");
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
  };

  // imgui
  auto create_imgui = []() {
    ImGui::Begin("Menu");
    ImGui::Text("Welcome");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::SliderFloat("light x", &light_position[0], -1, 1);
    ImGui::SliderFloat("light y", &light_position[1], -1, 1);
    ImGui::SliderFloat("light z", &light_position[2], -1, 1);
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
  GLuint debug_shader_program = helper::CreatProgramWithShader(
      "../resources/shaders/debug_quad.vs.glsl",
      "../resources/shaders/debug_quad_depth.fs.glsl");
  GLuint depth_shader_program = 
      helper::CreatProgramWithShader("../resources/shaders/depth.vs.glsl",
                                     "../resources/shaders/depth.fs.glsl");
  GLuint simple_shader_program = 
      helper::CreatProgramWithShader("../resources/shaders/model.vs.glsl",
                                     "../resources/shaders/model.fs.glsl");
  // create texture
  GLuint eye_texture, box_texture;
  helper::CreateTexture(eye_texture, "../resources/textures/eye.jpg");
  helper::CreateTexture(box_texture, "../resources/textures/box_texture.jpeg");

  std::vector<GLfloat> plane_vertices{
      // Positions          // Normals         // Texture Coords
      25.0f,  -0.5f, 25.0f,  0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
      -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f,  25.0f,
      -25.0f, -0.5f, 25.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f,

      25.0f,  -0.5f, 25.0f,  0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
      25.0f,  -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
      -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f,  25.0f };
  
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

  // Configure depth map FBO
  const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
  GLuint depthMapFBO;
  glGenFramebuffers(1, &depthMapFBO);
  // - Create depth texture
  GLuint depthMap;
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    update_delta();
    ProcessInput(window);
    glfwGetWindowSize(window, &width, &height);
    // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    // also clear the depth buffer now!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplGlfwGL3_NewFrame();
    create_imgui();

    glUseProgram(simple_shader_program);
    RenderScene(simple_shader_program);
    // // 1. Render depth of scene to texture (from light's perspective)
    // // - Get light projection/view matrix.
    // glm::mat4 lightProjection, lightView;
    // glm::mat4 lightSpaceMatrix;
    // GLfloat near_plane = 1.0f, far_plane = 7.5f;
    // lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    // lightView = glm::lookAt(light_position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    // lightSpaceMatrix = lightProjection * lightView;
    // // - render scene from light's point of view
    // glUseProgram(depth_shader_program);
    // glUniformMatrix4fv(glGetUniformLocation(depth_shader_program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    // glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    // glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    //   glClear(GL_DEPTH_BUFFER_BIT);
    //   RenderScene(depth_shader_program);
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // // Reset viewport
    // glViewport(0, 0, width, height);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // // Render Depth map to quad
    // glUseProgram(debug_shader_program);
    // glUniform1f(glGetUniformLocation(debug_shader_program, "near_plane"), near_plane);
    // glUniform1f(glGetUniformLocation(debug_shader_program, "far_plane"), far_plane);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, depthMap);
    // RenderQuad();


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

void RenderScene(GLuint &shader_program) {
  // Floor
  glm::mat4 model = glm::mat4(1.0f);
  glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE,
                     glm::value_ptr(model));
  glBindVertexArray(plane_vao[0]);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
  // Cubes
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
  glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE,
                     glm::value_ptr(model));
  RenderCube();
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
  glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE,
                     glm::value_ptr(model));
  RenderCube();
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0f));
  model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
  model = glm::scale(model, glm::vec3(0.5));
  glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE,
                     glm::value_ptr(model));
  RenderCube();
}

// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color
// targets and post-processing effects.
GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad() {
  if (quadVAO == 0) {
    GLfloat quadVertices[] = {
        // Positions        // Texture Coords
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // Setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          (GLvoid*)(3 * sizeof(GLfloat)));
  }
  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}

// RenderCube() Renders a 1x1 3D cube in NDC.
GLuint cubeVAO = 0;
GLuint cubeVBO = 0;
void RenderCube() {
  // Initialize (if necessary)
  if (cubeVAO == 0) {
    GLfloat vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // Bottom-left
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top-right
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,   // bottom-right
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top-right
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,   // top-left
        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // bottom-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top-right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // top-left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        // Left face
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-right
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-right
        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-right
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top-right
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,   // top-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // bottom-left
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top-right
        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,    // bottom-right
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,    // bottom-right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f    // bottom-left
    };
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    // Fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Link vertex attributes
    glBindVertexArray(cubeVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (GLvoid*)(6 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
  // Render Cube
  glBindVertexArray(cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}