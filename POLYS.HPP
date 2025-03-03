#ifndef __POLYS_HPP__
#define __POLYS_HPP__

#include <math.h>
#include "vector.hpp"

class Color {
public:
  float red;
  float green;
  float blue;
  float alpha;
  Color() {
    red = 1.f;
    green = 0.f;
    blue = 1.f;
    alpha = 1.f;
  }
  unsigned int rgba32() {
    int r = (int)floor(red * 255.f);
    int g = (int)floor(green * 255.f);
    int b = (int)floor(blue * 255.f);
    int a = (int)floor(alpha * 255.f);
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    if (a < 0) a = 0;
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    if (a > 255) a = 255;
    return r|(g<<8)|(b<<16)|(a<<24);
  }
  void init(const Vector &v) {
    red = v.x;
    green = v.y;
    blue = v.z;
    alpha = v.w;
  }
  void init(const Color &c) {
    red = c.red;
    green = c.green;
    blue = c.blue;
    alpha = c.alpha;
  }
  void init(float r, float g, float b, float a) {
    red = r;
    green = g;
    blue = b;
    alpha = a;
  }
  void init(unsigned int rgba32) {
    red = (float)((rgba32>>0) & 255)/255.f;
    green = (float)((rgba32>>8) & 255)/255.f;
    blue = (float)((rgba32>>16) & 255)/255.f;
    alpha = (float)((rgba32>>24) & 255)/255.f;
  }
};


class Vertex {
public:
  Vector p;
  Vector n;
  float sx, sy;
  float u[1],v[1];
  Color c[1];
};

class Poly {
public:
  Vertex v[3];
  Color c[1];
};

#endif __POLYS_HPP__