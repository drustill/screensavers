#include "../../include/glfw_runner.h"
#include "reddust.h"

int main() {
  return runWithGLFW<RedDust>(800, 600, "Red Dust", "src/reddust/");
}
