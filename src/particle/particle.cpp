#include "../../include/shader.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct Particle {
  float x, y;
  float vx, vy;
  float r, g, b, a;
};

struct Attractor {
  float x, y;
  float r, g, b;
};

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

int main() { return 0; }