#include "windfield.h"

void WindField::init(int w, int h, const std::string &path) {
  width = w;
  height = h;
  resourcePath = path;

  std::string vsPath = resourcePath + "windfield.vs";
  std::string fsPath = resourcePath + "windfield.fs";
  shader = new Shader(vsPath.c_str(), fsPath.c_str());

  float line[] = {0.0f, 0.0f, 1.0f, 0.0f};

  gridPoints.resize(GRID_SIZE * GRID_SIZE);
  for (int y = 0; y < GRID_SIZE; y++) {
    for (int x = 0; x < GRID_SIZE; x++) {
      int idx = y * GRID_SIZE + x;
      gridPoints[idx].x = (x / (float)(GRID_SIZE - 1)) * 2.0f - 1.0f;
      gridPoints[idx].y = (y / (float)(GRID_SIZE - 1)) * 2.0f - 1.0f;
      gridPoints[idx].dirX = 1.0f;
      gridPoints[idx].dirY = 0.0f;
    }
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &lineVBO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
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
      //   Vec2 wind = sampleWind(gridPoints[idx].x, gridPoints[idx].y, time);
      gridPoints[idx].dirX = 0.0f;
      gridPoints[idx].dirY = 0.0f;
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, gridPoints.size() * sizeof(GridPoint),
                  gridPoints.data());
}

void WindField::render() {
  glClear(GL_COLOR_BUFFER_BIT);

  shader->use();
  shader->setFloat("uAspectRatio", (float)width / (float)height);
  shader->setFloat("uArrowLength", 0.08f);

  glBindVertexArray(VAO);
  glDrawArraysInstanced(GL_LINES, 0, 2, GRID_SIZE * GRID_SIZE);
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