#include "../../include/shader.h"
#include "../window.cpp"
#include "GLFW/glfw3.h"
#include <cstdlib>
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 600;
const float TWO_PI = 2.0f * M_PI;

inline float randf() { return ((float)rand() / RAND_MAX); }

struct Clover {
  float x, y;
  float vx, vy;
  float size, rotation;
};

const int NUM_CLOVERS = 50;
std::vector<Clover> clovers(NUM_CLOVERS);

void initClovers() {
  for (int i = 0; i < NUM_CLOVERS; i++) {
    clovers[i].x = randf() * 2.0f - 1.0f;
    clovers[i].y = randf() * 2.0f - 1.0f;
    clovers[i].vx = 0.0f;
    clovers[i].vy = 0.0f;
    clovers[i].size = 0.1f + randf() * 0.3f;
    clovers[i].rotation = randf() * TWO_PI * 0.2;
  }
}

void updatePhysics(float dt, float time) {
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

int main() {
  Window window(WIDTH, HEIGHT, "Clover");
  Shader shader("src/clover/clover.vs", "src/clover/clover.fs");
  initClovers();

  float quad[] = {
      -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,  0.5f,
      -0.5f, -0.5f, 0.5f, 0.5f,  -0.5f, 0.5f,
  };

  unsigned int VAO, VBO, cloverVBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &cloverVBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, cloverVBO);
  glBufferData(GL_ARRAY_BUFFER, NUM_CLOVERS * sizeof(Clover), clovers.data(),
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Clover),
                        (void *)offsetof(Clover, x));
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

  float lastTime = glfwGetTime();

  while (!window.shouldClose()) {
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    updatePhysics(deltaTime, currentTime);

    glBindBuffer(GL_ARRAY_BUFFER, cloverVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, NUM_CLOVERS * sizeof(Clover),
                    clovers.data());

    int width, height;
    glfwGetWindowSize(window.window, &width, &height);

    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    shader.setFloat("uAspectRatio", (float)width / (float)height);
    shader.setFloat("time", glfwGetTime());

    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, NUM_CLOVERS);

    window.swapBuffers();
    window.pollEvents();
  }

  return 0;
}