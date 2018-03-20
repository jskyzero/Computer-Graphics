#include <homework2.hpp>
#include <iostream>
#include <opengl_helper.hpp>

GLfloat vertices[] = {
    0.5f,  0.5f,  0.0f,  // 右上角
    0.5f,  -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f,  // 左下角
    -0.5f, 0.5f,  0.0f   // 左上角
};

GLuint indices1[] = {
    // 注意索引从0开始!
    1, 2, 3  // 第二个三角形
};

GLuint indices2[] = {
    // 注意索引从0开始!
    0, 1, 3,  // 第一个三角形
};

int main() {
  GLFWwindow* window;

  auto initial_window = [&window] {
    window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);
    helper::assert_true(window != NULL, "Failed to create GLFW windows");
    glfwMakeContextCurrent(window);
  };

  auto set_vao_1 = [](GLuint VAO,GLuint VBO,GLuint EBO) {
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1,
               GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (void*)0);
    glEnableVertexAttribArray(0);
  };

  auto set_vao_2 = [](GLuint VAO,GLuint VBO,GLuint EBO) {
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2,
               GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (void*)0);
    glEnableVertexAttribArray(0);
  };

  helper::initial_opengl(initial_window);


  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  GLuint shader_program = helper::create_program_with_shader(
      "../shader/simpile.vs.glsl", "../shader/simpile.fs.glsl");
  GLuint VAO1, VBO1, EBO1;
  
  helper::set_vao(VAO1, VBO1, EBO1, set_vao_1);

  GLuint VAO2, VBO2, EBO2;
  helper::set_vao(VAO2, VBO2, EBO2, set_vao_2);


  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program);
    glBindVertexArray(VAO1);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glUseProgram(shader_program);
    glBindVertexArray(VAO2);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    glfwPollEvents();
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