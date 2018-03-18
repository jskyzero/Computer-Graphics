#ifndef __HW_TRIANGLE__
#define __HW_TRIANGLE__

#include <cstdlib>

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void process_input(GLFWwindow* window);

void test_shader_success(GLuint id, int type,  PFNGLGETSHADERIVPROC f, PFNGLGETSHADERINFOLOGPROC f2);

void test_error(bool isOK, std::string message);

void exit_program(int exit_code, std::string message);

void exit_program(int exit_code);

#endif