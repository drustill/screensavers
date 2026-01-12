#ifndef REACTIONDIFFUSION_H
#define REACTIONDIFFUSION_H

#include "screensaver.h"
#include "shader.h"

struct Laplace {
  float a, b;
};
class Reactiondiffusion : public Screensaver {
public:
  void init(int w, int h, const std::string &resourcePath) override;
  void resize(int w, int h) override;
  void update(float deltaTime, float totalTime) override;
  void render() override;
  void cleanup() override;

private:
  Shader *shader = nullptr;
  unsigned int VAO = 0, VBO = 0;
  unsigned int texture1, texture2;

  int size = 512 * 512;
  float *A = new float[size];
  float *B = new float[size];
  float *A_next = new float[size];
  float *B_next = new float[size];
  float diffusion_a = 0.21;
  float diffusion_b = 0.105;
  float feed = 0.055;
  float kill = 0.062;
  float time = 0.0f;
  float aspectRatio;
  Laplace laplace(int i);
};

#endif
