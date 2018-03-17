#ifndef __HELLO_TRIANGLE__
#define __HELLO_TRIANGLE__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void process_input(GLFWwindow* window);

void test_shader_success(GLuint id);

void exit_program(int exit_code);

#endif