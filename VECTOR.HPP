#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

class Vector {
public:
  float x,y,z;
  float w;

  Vector() {
    x  = 0;
    y  = 0;
    z  = 0;
    w  = 1;
  }

  Vector &set(float _x, float _y, float _z);
  Vector &set(float v);
  Vector &set(const Vector &b);

  Vector &zero();

  Vector &add(float _x, float _y, float _z);
  Vector &sub(float _x, float _y, float _z);
  Vector &div(float _x, float _y, float _z);
  Vector &mul(float _x, float _y, float _z);

  Vector &add(const Vector &b);
  Vector &sub(const Vector &b);
  Vector &div(const Vector &b);
  Vector &mul(const Vector &b);

  Vector &add(float v);
  Vector &sub(float v);
  Vector &div(float v);
  Vector &mul(float v);
  
  float length() const;
  float lengthSqr() const;

  Vector &normalize();
};

float dot(const Vector &a, const Vector &b);
Vector cross(const Vector &a, const Vector &b);
Vector normalized(const Vector &b);

Vector add(const Vector &a, const Vector &b);
Vector sub(const Vector &a, const Vector &b);
Vector mul(const Vector &a, const Vector &b);
Vector div(const Vector &a, const Vector &b);
Vector add(const Vector &a, float v);
Vector sub(const Vector &a, float v);
Vector mul(const Vector &a, float v);
Vector div(const Vector &a, float v);
Vector add(const Vector &a, float x, float y, float z);
Vector sub(const Vector &a, float x, float y, float z);
Vector mul(const Vector &a, float x, float y, float z);
Vector div(const Vector &a, float x, float y, float z);

#endif //__VECTOR_HPP__
