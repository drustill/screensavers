#include "../../include/glfw_runner.h"
#include "red_dust.h"

int main() {
  return runWithGLFW<RedDust>(800, 600, "Red Dust", "src/red_dust/");
}
