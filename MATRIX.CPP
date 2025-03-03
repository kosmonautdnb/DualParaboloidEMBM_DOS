#include "matrix.hpp"
#include <math.h>

Matrix &Matrix::zero() {
  for (int i = 0; i < 16; i++) m[i]=0;
  return *this;
}

Matrix &Matrix::identity() {
  zero();
  set(0,0,1);
  set(1,1,1);
  set(2,2,1);
  set(3,3,1);
  return *this;
}  

Matrix &Matrix::set(int x, int y, float v) {
  m[x+y*4]=v;
  return *this;
}

Matrix &Matrix::set(const Matrix &b) {
  for (int i = 0; i < 16; i++) m[i]=b.m[i];
  return *this;
}

float Matrix::get(int x, int y) const {
  return m[x+y*4];
}

Matrix &Matrix::setRow(int v, const Vector &b) {
  set(0,v,b.x);
  set(1,v,b.y);
  set(2,v,b.z);
  set(3,v,b.w);
  return *this;
}

Vector Matrix::getRow(int v) const {
  Vector r;
  r.x = get(0,v);
  r.y = get(1,v);
  r.z = get(2,v);
  r.w = get(3,v);
  return r;
}

Matrix &Matrix::setColumn(int v, const Vector &b) {
  set(v,0,b.x);
  set(v,1,b.y);
  set(v,2,b.z);
  set(v,3,b.w);
  return *this;
}

Vector Matrix::getColumn(int v) const {
  Vector r;
  r.x = get(v,0);
  r.y = get(v,1);
  r.z = get(v,2);
  r.w = get(v,3);
  return r;
}

Matrix &Matrix::transpose() {
  Matrix b;
  b.set(*this);
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      set(x,y,b.get(y,x));
    }
  }
  return *this;
}

Matrix &Matrix::transpose3x3() {
  Matrix b;
  b.set(*this);
  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      set(x,y,b.get(y,x));
    }
  }
  return *this;
}

Matrix &Matrix::mul(const Matrix &m2) {
  Matrix m1;
  m1.set(*this);
  for (int i0=0;i0<4;i0++) {
    for (int i1=0;i1<4;i1++) {
      float a = 0.f;
      for (int i2=0;i2<4;i2++) {
        a+=m1.get(i2,i1)*m2.get(i0,i2);
      }
      set(i0,i1,a);
    }
  }
  return *this;
}

Matrix &Matrix::mul3x3(const Matrix &m2) {
  Matrix m1;
  m1.set(*this);
  for (int i0=0;i0<3;i0++) {
    for (int i1=0;i1<3;i1++) {
      float a = 0.f;
      for (int i2=0;i2<3;i2++) {
        a+=m1.get(i2,i1)*m2.get(i0,i2);
      }
      set(i0,i1,a);
    }
  }
  return *this;
}

Matrix &Matrix::mulTransposed(const Matrix &m2) {
  Matrix m1;
  m1.set(*this);
  for (int i0=0;i0<4;i0++) {
    for (int i1=0;i1<4;i1++) {
      float a = 0.f;
      for (int i2=0;i2<4;i2++) {
        a+=m2.get(i2,i1)*m1.get(i0,i2);
      }
      set(i0,i1,a);
    }
  }
  return *this;
}

Matrix &Matrix::mul3x3Transposed(const Matrix &m2) {
  Matrix m1;
  m1.set(*this);
  for (int i0=0;i0<3;i0++) {
    for (int i1=0;i1<3;i1++) {
      float a = 0.f;
      for (int i2=0;i2<3;i2++) {
        a+=m2.get(i2,i1)*m1.get(i0,i2);
      }
      set(i0,i1,a);
    }
  }
  return *this;
}

