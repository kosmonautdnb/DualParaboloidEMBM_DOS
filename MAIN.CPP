#include "types.hpp"
#include "vga.hpp"
#include "vesa.hpp"
#include "drawpoly.hpp"
#include <conio.h>
#include "mesh.hpp"
#include "matrix.hpp"
#include "pngload.hpp"

#define FRAMEBUFFERWIDTH 640
#define FRAMEBUFFERHEIGHT 480
#define FARPLANE 10000.f

int frameBufferWidth = FRAMEBUFFERWIDTH;
int frameBufferHeight =  FRAMEBUFFERHEIGHT;

unsigned int *frameBuffer;
float *zBuffer;

unsigned int *linearFrameBuffer;

unsigned int *texture;
int textureWidth;
int textureHeight;

unsigned int *texture2;
int textureWidth2;
int textureHeight2;

Matrix envyMatrix;

void setupVesa() {
  VbeInfoBlock vesa_info;
  VbeModeInfo vesa_modeInfo;
  int mode = vesa_getMode(frameBufferWidth,frameBufferHeight,32,true);
  vesa_setMode(mode);
  vesa_getModeInfo(mode, vesa_modeInfo);
  linearFrameBuffer = (unsigned int *)vesa_initLinearFrameBuffer(vesa_modeInfo);
}

void blitToScreen() {
  memcpy(linearFrameBuffer,frameBuffer,frameBufferWidth*frameBufferHeight*4);
}

void clearFrameBuffer(unsigned int color) {
  for (int i = 0; i < frameBufferWidth*frameBufferHeight; i++) frameBuffer[i] = color;
}

void clearZBuffer(float z) {
  for (int i = 0; i < frameBufferWidth*frameBufferHeight; i++) zBuffer[i] = z;
}

Matrix globalMatrix;
float screenCenterX = frameBufferWidth/2;
float screenCenterY = frameBufferHeight/2;

bool transformProject(Vertex &v) {
  Vector t;
  Vector &p = v.p;
  t.x = p.x * globalMatrix.get(0,0) + p.y * globalMatrix.get(1,0) + p.z * globalMatrix.get(2,0) + globalMatrix.get(3,0);
  t.y = p.x * globalMatrix.get(0,1) + p.y * globalMatrix.get(1,1) + p.z * globalMatrix.get(2,1) + globalMatrix.get(3,1);
  t.z = p.x * globalMatrix.get(0,2) + p.y * globalMatrix.get(1,2) + p.z * globalMatrix.get(2,2) + globalMatrix.get(3,2);
  t.w = p.x * globalMatrix.get(0,3) + p.y * globalMatrix.get(1,3) + p.z * globalMatrix.get(2,3) + globalMatrix.get(3,3);
  t.x /= t.w;  
  t.y /= t.w;  
  t.z /= t.w;  
  t.w /= t.w;  
  v.p.x = t.x;
  v.p.y = t.y;
  v.p.z = t.z;
  v.p.w = t.w;
  v.sx = t.x * 220.f / t.z + screenCenterX;
  v.sy = t.y * 220.f / t.z + screenCenterY;
  return true;
}

int main(int argc, const char *argv[])
{
  int gfxMode = getBiosGraphicsMode();
  setupVesa();
  frameBuffer = new unsigned int[FRAMEBUFFERWIDTH*FRAMEBUFFERHEIGHT];
  zBuffer = new float[FRAMEBUFFERWIDTH*FRAMEBUFFERHEIGHT];
  Mesh m;
  m.loadASC("CHRMFACE.ASC");
  ImageLayer l;
  loadPNG(&l,"SUNSET.PNG");
  texture = l.data;
  textureWidth = l.width;
  textureHeight = l.height;

  textureWidth2 = 256;
  textureHeight2 = 256;
  texture2 = new unsigned int[textureWidth2*textureHeight2];
  for (int y = 0; y < textureHeight2; y++) {
    for (int x = 0; x < textureWidth2; x++) {
      int u = sin((float)x*4/textureWidth2*2.f*3.14159f)*127+128;
      int v = sin((float)y*4/textureHeight2*2.f*3.14159f)*127+128;
      texture2[x+y*textureWidth2] = (u & 255)+((v & 255)<<8);
    }
  }

  globalMatrix.identity();
  globalMatrix.set(3,2, 75);
  Vector p0,p1;
  m.getBounds(p0,p1);
  m.translate(add(p0,p1).mul(-0.5));
  Poly p;
  float time = 0.f;
  do {
    clearFrameBuffer(0xff000000);
    clearZBuffer(FARPLANE);
    Matrix mat = rotationMatrixY(time*3).mul(rotationMatrixX(time));
    envyMatrix = mat;
    for (int j = 0; j < m.parts.size(); j++) {
      MeshPart *part = m.parts[j];
      for (int i = 0; i < part->faces.size(); i++) {
        MeshTriangle &f = part->faces[i];
        p.v[0].c[0].init(add(mul(part->vertices[f.i[0]].n,0.5),0.5));
        p.v[1].c[0].init(add(mul(part->vertices[f.i[1]].n,0.5),0.5));
        p.v[2].c[0].init(add(mul(part->vertices[f.i[2]].n,0.5),0.5));
        float us = 0.075; float vs = 0.075;
        p.v[0].u[0]=part->vertices[f.i[0]].p.x*us+0.5;
        p.v[1].u[0]=part->vertices[f.i[1]].p.x*us+0.5;
        p.v[2].u[0]=part->vertices[f.i[2]].p.x*us+0.5;
        p.v[0].v[0]=part->vertices[f.i[0]].p.z*vs-0.5;
        p.v[1].v[0]=part->vertices[f.i[1]].p.z*vs-0.5;
        p.v[2].v[0]=part->vertices[f.i[2]].p.z*vs-0.5;
        p.v[0].n = part->vertices[f.i[0]].n;
        p.v[1].n = part->vertices[f.i[1]].n;
        p.v[2].n = part->vertices[f.i[2]].n;
        p.v[0].p = part->vertices[f.i[0]].p;
        p.v[1].p = part->vertices[f.i[1]].p;
        p.v[2].p = part->vertices[f.i[2]].p;
        p.v[0].p = transform(p.v[0].p,mat);
        p.v[1].p = transform(p.v[1].p,mat);
        p.v[2].p = transform(p.v[2].p,mat);
        transformProject(p.v[0]);
        transformProject(p.v[1]);
        transformProject(p.v[2]);
        drawEMBMPoly(&p);
      }
    }
    blitToScreen();
    time += 0.01f;
  } while((inp(0x60)&0x80)!=0x00);
  setBiosGraphicsMode(gfxMode);
  delete[] frameBuffer;
  delete[] zBuffer;
  return 0;
}
