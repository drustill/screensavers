#include "../../include/shader.h"
#include "../window.cpp"
#include "GLFW/glfw3.h"
#include <cstddef>

struct Particle {
  float x, y;
  float vx, vy;
  float r, g, b, a;
};

struct Attractor {
  float x, y;
  float r, g, b;
};

const int NUM_PARTICLES = 15000;
const int NUM_ATTRACTORS = 3;
const float GRAVITY = 50.0f;
const float DRAG = 0.98f;

std::vector<Particle> particles(NUM_PARTICLES);
std::vector<Attractor> attractors(NUM_ATTRACTORS);

// Hashtag Claude generated
void updatePhysics(float deltaTime) {
  float time = glfwGetTime();

  // Update attractor positions (like Flurry's UpdateSpark)
  // They orbit in Lissajous patterns, just like Flurry!
  for (int i = 0; i < NUM_ATTRACTORS; i++) {
    float phase = (float)i * 2.0f * M_PI / NUM_ATTRACTORS;
    attractors[i].x = 0.5f * cosf(time * 1.3f + phase) * cosf(time * 0.7f);
    attractors[i].y = 0.5f * sinf(time * 1.1f + phase) * sinf(time * 0.9f);

    // Color cycling like Flurry
    attractors[i].r = 0.5f + 0.5f * sinf(time + phase);
    attractors[i].g = 0.5f + 0.5f * sinf(time * 1.3f + phase);
    attractors[i].b = 0.5f + 0.5f * sinf(time * 0.7f + phase);
  }

  // Update each particle (like Flurry's smoke particle loop)
  for (int i = 0; i < NUM_PARTICLES; i++) {
    Particle &p = particles[i];

    // Attract to each attractor (this is Flurry's core physics!)
    for (int j = 0; j < NUM_ATTRACTORS; j++) {
      float dx = attractors[j].x - p.x;
      float dy = attractors[j].y - p.y;
      float distSq = dx * dx + dy * dy + 0.001f; // avoid division by zero

      // F = G / r² (inverse square law, like Flurry)
      float force = GRAVITY / distSq * deltaTime;
      float dist = sqrtf(distSq);

      // Apply force in direction of attractor
      p.vx += (dx / dist) * force;
      p.vy += (dy / dist) * force;

      // Color particle based on nearest attractor
      if (j == i % NUM_ATTRACTORS) {
        p.r = attractors[j].r;
        p.g = attractors[j].g;
        p.b = attractors[j].b;
      }
    }

    // Apply drag (like Flurry's info->drag)
    p.vx *= DRAG;
    p.vy *= DRAG;

    // Integrate position
    p.x += p.vx * deltaTime;
    p.y += p.vy * deltaTime;

    // Fade based on velocity (faster = brighter)
    float speed = sqrtf(p.vx * p.vx + p.vy * p.vy);
    p.a = fminf(1.0f, speed * 2.0f);

    // Reset if too far (particle
    // %.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.%.
    if (fabsf(p.x) > 2.0f || fabsf(p.y) > 2.0f) {
      p.x = ((float)rand() / RAND_MAX - 0.5f) * 0.1f;
      p.y = ((float)rand() / RAND_MAX - 0.5f) * 0.1f;
      p.vx = p.vy = 0.0f;
    }
  }
}

void initParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    particles[i].x = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
    particles[i].y = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
    particles[i].vx = 0.0f;
    particles[i].vy = 0.0f;
    particles[i].r = 1.0f;
    particles[i].g = 0.5f;
    particles[i].b = 0.2f;
    particles[i].a = 1.0f;
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

    updatePhysics(deltaTime);

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