bool Matrix::invert() {
  // supposedly taken from glu (gl)
  float inv[16], det;    
  int i;    

  inv[0] = m[5]  * m[10] * m[15] -
  m[5]  * m[11] * m[14] -
  m[9]  * m[6]  * m[15] +              
  m[9]  * m[7]  * m[14] +             
  m[13] * m[6]  * m[11] -             
  m[13] * m[7]  * m[10];    

  inv[4] = -m[4]  * m[10] * m[15] +               
  m[4]  * m[11] * m[14] +               
  m[8]  * m[6]  * m[15] -               
  m[8]  * m[7]  * m[14] -               
  m[12] * m[6]  * m[11] +               
  m[12] * m[7]  * m[10];    
  
  inv[8] = m[4]  * m[9] * m[15] -              
  m[4]  * m[11] * m[13] -              
  m[8]  * m[5] * m[15] +              
  m[8]  * m[7] * m[13] +              
  m[12] * m[5] * m[11] -              
  m[12] * m[7] * m[9];    
  
  inv[12] = -m[4]  * m[9] * m[14] +                
  m[4]  * m[10] * m[13] +               
  m[8]  * m[5] * m[14] -                
  m[8]  * m[6] * m[13] -                
  m[12] * m[5] * m[10] +                
  m[12] * m[6] * m[9];    
  
  inv[1] = -m[1]  * m[10] * m[15] +              
  m[1]  * m[11] * m[14] +               
  m[9]  * m[2] * m[15] -               
  m[9]  * m[3] * m[14] -               
  m[13] * m[2] * m[11] +               
  m[13] * m[3] * m[10];    
  
  inv[5] = m[0]  * m[10] * m[15] -              
  m[0]  * m[11] * m[14] -              
  m[8]  * m[2] * m[15] +              
  m[8]  * m[3] * m[14] +              
  m[12] * m[2] * m[11] -              
  m[12] * m[3] * m[10];    
  
  inv[9] = -m[0]  * m[9] * m[15] +               
  m[0]  * m[11] * m[13] +               
  m[8]  * m[1] * m[15] -                 
  m[8]  * m[3] * m[13] -               
  m[12] * m[1] * m[11] +               
  m[12] * m[3] * m[9];    
  
  inv[13] = m[0]  * m[9] * m[14] -               
  m[0]  * m[10] * m[13] -               
  m[8]  * m[1] * m[14] +               
  m[8]  * m[2] * m[13] +               
  m[12] * m[1] * m[10] -               
  m[12] * m[2] * m[9];    
  
  inv[2] = m[1]  * m[6] * m[15] -              
  m[1]  * m[7] * m[14] -              
  m[5]  * m[2] * m[15] +              
  m[5]  * m[3] * m[14] +              
  m[13] * m[2] * m[7] -              
  m[13] * m[3] * m[6];    
  
  inv[6] = -m[0]  * m[6] * m[15] +               
  m[0]  * m[7] * m[14] +               
  m[4]  * m[2] * m[15] -               
  m[4]  * m[3] * m[14] -               
  m[12] * m[2] * m[7] +               
  m[12] * m[3] * m[6];    
  
  inv[10] = m[0]  * m[5] * m[15] -               
  m[0]  * m[7] * m[13] -               
  m[4]  * m[1] * m[15] +               
  m[4]  * m[3] * m[13] +               
  m[12] * m[1] * m[7] -               
  m[12] * m[3] * m[5];    
  
  inv[14] = -m[0]  * m[5] * m[14] +                
  m[0]  * m[6] * m[13] +                
  m[4]  * m[1] * m[14] -                
  m[4]  * m[2] * m[13] -                
  m[12] * m[1] * m[6] +                
  m[12] * m[2] * m[5];    
  
  inv[3] = -m[1] * m[6] * m[11] +               
  m[1] * m[7] * m[10] +               
  m[5] * m[2] * m[11] -               
  m[5] * m[3] * m[10] -               
  m[9] * m[2] * m[7] +               
  m[9] * m[3] * m[6];    
  
  inv[7] = m[0] * m[6] * m[11] -              
  m[0] * m[7] * m[10] -              
  m[4] * m[2] * m[11] +              
  m[4] * m[3] * m[10] +              
  m[8] * m[2] * m[7] -              
  m[8] * m[3] * m[6];    
  
  inv[11] = -m[0] * m[5] * m[11] +                
  m[0] * m[7] * m[9] +                
  m[4] * m[1] * m[11] -                
  m[4] * m[3] * m[9] -                
  m[8] * m[1] * m[7] +                
  m[8] * m[3] * m[5];    
  
  inv[15] = m[0] * m[5] * m[10] -               
  m[0] * m[6] * m[9] -               
  m[4] * m[1] * m[10] +               
  m[4] * m[2] * m[9] +               
  m[8] * m[1] * m[6] -               
  m[8] * m[2] * m[5];    

  det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];    
  if (det == 0)        
    return false;    
  
  det = 1.f / det;    
  for (i = 0; i < 16; i++)        
    m[i] = inv[i] * det;
  
  return true;
}

