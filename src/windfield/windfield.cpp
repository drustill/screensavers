#include "windfield.h"
#include <algorithm>
#include <random>

void WindField::initPermutation() {
  // Standard Perlin permutation table
  std::vector<int> p(256);
  for (int i = 0; i < 256; i++)
    p[i] = i;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(p.begin(), p.end(), gen);

  for (int i = 0; i < 256; i++) {
    permutation[i] = p[i];
    permutation[256 + i] = p[i];
  }
}

float WindField::fade(float t) {
  // 6t^5 - 15t^4 + 10t^3
  return t * t * t * (t * (t * 6 - 15) + 10);
}

float WindField::lerp(float a, float b, float t) { return a + t * (b - a); }

float WindField::grad(int hash, float x, float y) {
  // Convert low 2 bits of hash to gradient direction
  int h = hash & 3;
  float u = h < 2 ? x : y;
  float v = h < 2 ? y : x;
  return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

float WindField::noise2D(float x, float y) {
  // Find unit square containing point
  int xi = (int)floor(x) & 255;
  int yi = (int)floor(y) & 255;

  // Relative position in square
  float xf = x - floor(x);
  float yf = y - floor(y);

  // Fade curves
  float u = fade(xf);
  float v = fade(yf);

  // Hash corners
  int aa = permutation[permutation[xi] + yi];
  int ab = permutation[permutation[xi] + yi + 1];
  int ba = permutation[permutation[xi + 1] + yi];
  int bb = permutation[permutation[xi + 1] + yi + 1];

  // Blend
  float x1 = lerp(grad(aa, xf, yf), grad(ba, xf - 1, yf), u);
  float x2 = lerp(grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1), u);

  return lerp(x1, x2, v);
}

Vec2 WindField::sampleWind(float x, float y, float time) {
  // Sample noise at different offsets for x and y components
  // Adding time makes it animate
  float scale = 2.0f;
  float noiseX = noise2D(x * scale + time * 0.3f, y * scale);
  float noiseY = noise2D(x * scale + 100.0f, y * scale + time * 0.3f);

  // Base wind direction (mostly horizontal)
  float baseWindX = 0.5f;
  float baseWindY = 0.0f;

  // Add noise variation
  float windX = baseWindX + noiseX * 0.4f;
  float windY = baseWindY + noiseY * 0.3f;

  return Vec2(windX, windY);
}

void WindField::init(int w, int h, const std::string &path) {
  initPermutation();
  width = w;
  height = h;
  resourcePath = path;
  aspectRatio = (float)w / (float)h;

  std::string vsPath = resourcePath + "windfield.vs";
  std::string fsPath = resourcePath + "windfield.fs";
  shader = new Shader(vsPath.c_str(), fsPath.c_str());

  float line[] = {0.0f, 0.0f, 1.0f, 0.0f};
  float quad[] = {0.0f, -0.5f, 1.0f, -0.5f, 1.0f, 0.5f,
                  0.0f, -0.5f, 1.0f, 0.5f,  0.0f, 0.5f};

  float margin = 0.1f;
  gridPoints.resize(GRID_SIZE * GRID_SIZE);
  for (int y = 0; y < GRID_SIZE; y++) {
    for (int x = 0; x < GRID_SIZE; x++) {
      int idx = y * GRID_SIZE + x;
      gridPoints[idx].x =
          (x / (float)(GRID_SIZE - 1)) * (2.0f - 2 * margin) - 1.0f + margin;
      gridPoints[idx].y =
          (y / (float)(GRID_SIZE - 1)) * (2.0f - 2 * margin) - 1.0f + margin;
      gridPoints[idx].dirX = 1.0f;
      gridPoints[idx].dirY = 0.0f;
    }
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &lineVBO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, gridPoints.size() * sizeof(GridPoint),
               gridPoints.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GridPoint), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribDivisor(1, 1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GridPoint),
                        (void *)offsetof(GridPoint, dirX));
  glEnableVertexAttribArray(2);
  glVertexAttribDivisor(2, 1);

  glBindVertexArray(0);
}

void WindField::update(float deltaTime, float totalTime) {
  for (int y = 0; y < GRID_SIZE; y++) {
    for (int x = 0; x < GRID_SIZE; x++) {
      int idx = y * GRID_SIZE + x;
      Vec2 wind = sampleWind(gridPoints[idx].x, gridPoints[idx].y, totalTime);
      gridPoints[idx].dirX = wind.x;
      gridPoints[idx].dirY = wind.y;
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, gridPoints.size() * sizeof(GridPoint),
                  gridPoints.data());
}

void WindField::render() {
  glClear(GL_COLOR_BUFFER_BIT);

  shader->use();
  shader->setFloat("uAspectRatio", aspectRatio);
  shader->setFloat("uArrowLength", 0.08f);
  shader->setFloat("uArrowThickness", 0.008f);

  glBindVertexArray(VAO);
  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, GRID_SIZE * GRID_SIZE);
}

void WindField::cleanup() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &lineVBO);
  glDeleteBuffers(1, &VBO);
}

void WindField::resize(int w, int h) {
  width = w;
  height = h;
}