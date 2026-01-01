#include "../../include/shader.h"
#include "../window.cpp"
#include <cstdlib>
#include <vector>

struct Particle {
  float x, y;
  float vx, vy;
  float r, g, b, a;
};

const int NUM_PARTICLES = 5;
std::vector<Particle> particles(NUM_PARTICLES);

const float DIMINISHER = 0.3f;

enum ColorMode { RAINBOW };
ColorMode colorMode = RAINBOW;

void initParticles() {
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
  float colorRotation = 2.0f * M_PI;
  float beginR = wave(time * colorRotation);
  float beginG = wave((time + ONE_THIRD) * colorRotation);
  float beginB = wave((time + TWO_THIRDS) * colorRotation);
  p.r = beginR;
  p.g = beginG;
  p.b = beginB;
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
      float angle = ((float)rand() / RAND_MAX) * 2.0f * M_PI;
      float radius = 0.5f;
      p.x = cosf(angle) * radius;
      p.y = sinf(angle) * radius;
      p.vx = 0.0f;
      p.vy = 0.0f;
    }
  }
}

int main() {
  Window window(800, 600, "Particle");
  Shader shader("src/particle/particle.vs", "src/particle/particle.fs");
  initParticles();

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES * sizeof(Particle), nullptr,
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle),
                        (void *)offsetof(Particle, x));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle),
                        (void *)offsetof(Particle, r));
  glEnableVertexAttribArray(1);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glEnable(GL_PROGRAM_POINT_SIZE);

  float lastTime = glfwGetTime();

  while (!window.shouldClose()) {
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    updatePhysics(deltaTime, currentTime);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, NUM_PARTICLES * sizeof(Particle),
                    particles.data());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
    window.swapBuffers();
    window.pollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  return 0;
}