Matrix rotationMatrixX(float angle) {
  Matrix r;
  r.identity();
  r.set(2,2,cos(angle));
  r.set(1,2,-sin(angle));
  r.set(1,1,cos(angle));
  r.set(2,1,sin(angle));
  return r;
}

Matrix rotationMatrixY(float angle) {
  Matrix r;
  r.identity();
  r.set(0,0,cos(angle));
  r.set(2,0,-sin(angle));
  r.set(2,2,cos(angle));
  r.set(0,2,sin(angle));
  return r;
}
                 
Matrix rotationMatrixZ(float angle) {
  Matrix r;
  r.identity();
  r.set(1,1,cos(angle));
  r.set(0,1,-sin(angle));
  r.set(0,0,cos(angle));
  r.set(1,0,sin(angle));
  return r;
}

Vector transform(const Vector &v, const Matrix &m) {
  Vector r;
  r.x = m.get(0,0)*v.x+m.get(1,0)*v.y+m.get(2,0)*v.z+m.get(3,0)*v.w;
  r.y = m.get(0,1)*v.x+m.get(1,1)*v.y+m.get(2,1)*v.z+m.get(3,1)*v.w;
  r.z = m.get(0,2)*v.x+m.get(1,2)*v.y+m.get(2,2)*v.z+m.get(3,2)*v.w;
  r.w = m.get(0,3)*v.x+m.get(1,3)*v.y+m.get(2,3)*v.z+m.get(3,3)*v.w;
  return r;
}

Vector transformTransposed(const Vector &v, const Matrix &m) {
  Vector r;
  r.x = m.get(0,0)*v.x+m.get(0,1)*v.y+m.get(0,2)*v.z+m.get(0,3)*v.w;
  r.y = m.get(1,0)*v.x+m.get(1,1)*v.y+m.get(1,2)*v.z+m.get(1,3)*v.w;
  r.z = m.get(2,0)*v.x+m.get(2,1)*v.y+m.get(2,2)*v.z+m.get(2,3)*v.w;
  r.w = m.get(3,0)*v.x+m.get(3,1)*v.y+m.get(2,2)*v.z+m.get(3,3)*v.w;
  return r;
}

Vector transform3x3(const Vector &v, const Matrix &m) {
  Vector r;
  r.x = m.get(0,0)*v.x+m.get(1,0)*v.y+m.get(2,0)*v.z;
  r.y = m.get(0,1)*v.x+m.get(1,1)*v.y+m.get(2,1)*v.z;
  r.z = m.get(0,2)*v.x+m.get(1,2)*v.y+m.get(2,2)*v.z;
  return r;
}

Vector transformTransposed3x3(const Vector &v, const Matrix &m) {
  Vector r;
  r.x = m.get(0,0)*v.x+m.get(0,1)*v.y+m.get(0,2)*v.z;
  r.y = m.get(1,0)*v.x+m.get(1,1)*v.y+m.get(1,2)*v.z;
  r.z = m.get(2,0)*v.x+m.get(2,1)*v.y+m.get(2,2)*v.z;
  return r;
}
