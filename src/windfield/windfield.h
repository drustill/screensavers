#ifndef WINDFIELD_H
#define WINDFIELD_H

#include "include/perlin_noise.h"
#include "screensaver.h"
#include "shader.h"
#include "vec2.h"

struct GridPoint {
  float x, y;
  float dirX, dirY;
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
  unsigned int VAO = 0, VBO = 0, lineVBO = 0;
  float time = 0.0f, aspectRatio;
  static const int GRID_SIZE = 20;
  std::vector<GridPoint> gridPoints;
};

#endif
