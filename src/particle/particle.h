#ifndef PARTICLE_H
#define PARTICLE_H

#include "../../include/screensaver.h"
#include "../../include/shader.h"
#include <cmath>
#include <cstdlib>
#include <vector>

class Particle : public Screensaver {
public:
  void init(int width, int height, const std::string &resourcePath) override;
  void resize(int width, int height) override;
  void update(float deltaTime, float totalTime) override;
  void render() override;
  void cleanup() override;

private:
  static constexpr int NUM_PARTICLES = 5;
  static constexpr float DIMINISHER = 0.3f;
  static constexpr float ONE_THIRD = 1.0f / 3.0f;
  static constexpr float TWO_THIRDS = 2.0f / 3.0f;

  struct Instance {
    float x, y;
    float vx, vy;
    float r, g, b, a;
  };

  std::vector<Instance> particles;
  Shader *shader = nullptr;
  unsigned int VAO = 0, VBO = 0;

  static inline float wave(float x) { return 0.5f * (cosf(x) + 1.0f); }
  void initParticles();
  void makeColor(Instance &p, float time);
  void updatePhysics(float dt, float time);
};

#endif
