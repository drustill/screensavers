#!/bin/bash

cd "$(dirname "$0")/.."

echo "Name of screensaver:"
read name

if [ -z "$name" ]; then
  echo "Invalid name. Cannot be empty. Exiting."
  exit 1
fi

classname="$(tr '[:lower:]' '[:upper:]' <<< ${name:0:1})${name:1}"
uppername=$(echo "$name" | tr '[:lower:]' '[:upper:]')

mkdir src/$name

write_main () {
  cat > "src/$name/main.cpp" << EOF
#include "../../include/glfw_runner.h"
#include "$name.h"

int main() { return runWithGLFW<$classname>(800, 600, "$classname", "src/$name/"); }
EOF
}

write_h () {
  cat > "src/$name/$name.h" << EOF
#ifndef ${uppername}_H
#define ${uppername}_H

#include "../../include/screensaver.h"
#include "../../include/shader.h"

class $classname : public Screensaver {
public:
  void init(int width, int height, const std::string &resourcePath) override;
  void resize(int width, int height) override;
  void update(float deltaTime, float totalTime) override;
  void render() override;
  void cleanup() override;

private:
  Shader *shader = nullptr;
  unsigned int VAO = 0, VBO = 0;
  float time = 0.0f;
};

#endif
EOF
}

write_mm () {
  cat > "src/$name/macos.mm" << EOF
#include <glad/glad.h>
#include <string>

#include "../backends/SaverTemplate.h"
#include "$name.h"

SAVER_TEMPLATE(${classname}SaverView, $classname)
EOF
}

touch src/$name/$name.cpp src/$name/$name.vs src/$name/$name.fs 
write_main
write_h
write_mm

echo "Initialized $name"
