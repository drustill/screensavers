#ifndef GLFW_RUNNER_H
#define GLFW_RUNNER_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "screensaver.h"
#include <iostream>
#include <string>

template <typename T>
int runWithGLFW(int width, int height, const std::string &title,
                const std::string &resourcePath) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window =
      glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  T screensaver;
  screensaver.init(width, height, resourcePath);

  glfwSetWindowUserPointer(window, &screensaver);
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow *win, int w, int h) {
    glViewport(0, 0, w, h);
    T *saver = static_cast<T *>(glfwGetWindowUserPointer(win));
    if (saver) {
      saver->resize(w, h);
    }
  });

  float lastTime = static_cast<float>(glfwGetTime());

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    float currentTime = static_cast<float>(glfwGetTime());
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    screensaver.update(deltaTime, currentTime);
    screensaver.render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  screensaver.cleanup();
  glfwTerminate();
  return 0;
}

#endif
