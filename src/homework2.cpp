#include <homework2.hpp>
#include <iostream>
#include <opengl_helper.hpp>


GLfloat vertices[] = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};

GLuint indices1[] = { // 注意索引从0开始! 
    1, 2, 3  // 第二个三角形
};

GLuint indices2[] = { // 注意索引从0开始! 
    0, 1, 3, // 第一个三角形
};

int main() {
  helper::initial_glfw();
  GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);
  helper::assert_true(window != NULL, "Failed to create GLFW windows");
  glfwMakeContextCurrent(window);
  helper::initial_glad();

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  GLuint vertex_shader = helper::compile_vertex_shader("../shader/simpile.vs.glsl");
  GLuint fragment_shader = helper::compile_fragment_shader("../shader/simpile.fs.glsl");
  GLuint shader_program =
      helper::create_program_with_shader(vertex_shader, fragment_shader);

  GLuint VAO1, VBO1, EBO1;
  glGenVertexArrays(1, &VAO1);
  glGenBuffers(1, &VBO1);
  glGenBuffers(1, &EBO1);

  glBindVertexArray(VAO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);


  GLuint VAO2, VBO2, EBO2;
  glGenVertexArrays(1, &VAO2);
  glGenBuffers(1, &VBO2);
  glGenBuffers(1, &EBO2);

  glBindVertexArray(VAO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  glViewport(0, 0, 800, 600);

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