#ifndef __HOMEWORK3__
#define __HOMEWORK3__

#include <glad/glad.h>   // for glad
#include <GLFW/glfw3.h>  // for glfw

#include <vector>
#include <cmath>


void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void add_point(std::vector<GLfloat>& v, GLfloat x, GLfloat y) {
  v.insert(v.end(), x);
  v.insert(v.end(), y);
  v.insert(v.end(), 0);
}

void plot_line(std::vector<GLfloat>& points, GLfloat begin[], GLfloat end[],
               float point_spacing) {
  auto plot_line_low = [&](GLfloat begin[], GLfloat end[]) {
    GLfloat dx = end[0] - begin[0];
    GLfloat dy = end[1] - begin[1];
    GLfloat yi = point_spacing;

    if (dy < 0) {
      yi = -yi;
      dy = -dy;
    }
    GLfloat D = 2 * dy - dx;
    GLfloat x = begin[0];
    GLfloat y = begin[1];

    while (x < end[0]) {
      add_point(points, x, y);
      if (D > 0) {
        y = y + yi;
        D = D - 2 * dx;
      }
      D = D + 2 * dy;
      x = x + point_spacing;
    }
  };

  auto plot_line_high = [&](GLfloat begin[], GLfloat end[]) {
    GLfloat dx = end[0] - begin[0];
    GLfloat dy = end[1] - begin[1];
    GLfloat xi = point_spacing;

    if (dx < 0) {
      xi = -xi;
      dx = -dx;
    }
    GLfloat D = 2 * dx - dy;
    GLfloat x = begin[0];
    GLfloat y = begin[1];

    while (y < end[1]) {
      add_point(points, x, y);
      if (D > 0) {
        x = x + xi;
        D = D - 2 * dy;
      }
      D = D + 2 * dx;
      y = y + point_spacing;
    }
  };

  if (std::abs(end[1] - begin[1]) < std::abs(end[0] - begin[0])) {
    if (begin[0] > end[0]) {
      plot_line_low(end, begin);
    } else {
      plot_line_low(begin, end);
    }
  } else {
    if (begin[1] > end[1]) {
      plot_line_high(end, begin);
    } else {
      plot_line_high(begin, end);
    }
  }
}

  // // initial version use float
  // auto plot_line = [&](GLfloat begin[], GLfloat end[]) {
  //   float space = point_spacing;

  //   GLfloat delta_x = begin[0] - end[0];
  //   GLfloat delta_y = begin[1] - end[1];
  //   if (delta_x == 0) { // vertical

  //   } else {
      
  //     GLfloat delta_err = std::abs(delta_y / delta_x);
  //     GLfloat error = 0;

  //     GLfloat x = begin[0], y = begin[1];
  //     GLfloat change_x = begin[0] < end[0] ? space : -space;
  //     GLfloat change_y = begin[1] < end[1] ? space * delta_err : -space * delta_err;
  //     // std::cout << change_x << " " << change_y  << std::endl;

  //     for (int i = 0; i < (int)abs(delta_x / space); i++) {
  //       add_point(points, x , y);
  //       error = error + delta_err;
  //       while (error >= 0.5) {
  //         y += change_y;
  //         error -= 1.0;
  //       }
  //       // y += change_y;
  //       x += change_x;
  //     }
  //   }
  // };

#endif