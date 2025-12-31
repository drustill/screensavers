#include "../../include/shader.h"
#include "../window.cpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

int main() {
  Window window(800, 600, "Plasma");
  Shader ourShader("src/plasma/plasma.vs", "src/plasma/plasma.fs");

  float vertices[] = {
      -1.0f, -1.0f, -1.0f, 1.0f,  1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,
  };

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  while (!window.shouldClose()) {
    glClear(GL_COLOR_BUFFER_BIT);

    ourShader.use();
    ourShader.setFloat("time", glfwGetTime());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    window.swapBuffers();
    window.pollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  return 0;
}