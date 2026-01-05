#include "particle.h"
#include <glad/glad.h>

void Particle::init(int w, int h, const std::string &path) {
  width = w;
  height = h;
  resourcePath = path;

  std::string vsPath = resourcePath + "particle.vs";
  std::string fsPath = resourcePath + "particle.fs";
  shader = new Shader(vsPath.c_str(), fsPath.c_str());

  particles.resize(NUM_PARTICLES);
  initParticles();

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES * sizeof(Instance), nullptr,
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Instance),
                        (void *)offsetof(Instance, x));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Instance),
                        (void *)offsetof(Instance, r));
  glEnableVertexAttribArray(1);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glEnable(GL_PROGRAM_POINT_SIZE);
}

void Particle::resize(int w, int h) {
  width = w;
  height = h;
}

void Particle::update(float deltaTime, float totalTime) {
  updatePhysics(deltaTime, totalTime);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, NUM_PARTICLES * sizeof(Instance),
                  particles.data());
}

void Particle::render() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shader->use();
  glBindVertexArray(VAO);
  glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
}

void Particle::cleanup() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  delete shader;
  shader = nullptr;
}

void Particle::initParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    float angle = ((float)rand() / RAND_MAX) * 2.0f * M_PI;
    float radius = 0.3f + ((float)rand() / RAND_MAX) * 0.5f;
    particles[i].x = cosf(angle) * radius;
    particles[i].y = sinf(angle) * radius;
    particles[i].vx = 0.0f;
    particles[i].vy = 0.0f;
    particles[i].a = 1.0f;
  }
}

void Particle::makeColor(Instance &p, float time) {
  float cycleTime = 5.0f;
  float colorRotation = 2.0f * M_PI / cycleTime;
  float beginR = wave(time * colorRotation);
  float beginG = wave((time + cycleTime * ONE_THIRD) * colorRotation);
  float beginB = wave((time + cycleTime * TWO_THIRDS) * colorRotation);
  p.r = beginR;
  p.g = beginG;
  p.b = beginB;
}

void Particle::updatePhysics(float dt, float time) {
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
      float angle = ((float)rand() / RAND_MAX) * 2.0f * M_PI;
      float radius = 0.5f;
      p.x = cosf(angle) * radius;
      p.y = sinf(angle) * radius;
      p.vx = 0.0f;
      p.vy = 0.0f;
    }
  }
}
