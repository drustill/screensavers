#ifndef QUADS_H
#define QUADS_H

#include "../../include/screensaver.h"
#include "../../include/shader.h"
#include <cmath>
#include <cstdlib>
#include <vector>

class Quads : public Screensaver {
public:
  void init(int width, int height, const std::string &resourcePath) override;
  void resize(int width, int height) override;
  void update(float deltaTime, float totalTime) override;
  void render() override;
  void cleanup() override;

private:
  static constexpr int NUM_PARTICLES = 15;
  static constexpr float DIMINISHER = 0.3f;
  static constexpr float TWO_PI = 2.0f * M_PI;
  static constexpr float ONE_THIRD = 1.0f / 3.0f;
  static constexpr float TWO_THIRDS = 2.0f / 3.0f;

  struct Particle {
    float x, y;
    float vx, vy;
    float r, g, b, a;
    float shimmer;
  };

  std::vector<Particle> particles;
  Shader *shader = nullptr;
  unsigned int VAO = 0, particleVBO = 0, quadVBO = 0;

  static inline float randf() { return ((float)rand() / RAND_MAX); }
  static inline float wave(float x) { return 0.5f * (cosf(x) + 1.0f); }
  void initParticles();
  void makeColor(Particle &p, float time);
  void updatePhysics(float dt, float time);
};

#endif
