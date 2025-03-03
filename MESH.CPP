#include "mesh.hpp"

void MeshPart::clear() {
  vertices.clear();
  faces.clear();
}


void MeshPart::getBounds(Vector &bmin, Vector &bmax) {
  for (int i = 0; i < vertices.size(); i++) {
    Vector k = vertices[i].p;
    if (i == 0||k.x<bmin.x) bmin.x = k.x;
    if (i == 0||k.y<bmin.y) bmin.y = k.y;
    if (i == 0||k.z<bmin.z) bmin.z = k.z;
    if (i == 0||k.x>bmax.x) bmax.x = k.x;
    if (i == 0||k.y>bmax.y) bmax.y = k.y;
    if (i == 0||k.z>bmax.z) bmax.z = k.z;
  }
}

void MeshPart::translate(const Vector &mov) {
  for (int i = 0; i < vertices.size(); i++) {
    vertices[i].p.add(mov);
  }
}

void MeshPart::generateNormals() {
  int i;
  for (i = 0; i < faces.size(); i++) {
    MeshTriangle *f = &faces[i];
    const MeshVertex &p0 = vertices[f->i[0]];
    const MeshVertex &p1 = vertices[f->i[1]];
    const MeshVertex &p2 = vertices[f->i[2]];
    f->n = cross(sub(p1.p,p0.p).normalize(),sub(p2.p,p0.p).normalize()).normalize();
  }
  for (i = 0; i < vertices.size(); i++) {
    vertices[i].n.set(0,0,0);
  }
  for (i = 0; i < faces.size(); i++) {
    MeshTriangle *f = &faces[i];
    MeshVertex &p0 = vertices[f->i[0]];
    MeshVertex &p1 = vertices[f->i[1]];
    MeshVertex &p2 = vertices[f->i[2]];
    p0.n.add(f->n);
    p1.n.add(f->n);
    p2.n.add(f->n);
  }
  for (i = 0; i < vertices.size(); i++) {
    vertices[i].n.normalize();
  }
}

void Mesh::clear() {
  for (int i = 0; i < parts.size(); i++) 
    delete parts[i];
  parts.clear();
}

void Mesh::loadASC(const String &fileName) {
  FILE *in = fopen(fileName.c_str(),"r");
  char buffer[1024];
  int i;
  clear();  
  while(fgets(buffer,1024,in)) {
    int vertexCount;
    int faceCount;
    if (sscanf(buffer,"Tri-mesh, Vertices:%d  Faces:%d",&vertexCount,&faceCount)) {
      MeshPart *part = new MeshPart();
      parts.push_back(part);
      for (i = 0; i < vertexCount; i++) {
        int id; MeshVertex v;
        if (!fgets(buffer,1024,in)) break;
        if (sscanf(buffer,"Vertex %d: X: %f Y: %f Z: %f",&id, &v.p.x,&v.p.y,&v.p.z)) {
          part->vertices.resize(id+1);
          part->vertices[id] = v;
        } else i--;
      }
      for (i = 0; i < faceCount; i++) {
        int id; MeshTriangle f;
        if (!fgets(buffer,1024,in)) break;
        if (sscanf(buffer,"Face %d: A:%d B:%d C:%d",&id, &f.i[0], &f.i[1], &f.i[2])) {
          part->faces.resize(id+1);
          part->faces[id] = f;
        } else i--;
      }
      part->generateNormals();
    }
  }
  fclose(in);
}

void Mesh::getBounds(Vector &bmin, Vector &bmax) {
  for (int i = 0; i < parts.size(); i++) {
    Vector amin,amax;
    parts[i]->getBounds(amin,amax);
    if (i == 0||amin.x<bmin.x) bmin.x = amin.x;
    if (i == 0||amin.y<bmin.y) bmin.y = amin.y;
    if (i == 0||amin.z<bmin.z) bmin.z = amin.z;
    if (i == 0||amax.x>bmax.x) bmax.x = amax.x;
    if (i == 0||amax.y>bmax.y) bmax.y = amax.y;
    if (i == 0||amax.z>bmax.z) bmax.z = amax.z;
  }
}

void Mesh::translate(const Vector &mov) {
  for (int i = 0; i < parts.size(); i++) 
    parts[i]->translate(mov);
}
