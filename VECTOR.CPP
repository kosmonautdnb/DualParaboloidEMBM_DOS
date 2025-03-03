#include "vector.hpp"
#include "polys.hpp"
#include <math.h>


Vector &Vector::set(float _x, float _y, float _z) {
  x = _x; 
  y = _y; 
  z = _z; 
  return *this;
}

Vector &Vector::set(float v) {
  x = v; 
  y = v; 
  z = v; 
  return *this;
}

Vector &Vector::set(const Vector &b) {
  x = b.x; 
  y = b.y; 
  z = b.z; 
  return *this;
}

Vector &Vector::zero() {
  x = 0; 
  y = 0; 
  z = 0; 
  return *this;
}

Vector &Vector::add(float _x, float _y, float _z) {
  x += _x; 
  y += _y; 
  z += _z; 
  return *this;
}

Vector &Vector::sub(float _x, float _y, float _z) {
  x -= _x; 
  y -= _y; 
  z -= _z; 
  return *this;
}

Vector &Vector::div(float _x, float _y, float _z) {
  x /= _x; 
  y /= _y; 
  z /= _z; 
  return *this;
}

Vector &Vector::mul(float _x, float _y, float _z) {
  x *= _x;
  y *= _y;
  z *= _z; 
  return *this;
}

Vector &Vector::add(const Vector &b) {
  x += b.x; 
  y += b.y; 
  z += b.z; 
  return *this;
}

Vector &Vector::sub(const Vector &b) {
  x -= b.x; 
  y -= b.y; 
  z -= b.z; 
  return *this;
}

Vector &Vector::div(const Vector &b) {
  x /= b.x; 
  y /= b.y; 
  z /= b.z; 
  return *this;
}

Vector &Vector::mul(const Vector &b) {
  x *= b.x; 
  y *= b.y; 
  z *= b.z; 
  return *this;
}

Vector &Vector::add(float v) {
  x += v; 
  y += v; 
  z += v; 
  return *this;
}

Vector &Vector::sub(float v) {
  x -= v; 
  y -= v; 
  z -= v; 
  return *this;
}

Vector &Vector::div(float v) {
  x /= v; 
  y /= v; 
  z /= v; 
  return *this;
}                                   

Vector &Vector::mul(float v) {
  x *= v; 
  y *= v; 
  z *= v; 
  return *this;
}

float Vector::length() const {
  return sqrt(x*x+y*y+z*z);
}

float Vector::lengthSqr() const {
  return x*x+y*y+z*z;
}

Vector &Vector::normalize() {
  float l = length(); 
  if (l>0) 
    div(l); 
  return *this;
}


float dot(const Vector &a, const Vector &b) {
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector cross(const Vector &a, const Vector &b)  {
  Vector r;
  r.x = a.y*b.z - a.z*b.y;
  r.y = a.z*b.x - a.x*b.z;
  r.z = a.x*b.y - a.y*b.x;
  return r;
}

Vector normalized(const Vector &b) {
  Vector r;
  r.set(b).normalize();
  return r;
}


Vector add(const Vector &a, const Vector &b) {
  Vector r;
  r.set(a).add(b);
  return r;
}

Vector sub(const Vector &a, const Vector &b) {
  Vector r;
  r.set(a).sub(b);
  return r;
}

Vector mul(const Vector &a, const Vector &b) { 
  Vector r;
  r.set(a).mul(b);
  return r;
}

Vector div(const Vector &a, const Vector &b) {
  Vector r;
  r.set(a).div(b);
  return r;
}

Vector add(const Vector &a, float v) {
  Vector r;
  r.set(a).add(v);
  return r;
}

Vector sub(const Vector &a, float v) {
  Vector r;
  r.set(a).sub(v);
  return r;
}

Vector mul(const Vector &a, float v) {
  Vector r;
  r.set(a).mul(v);
  return r;
}

Vector div(const Vector &a, float v) {
  Vector r;
  r.set(a).div(v);
  return r;
}

Vector add(const Vector &a, float x, float y, float z) {
  Vector r;
  r.set(a).add(x,y,z);
  return r;
}

Vector sub(const Vector &a, float x, float y, float z) {
  Vector r;
  r.set(a).sub(x,y,z);
  return r;
}

Vector mul(const Vector &a, float x, float y, float z) {
  Vector r;
  r.set(a).mul(x,y,z);
  return r;
}

Vector div(const Vector &a, float x, float y, float z) {
  Vector r;
  r.set(a).div(x,y,z);
  return r;
}
