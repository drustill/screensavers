#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <string>

class Screensaver {
public:
  virtual ~Screensaver() = default;

  virtual void init(int width, int height, const std::string &resourcePath) = 0;
  virtual void resize(int width, int height) = 0;
  virtual void update(float deltaTime, float totalTime) = 0;
  virtual void render() = 0;
  virtual void cleanup() = 0;

protected:
  int width = 0;
  int height = 0;
  std::string resourcePath;
};

#endif
