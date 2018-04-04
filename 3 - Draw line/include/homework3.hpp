#ifndef __HOMEWORK3__
#define __HOMEWORK3__

#include <glad/glad.h>   // for glad
#include <GLFW/glfw3.h>  // for glfw

#include <cmath>
#include <vector>
#include <algorithm>

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

void plot_circle(std::vector<GLfloat>& points, GLfloat center[], float radius,
                 float point_spacing) {
  GLfloat x = radius - point_spacing;
  GLfloat y = 0;
  GLfloat dx = point_spacing;
  GLfloat dy = point_spacing;
  GLfloat err = dx - (radius * 2);

  while (x >= y) {
    add_point(points, center[0] + x, center[1] + y);
    add_point(points, center[0] + y, center[1] + x);
    add_point(points, center[0] - y, center[1] + x);
    add_point(points, center[0] - x, center[1] + y);
    add_point(points, center[0] - x, center[1] - y);
    add_point(points, center[0] - y, center[1] - x);
    add_point(points, center[0] + y, center[1] - x);
    add_point(points, center[0] + x, center[1] - y);

    if (err <= 0) {
      y += point_spacing;
      err += dy;
      dy += 2 * point_spacing;
    }
    if (err > 0) {
      x -= point_spacing;
      dx += 2 * point_spacing;
      err += dx - (radius * 2);
    }
  }
}

void fill_triangle(std::vector<GLfloat>& points, GLfloat A[], GLfloat B[],
                   GLfloat C[], float point_spacing) {
  auto sign = [](GLfloat A[], GLfloat B[], GLfloat C[]) {
    return (A[0] - C[0]) * (B[1] - C[1]) - (B[0] - C[0]) * (A[1] - C[1]);
  };

  auto in_triangle = [sign](GLfloat x, GLfloat y, GLfloat A[], GLfloat B[],
                            GLfloat C[]) {
    GLfloat point[]{x, y};
    bool b1 = sign(point, A, B) < 0.0f;
    bool b2 = sign(point, B, C) < 0.0f;
    bool b3 = sign(point, C, A) < 0.0f;
    return (b1 == b2) && (b2 == b3);
  };

  GLfloat x_min = std::min(std::min(A[0], B[0]), C[0]);
  GLfloat y_min = std::min(std::min(A[1], B[1]), C[1]);
  GLfloat x_max = std::max(std::max(A[0], B[0]), C[0]);
  GLfloat y_max = std::max(std::max(A[1], B[1]), C[1]);

  for (GLfloat x = x_min; x < x_max; x += point_spacing) {
    for (GLfloat y = y_min; y < y_max; y += point_spacing) {
      if (in_triangle(x, y, A, B, C)) add_point(points, x, y);
    }
  }
}

void fill_circle(std::vector<GLfloat>& points, GLfloat center[], float radius,
                 float point_spacing) {
  auto in_circle = [](GLfloat x, GLfloat y, GLfloat center[], float radius) {
    return 0 > ((x - center[0]) * (x - center[0]) +
                (y - center[1]) * (y - center[1]) - radius * radius);
  };

  GLfloat x_min = center[0] - radius;
  GLfloat y_min = center[1] - radius;
  GLfloat x_max = center[0] + radius;
  GLfloat y_max = center[1] + radius;

  for (GLfloat x = x_min; x < x_max; x += point_spacing) {
    for (GLfloat y = y_min; y < y_max; y += point_spacing) {
      if (in_circle(x, y, center, radius)) add_point(points, x, y);
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
  //     GLfloat change_y = begin[1] < end[1] ? space * delta_err : -space *
  //     delta_err;
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