#ifndef WINDFIELD_H
#define WINDFIELD_H

#include "perlin_noise.h"
#include "screensaver.h"
#include "shader.h"
#include "vec2.h"

struct GridPoint {
  float x, y;
  float dirX, dirY;
};

struct Particle {
  Vec2 position, velocity;
  float age;
};

class WindField : public Screensaver {
public:
  void init(int w, int h, const std::string &path) override;
  void resize(int w, int h) override;
  void update(float deltaTime, float totalTime) override;
  void render() override;
  void cleanup() override;

  Vec2 sampleWind(float x, float y, float time);

private:
  PerlinNoise perlin = PerlinNoise();

  Shader *shader = nullptr;
  Shader *particleShader = nullptr;
  unsigned int VAO = 0, VBO = 0, lineVBO = 0;
  unsigned int particleVAO = 0, particleVBO = 0;

  float time = 0.0f, aspectRatio;

  static const int GRID_SIZE = 20;
  static const int PARTICLE_COUNT = 50000;
  std::vector<GridPoint> gridPoints;
  std::vector<Particle> particles;

  static inline float randf() { return ((float)rand() / RAND_MAX); }
  void initParticles();
  void respawnParticle(Particle &p);
};

#endif
