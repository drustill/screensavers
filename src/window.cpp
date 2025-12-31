#include "../include/window.h"
#include "GLFW/glfw3.h"
#include <cstddef>
#include <iostream>

Window::Window(int w, int h, const std::string &title) : width(w), height(h) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }

  glViewport(0, 0, width, height);
};

void Window::processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void Window::framebuffer_size_callback(GLFWwindow *window, int width,
                                       int height) {
  glViewport(0, 0, width, height);
}

bool Window::shouldClose() {
  bool should_close = glfwWindowShouldClose(window);
  if (!should_close) {
    processInput(window);
  }
  return should_close;
}

void Window::swapBuffers() { glfwSwapBuffers(window); }
void Window::pollEvents() { glfwPollEvents(); }

Window::~Window() { glfwTerminate(); }