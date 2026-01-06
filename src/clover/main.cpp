#include "../../include/glfw_runner.h"
#include "clover.h"

int main() {
  return runWithGLFW<CloverField>(800, 600, "Clover", "src/clover/");
}
