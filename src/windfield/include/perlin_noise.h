#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <array>

/// Given (double x, double y) put it in perspective of unit square, and then
// for each vertex, pseudorandom choose a gradient vector from the center to the
// edges/vertices. Dot product each gradient of each vertex with the distance
// from (double x, double y) to that vertex. Each dot product output is an
// influence value. Linearly interpolating these influences based on our input
// coordinate is the noise
//
// When crossing an edge between two squares, the transition of influence values
// can create an unnatural transition. An ease function makes the rate of change
// approach 0 as (double x, double y) -> edge
//
// Ken Perlin ease curve: 6t^5 - 15t^4 + 10t^3

class PerlinNoise {
public:
  double noise(double x, double y) {
    int xfloor = (int)floor(x);
    int yfloor = (int)floor(y);

    int xi = xfloor & 255;
    int yi = yfloor & 255;

    double xf = x - xfloor;
    double yf = y - yfloor;

    double u = fade(xf);
    double v = fade(yf);

    int aa = p[p[xi] + yi];
    int ba = p[p[xi + 1] + yi];
    int ab = p[p[xi] + yi + 1];
    int bb = p[p[xi + 1] + yi + 1];

    double x1 = lerp(grad(aa, xf, yf), grad(ba, xf - 1, yf), u);
    double x2 = lerp(grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1), u);

    return lerp(x1, x2, v);
  }

private:
  static constexpr int permutation[256] = {
      151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,
      225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190,
      6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117,
      35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136,
      171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158,
      231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,
      245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209,
      76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,
      164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,
      202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
      58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,
      154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
      19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
      228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
      145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184,
      84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,
      222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156,
      180};

  static constexpr std::array<int, 512> p = []() {
    std::array<int, 512> res{};
    for (size_t i = 0; i < 512; i++) {
      res[i] = permutation[i % 256];
    }
    return res;
  }();

  double fade(double u) { return u * u * u * (u * (u * 6 - 15) + 10); }
  double lerp(double a, double b, double t) { return a * (1 - t) + b * t; }
  double grad(int hash, double x, double y) {
    switch (hash & 0x3) {
    case 0x0:
      return x + y;
    case 0x1:
      return -x + y;
    case 0x2:
      return x - y;
    case 0x3:
      return -x - y;
    default:
      return 0;
    }
  }
};

#endif