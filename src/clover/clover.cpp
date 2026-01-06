#include "clover.h"
#include <glad/glad.h>

void CloverField::init(int w, int h, const std::string &path) {
  width = w;
  height = h;
  resourcePath = path;

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
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, cloverVBO);
  glBufferData(GL_ARRAY_BUFFER, NUM_CLOVERS * sizeof(Clover), clovers.data(),
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Clover), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribDivisor(1, 1);

  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Clover),
                        (void *)offsetof(Clover, size));
  glEnableVertexAttribArray(2);
  glVertexAttribDivisor(2, 1);

  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Clover),
                        (void *)offsetof(Clover, rotation));
  glEnableVertexAttribArray(3);
  glVertexAttribDivisor(3, 1);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CloverField::resize(int w, int h) {
  width = w;
  height = h;
}

void CloverField::update(float deltaTime, float totalTime) {
  currentTime = totalTime;
  updatePhysics(deltaTime, totalTime);

  glBindBuffer(GL_ARRAY_BUFFER, cloverVBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, NUM_CLOVERS * sizeof(Clover),
                  clovers.data());
}

void CloverField::render() {
  glClear(GL_COLOR_BUFFER_BIT);

  shader->use();
  shader->setFloat("uAspectRatio", (float)width / (float)height);
  shader->setFloat("time", currentTime);

  glBindVertexArray(VAO);
  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, NUM_CLOVERS);
}

void CloverField::cleanup() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &cloverVBO);
  delete shader;
  shader = nullptr;
}

void CloverField::initClovers() {
  for (int i = 0; i < NUM_CLOVERS; i++) {
    clovers[i].position = Vec2();
    clovers[i].velocity = Vec2();
    clovers[i].mass = 1.0f;
    clovers[i].size = 1.0f;
    clovers[i].rotation = 0.0f;
  }
}

void CloverField::applyForce(Clover &c, Vec2 force) {
  c.acceleration += (force / c.mass);
}

void CloverField::checkEdges(Clover &c) {
  if (c.position.x > 2.0f) {
    c.position.x = -1.0f;
  } else if (c.position.x < -2.0f) {
    c.position.x = 1.0f;
  }

  if (c.position.y > 2.0f) {
    c.position.y = -1.0f;
  } else if (c.position.y < -2.0f) {
    c.position.y = 1.0f;
  }
}

void CloverField::updateClover(Clover &c, float dt) {
  c.velocity += c.acceleration * dt;
  c.position += c.velocity * dt;
  c.acceleration *= 0;
}

Vec2 CloverField::getWind(float time) {
  float strength = 0.2f * sin(time * 0.5f);
  return Vec2(strength, 0.0f);
}

void CloverField::updatePhysics(float dt, float time) {
  Vec2 gravity = Vec2(0.0f, -0.05f);
  for (auto &c : clovers) {
    Vec2 wind = getWind(time);
    applyForce(c, gravity);
    applyForce(c, wind);
    checkEdges(c);
    updateClover(c, dt);
  }
}
