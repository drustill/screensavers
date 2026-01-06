#ifndef CLOVER_H
#define CLOVER_H

#include "../../include/screensaver.h"
#include "../../include/shader.h"
#include "../../include/vec2.h"
#include <cmath>
#include <cstdlib>
#include <vector>

class CloverField : public Screensaver {
public:
  void init(int width, int height, const std::string &resourcePath) override;
  void resize(int width, int height) override;
  void update(float deltaTime, float totalTime) override;
  void render() override;
  void cleanup() override;

private:
  static constexpr float TWO_PI = 2.0f * M_PI;
  static constexpr int NUM_CLOVERS = 1;

  struct Clover {
    Vec2 position, velocity, acceleration;
    float mass, size, rotation;
  };

  std::vector<Clover> clovers;
  Shader *shader = nullptr;
  unsigned int VAO = 0, VBO = 0, cloverVBO = 0;
  float currentTime = 0.0f;

  static inline float randf() { return ((float)rand() / RAND_MAX); }
  void initClovers();
  void updatePhysics(float dt, float time);

  void updateClover(Clover &c, float dt);
  void applyForce(Clover &c, Vec2 force);
  void checkEdges(Clover &c);
  Vec2 getWind(float time);
};

#endif
