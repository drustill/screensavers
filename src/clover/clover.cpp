#include "clover.h"
#include <glad/glad.h>

void Clover::init(int width, int height, const std::string &resourcePath) {
  width_ = width;
  height_ = height;
  resourcePath_ = resourcePath;

  std::string vsPath = resourcePath + "clover.vs";
  std::string fsPath = resourcePath + "clover.fs";
  shader = new Shader(vsPath.c_str(), fsPath.c_str());

  clovers.resize(NUM_CLOVERS);
  initClovers();

  float quad[] = {
      -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,  0.5f,
      -0.5f, -0.5f, 0.5f, 0.5f,  -0.5f, 0.5f,
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &cloverVBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, cloverVBO);
  glBufferData(GL_ARRAY_BUFFER, NUM_CLOVERS * sizeof(Instance), clovers.data(),
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Instance),
                        (void *)offsetof(Instance, x));
  glEnableVertexAttribArray(1);
  glVertexAttribDivisor(1, 1);

  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Instance),
                        (void *)offsetof(Instance, size));
  glEnableVertexAttribArray(2);
  glVertexAttribDivisor(2, 1);

  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Instance),
                        (void *)offsetof(Instance, rotation));
  glEnableVertexAttribArray(3);
  glVertexAttribDivisor(3, 1);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Clover::resize(int width, int height) {
  width_ = width;
  height_ = height;
}

void Clover::update(float deltaTime, float totalTime) {
  currentTime = totalTime;
  updatePhysics(deltaTime, totalTime);

  glBindBuffer(GL_ARRAY_BUFFER, cloverVBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, NUM_CLOVERS * sizeof(Instance),
                  clovers.data());
}

void Clover::render() {
  glClear(GL_COLOR_BUFFER_BIT);

  shader->use();
  shader->setFloat("uAspectRatio", (float)width_ / (float)height_);
  shader->setFloat("time", currentTime);

  glBindVertexArray(VAO);
  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, NUM_CLOVERS);
}

void Clover::cleanup() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &cloverVBO);
  delete shader;
  shader = nullptr;
}

void Clover::initClovers() {
  for (int i = 0; i < NUM_CLOVERS; i++) {
    clovers[i].x = randf() * 2.0f - 1.0f;
    clovers[i].y = randf() * 2.0f - 1.0f;
    clovers[i].vx = 0.0f;
    clovers[i].vy = 0.0f;
    clovers[i].size = 0.1f + randf() * 0.3f;
    clovers[i].rotation = randf() * TWO_PI * 0.2f;
  }
}

void Clover::updatePhysics(float dt, float time) {
  float attractorX = 0.5f * sinf(time * 0.3f);
  float attractorY = 0.5f * cosf(time * 0.3f);

  for (auto &c : clovers) {
    float dx = attractorX - c.x;
    float dy = attractorY - c.y;
    float dist = sqrtf(dx * dx + dy * dy + 0.1f);

    float force = (1.0f / (dist * dist)) * 0.1f;
    c.vx += (dx / dist) * force * dt;
    c.vy += (dy / dist) * force * dt;

    c.x += c.vx * dt;
    c.y += c.vy * dt;

    if (c.x > 1.0f)
      c.x = -1.0f;
    if (c.x < -1.0f)
      c.x = 1.0f;
    if (c.y > 1.0f)
      c.y = -1.0f;
    if (c.y < -1.0f)
      c.y = 1.0f;
  }
}
