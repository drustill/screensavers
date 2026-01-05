#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <vector>

class Vec2 {
public:
  float x, y;
  Vec2() : x(0), y(0) {}
  Vec2(float x, float y) : x(x), y(y) {}
  Vec2(float s) : x(s), y(s) {}

  friend Vec2 operator+(const Vec2 &v1, const Vec2 &v2) {
    return Vec2(v1.x + v2.x, v1.y + v2.y);
  }
  friend Vec2 operator+(const Vec2 &v, float &s) {
    return Vec2(v.x + s, v.y + s);
  }
  friend Vec2 operator-(const Vec2 &v1, const Vec2 &v2) {
    return Vec2(v1.x - v2.x, v1.y - v2.y);
  }
  friend Vec2 operator-(const Vec2 &v, float &s) {
    return Vec2(v.x - s, v.y - s);
  }
  friend Vec2 operator*(const Vec2 &v1, const Vec2 &v2) {
    return Vec2(v1.x * v2.x, v1.y * v2.y);
  }
  friend Vec2 operator*(const Vec2 &v, float &s) {
    return Vec2(v.x * s, v.y * s);
  }
  friend Vec2 operator/(const Vec2 &v1, const Vec2 &v2) {
    return Vec2(v1.x / v2.x, v1.y / v2.y);
  }
  friend Vec2 operator/(const Vec2 &v, float &s) {
    return Vec2(v.x / s, v.y / s);
  }
  Vec2 &operator+=(const Vec2 &v) {
    x += v.x;
    y += v.y;
    return *this;
  }
  Vec2 &operator-=(const Vec2 &v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }
  Vec2 &operator*=(const Vec2 &v) {
    x *= v.x;
    y *= v.y;
    return *this;
  }
  Vec2 &operator/=(const Vec2 &v) {
    x /= v.x;
    y /= v.y;
    return *this;
  }
  float length() const { return sqrtf(x * x + y * y); }
};

#endif