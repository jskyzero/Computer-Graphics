#ifndef __HOMEWORK6__
#define __HOMEWORK6__

#include <glad/glad.h>   // for glad
#include <GLFW/glfw3.h>  // for glfw

#include <cmath>
#include <vector>
#include <algorithm>

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void ProcessInput(GLFWwindow *window);

#endif