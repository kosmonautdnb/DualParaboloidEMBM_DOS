#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "vector.hpp"
#include "array.hpp"
#include "string.hpp"

class MeshVertex {
public:
  Vector p;
  Vector n;
};

class MeshTriangle {
public:
  int i[3];
  Vector n;
};

class MeshPart {
public:
  Array<MeshVertex> vertices;
  Array<MeshTriangle> faces;

  void clear();
  void getBounds(Vector &bmin, Vector &bmax);
  void translate(const Vector &mov);
  void generateNormals();
};

class Mesh {
public:
  Array<MeshPart*> parts;

  ~Mesh() {
    for (int i = 0; i < parts.size(); i++) delete parts[i];
  }

  void clear();
  void loadASC(const String &fileName);
  void getBounds(Vector &bmin, Vector &bmax);
  void translate(const Vector &mov);
};

#endif //__MESH_HPP__
