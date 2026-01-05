#include "quads.h"
#include <glad/glad.h>

void Quads::init(int w, int h, const std::string &path) {
  width = w;
  height = h;
  resourcePath = path;

  std::string vsPath = resourcePath + "quads.vs";
  std::string fsPath = resourcePath + "quads.fs";
  shader = new Shader(vsPath.c_str(), fsPath.c_str());

  particles.resize(NUM_PARTICLES);
  initParticles();

  float quad[] = {
      -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,  0.5f,
      -0.5f, -0.5f, 0.5f, 0.5f,  -0.5f, 0.5f,
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &particleVBO);
  glGenBuffers(1, &quadVBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
  glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES * sizeof(Particle), nullptr,
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Particle),
                        (void *)offsetof(Particle, x));
  glEnableVertexAttribArray(1);
  glVertexAttribDivisor(1, 1);

  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle),
                        (void *)offsetof(Particle, r));
  glEnableVertexAttribArray(2);
  glVertexAttribDivisor(2, 1);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

void Quads::resize(int w, int h) {
  width = w;
  height = h;
}

void Quads::update(float deltaTime, float totalTime) {
  updatePhysics(deltaTime, totalTime);

  glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, NUM_PARTICLES * sizeof(Particle),
                  particles.data());
}

void Quads::render() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shader->use();
  shader->setFloat("uSize", 0.5f);
  shader->setFloat("uAspect", (float)width / (float)height);

  glBindVertexArray(VAO_);
  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, NUM_PARTICLES);
}

void Quads::cleanup() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &particleVBO);
  glDeleteBuffers(1, &quadVBO);
  delete shader;
  shader = nullptr;
}

void Quads::initParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    float angle = randf() * TWO_PI;
    float radius = 0.3f + randf() * 0.5f;
    particles[i].x = cosf(angle) * radius;
    particles[i].y = sinf(angle) * radius;
    particles[i].vx = 0.0f;
    particles[i].vy = 0.0f;
    particles[i].shimmer = randf();
    particles[i].a = 1.0f;
  }
}

void Quads::makeColor(Particle &p, float time) {
  float cycleTime = 5.0f;
  float colorRotation = TWO_PI / cycleTime;
  float beginR = wave(time * colorRotation);
  float beginG = wave((time + cycleTime * ONE_THIRD) * colorRotation);
  float beginB = wave((time + cycleTime * TWO_THIRDS) * colorRotation);
  float shimmer = sinf(p.shimmer * TWO_PI + time);
  p.r = beginR + shimmer;
  p.g = beginG + shimmer;
  p.b = beginB + shimmer;
}

void Quads::updatePhysics(float dt, float time) {
  float attractorX = 0.0f;
  float attractorY = 0.0f;

  for (auto &p : particles) {
    float dx = attractorX - p.x;
    float dy = attractorY - p.y;
    float dist = sqrtf(dx * dx + dy * dy + 0.1f);

    float force = (1.0f / (dist * dist)) * DIMINISHER;
    p.vx += (dx / dist) * force * dt;
    p.vy += (dy / dist) * force * dt;

    p.x += p.vx * dt;
    p.y += p.vy * dt;

    makeColor(p, time);

    if (fabsf(p.x) > 2.0f || fabsf(p.y) > 2.0f) {
      float angle = randf() * TWO_PI;
      float radius = 0.5f;
      p.x = cosf(angle) * radius;
      p.y = sinf(angle) * radius;
      p.vx = 0.0f;
      p.vy = 0.0f;
    }
  }
}
