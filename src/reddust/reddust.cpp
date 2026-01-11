#include "reddust.h"
#include <cstddef>

void RedDust::initParticles() {
  particles.resize(PARTICLE_COUNT);
  for (Particle &p : particles) {
    respawnParticle(p);
    p.age = randf() * 15.0f;
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(Particle), nullptr,
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Particle),
                        (void *)offsetof(Particle, velocity));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle),
                        (void *)offsetof(Particle, age));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void RedDust::respawnParticle(Particle &p) {
  p.position.x = -1.25f + randf() * 0.1f;
  p.position.y = -1.0f + randf() * 2.0f;
  p.velocity = Vec2();
  p.age = 0.0f;
}

Vec2 RedDust::sampleWind(float x, float y, float time) {
  float scale = 5.0f;
  float noiseX = perlin.noise(x * scale + time * 0.3f, y * scale);
  float noiseY = perlin.noise(x * scale + 100.0f, y * scale + time * 0.3f);

  float baseWindX = 0.2f;
  float baseWindY = 0.0f;

  float windX = baseWindX + noiseX * 0.4f;
  float windY = baseWindY + noiseY * 0.3f;

  return Vec2(windX, windY);
}

void RedDust::init(int w, int h, const std::string &path) {
  width = w;
  height = h;
  resourcePath = path;
  aspectRatio = (float)w / (float)h;

  std::string vsPath = resourcePath + "reddust.vs";
  std::string fsPath = resourcePath + "reddust.fs";
  shader = new Shader(vsPath.c_str(), fsPath.c_str());

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  initParticles();
}

void RedDust::update(float deltaTime, float totalTime) {
  for (int i = 0; i < PARTICLE_COUNT; i++) {
    Particle &p = particles[i];
    Vec2 wind = sampleWind(p.position.x, p.position.y, totalTime);
    p.velocity += (wind - p.velocity) * deltaTime;
    p.position += p.velocity * deltaTime;
    p.age += deltaTime;
    if (p.position.x > 1.5f || p.position.x < -1.5f || p.position.y > 1.5f ||
        p.position.y < -1.5f || p.age > 15.0f) {
      respawnParticle(p);
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, PARTICLE_COUNT * sizeof(Particle),
                  particles.data());
}

void RedDust::render() {
  glClear(GL_COLOR_BUFFER_BIT);

  shader->use();
  glBindVertexArray(VAO);
  glPointSize(2.0f);
  glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);
}

void RedDust::cleanup() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  delete shader;
}

void RedDust::resize(int w, int h) {
  width = w;
  height = h;
  aspectRatio = (float)w / (float)h;
}