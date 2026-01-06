#ifndef WINDFIELD_H
#define WINDFIELD_H

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
  Shader *shader = nullptr;
  unsigned int VAO = 0, VBO = 0, lineVBO = 0;
  float time = 0.0f, aspectRatio;
  static const int GRID_SIZE = 20;
  std::vector<GridPoint> gridPoints;

  float noise2D(float x, float y);
  float fade(float t);
  float lerp(float a, float b, float t);
  float grad(int hash, float x, float y);

  int permutation[512];
  void initPermutation();
};

#endif
