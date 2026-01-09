#ifndef REDDUST_H
#define REDDUST_H
#include "perlin_noise.h"
#include "screensaver.h"
#include "shader.h"
#include "vec2.h"

struct Particle {
  Vec2 position, velocity;
  float age;
};

class RedDust : public Screensaver {
public:
  void init(int w, int h, const std::string &path) override;
  void resize(int w, int h) override;
  void update(float deltaTime, float totalTime) override;
  void render() override;
  void cleanup() override;

private:
  PerlinNoise perlin = PerlinNoise();
  Shader *shader = nullptr;
  unsigned int VAO = 0, VBO = 0;
  int width, height;
  float aspectRatio;
  std::string resourcePath;

  static const int PARTICLE_COUNT = 150000;
  std::vector<Particle> particles;

  static inline float randf() { return ((float)rand() / RAND_MAX); }

  Vec2 sampleWind(float x, float y, float time);
  void initParticles();
  void respawnParticle(Particle &p);
};
#endif