#include "../../include/shader.h"
#include "../window.cpp"
#include <cmath>
#include <cstdlib>
#include <vector>

struct Particle {
  float x, y;
  float vx, vy;
  float r, g, b, a;
  float shimmer;
};

const float ASPECT_RATIO = 800.0f / 600.0f;
const int NUM_PARTICLES = 15;
std::vector<Particle> particles(NUM_PARTICLES);

const float DIMINISHER = 0.3f;
const float TWO_PI = 2.0f * M_PI;

inline float randf() { return ((float)rand() / RAND_MAX); }

enum ColorMode { RAINBOW };
ColorMode colorMode = RAINBOW;

void initParticles() {
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

/**
 * cos(x) between -1 and 1
 * => cos(x) + 1 between 0 and 2
 * => 0.5 * (cos(x) + 1) 0 and 1
 *
 * On color wheel, R, G, B are 120 deg apart
 */

const float ONE_THIRD = 1.0f / 3.0f;
const float TWO_THIRDS = 2.0f / 3.0f;

inline float wave(float x) { return 0.5f * (cosf(x) + 1.0f); }

void makeColor(Particle &p, float time) {
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

void updatePhysics(float dt, float time) {
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

int main() {
  Window window(800, 600, "Quads");
  Shader shader("src/quads/quads.vs", "src/quads/quads.fs");
  initParticles();

  float quad[] = {
      -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,  0.5f,
      -0.5f, -0.5f, 0.5f, 0.5f,  -0.5f, 0.5f,
  };

  unsigned int VAO, particleVBO, quadVBO;
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

  float lastTime = glfwGetTime();

  while (!window.shouldClose()) {
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    updatePhysics(deltaTime, currentTime);

    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, NUM_PARTICLES * sizeof(Particle),
                    particles.data());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    shader.setFloat("uSize", 0.5f);
    shader.setFloat("uAspect", ASPECT_RATIO);

    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, NUM_PARTICLES);
    window.swapBuffers();
    window.pollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &particleVBO);
  glDeleteBuffers(1, &quadVBO);
  return 0;
}