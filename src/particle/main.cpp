#include "../../include/glfw_runner.h"
#include "particle.h"

int main() {
  return runWithGLFW<Particle>(800, 600, "Particle", "src/particle/");
}
