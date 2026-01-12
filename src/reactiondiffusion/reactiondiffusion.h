#ifndef REACTIONDIFFUSION_H
#define REACTIONDIFFUSION_H

#include "screensaver.h"
#include "shader.h"

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
  float time = 0.0f;
  float aspectRatio;
};

#endif
