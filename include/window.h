#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glad/glad.h>
#include <string>

class Window {
public:
  GLFWwindow *window;
  int width, height;

  Window(int w, int h, const std::string &title);
  ~Window();

  bool shouldClose();
  void swapBuffers();
  void pollEvents();

private:
  void processInput(GLFWwindow *window);
  static void framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height);
};

#endif