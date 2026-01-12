#include "reactiondiffusion.h"
#include "shader.h"
#include <utility>

void Reactiondiffusion::init(int w, int h, const std::string &path) {
  width = w;
  height = h;
  resourcePath = path;
  aspectRatio = (float)w / (float)h;

  std::string vsPath = resourcePath + "reactiondiffusion.vs";
  std::string fsPath = resourcePath + "reactiondiffusion.fs";
  shader = new Shader(vsPath.c_str(), fsPath.c_str());

  float vertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f,  1.0f,
                      -1.0f, -1.0f, 1.0f, 1.0f,  -1.0f, 1.0f};

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  shader->use();
  shader->setInt("textureA", 0);
  shader->setInt("textureB", 1);

  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, 512, 512, 0, GL_RED, GL_FLOAT,
               nullptr);

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, 512, 512, 0, GL_RED, GL_FLOAT,
               nullptr);

  for (int i = 0; i < size; i++) {
    A[i] = 1.0f;
    B[i] = (rand() % 100 < 5) ? 1.0f : 0.0f;
  }
}

Laplace Reactiondiffusion::laplace(int i) {
  int x = i & 511;
  int y = i / 512;
  int left = y * 512 + ((x - 1 + 512) & 511);
  int right = y * 512 + ((x + 1) & 511);
  int up = ((y - 1 + 512) & 511) * 512 + x;
  int down = ((y + 1) & 511) * 512 + x;
  float a = A[left] + A[right] + A[up] + A[down] - 4 * A[i];
  float b = B[left] + B[right] + B[up] + B[down] - 4 * B[i];
  return Laplace{a, b};
}

void Reactiondiffusion::update(float deltaTime, float totalTime) {
  for (int i = 0; i < size; i++) {
    Laplace l = laplace(i);
    float abb = A[i] * B[i] * B[i];
    A_next[i] = A[i] + (diffusion_a * l.a - abb + feed * (1 - A[i]));
    B_next[i] = B[i] + (diffusion_b * l.b + abb - (kill + feed) * B[i]);
  }
  std::swap(A, A_next);
  std::swap(B, B_next);
}

void Reactiondiffusion::render() {
  glClear(GL_COLOR_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 512, GL_RED, GL_FLOAT, A);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 512, GL_RED, GL_FLOAT, B);

  shader->use();

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Reactiondiffusion::cleanup() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

void Reactiondiffusion::resize(int w, int h) {
  width = w;
  height = h;
}