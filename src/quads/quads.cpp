#include "quads.h"
#include <glad/glad.h>

void Quads::init(int width, int height, const std::string &resourcePath) {
  width_ = width;
  height_ = height;
  resourcePath_ = resourcePath;

  std::string vsPath = resourcePath + "quads.vs";
  std::string fsPath = resourcePath + "quads.fs";
  shader_ = new Shader(vsPath.c_str(), fsPath.c_str());

  particles_.resize(NUM_PARTICLES);
  initParticles();

  float quad[] = {
      -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,  0.5f,
      -0.5f, -0.5f, 0.5f, 0.5f,  -0.5f, 0.5f,
  };

  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &particleVBO_);
  glGenBuffers(1, &quadVBO_);

  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, particleVBO_);
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

void Quads::resize(int width, int height) {
  width_ = width;
  height_ = height;
}

void Quads::update(float deltaTime, float totalTime) {
  updatePhysics(deltaTime, totalTime);

  glBindBuffer(GL_ARRAY_BUFFER, particleVBO_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, NUM_PARTICLES * sizeof(Particle),
                  particles_.data());
}

void Quads::render() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shader_->use();
  shader_->setFloat("uSize", 0.5f);
  shader_->setFloat("uAspect", (float)width_ / (float)height_);

  glBindVertexArray(VAO_);
  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, NUM_PARTICLES);
}

void Quads::cleanup() {
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &particleVBO_);
  glDeleteBuffers(1, &quadVBO_);
  delete shader_;
  shader_ = nullptr;
}

void Quads::initParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    float angle = randf() * TWO_PI;
    float radius = 0.3f + randf() * 0.5f;
    particles_[i].x = cosf(angle) * radius;
    particles_[i].y = sinf(angle) * radius;
    particles_[i].vx = 0.0f;
    particles_[i].vy = 0.0f;
    particles_[i].shimmer = randf();
    particles_[i].a = 1.0f;
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

  for (auto &p : particles_) {
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

