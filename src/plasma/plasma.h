#ifndef PLASMA_H
#define PLASMA_H

#include "../../include/screensaver.h"
#include "../../include/shader.h"

class Plasma : public Screensaver {
public:
  void init(int width, int height, const std::string &resourcePath) override;
  void resize(int width, int height) override;
  void update(float deltaTime, float totalTime) override;
  void render() override;
  void cleanup() override;

private:
  Shader *shader = nullptr;
  unsigned int VAO = 0, VBO = 0;
  float currentTime = 0.0f;
};

#endif
