#include "plasma.h"
#include <glad/glad.h>

void Plasma::init(int width, int height, const std::string &resourcePath) {
  width_ = width;
  height_ = height;
  resourcePath_ = resourcePath;

  std::string vsPath = resourcePath + "plasma.vs";
  std::string fsPath = resourcePath + "plasma.fs";
  shader = new Shader(vsPath.c_str(), fsPath.c_str());

  float vertices[] = {
      -1.0f, -1.0f, -1.0f, 1.0f,  1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
}

void Plasma::resize(int width, int height) {
  width_ = width;
  height_ = height;
}

void Plasma::update(float deltaTime, float totalTime) {
  currentTime = totalTime;
}

void Plasma::render() {
  glClear(GL_COLOR_BUFFER_BIT);

  shader->use();
  shader->setFloat("time", currentTime);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Plasma::cleanup() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  delete shader;
  shader = nullptr;
}
