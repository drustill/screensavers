#include "../../include/glfw_runner.h"
#include "windfield.h"

int main() {
  return runWithGLFW<WindField>(800, 600, "Windfield", "src/windfield/");
}
