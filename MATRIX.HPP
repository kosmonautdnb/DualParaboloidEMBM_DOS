#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include "vector.hpp"

class Matrix {
public:
  float m[4*4];

  Matrix() {
    for (int i = 0; i < 16; i++) m[i]=0.f;
 }

  Matrix &zero();
  Matrix &identity();

  Matrix &set(int x, int y, float v);
  Matrix &set(const Matrix &b);
  float get(int x, int y) const;
  
  Matrix &setRow(int v, const Vector &b);
  Vector getRow(int v) const;
  Matrix &setColumn(int v, const Vector &b);
  Vector getColumn(int v) const;
  
  Matrix &transpose();
  Matrix &transpose3x3();
  
  Matrix &mul(const Matrix &m2);
  Matrix &mul3x3(const Matrix &m2);
  Matrix &mulTransposed(const Matrix &m2);
  Matrix &mul3x3Transposed(const Matrix &m2);

  bool invert();
};

Matrix rotationMatrixX(float angle);
Matrix rotationMatrixY(float angle);
Matrix rotationMatrixZ(float angle);

Vector transform(const Vector &v, const Matrix &m);
Vector transformTransposed(const Vector &v, const Matrix &m);
Vector transform3x3(const Vector &v, const Matrix &m);
Vector transformTransposed3x3(const Vector &v, const Matrix &m);

#endif //__MATRIX_HPP__
