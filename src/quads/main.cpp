#include "../../include/glfw_runner.h"
#include "quads.h"

int main() { return runWithGLFW<Quads>(800, 600, "Quads", "src/quads/"); }
