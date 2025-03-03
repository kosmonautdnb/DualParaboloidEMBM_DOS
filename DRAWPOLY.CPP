#include "drawpoly.hpp"
#include "polys.hpp"
#include "matrix.hpp"

extern int frameBufferWidth;
extern int frameBufferHeight;
extern unsigned int *frameBuffer;
extern float *zBuffer;

extern int textureWidth;
extern int textureHeight;
extern unsigned int *texture;

extern int textureWidth2;
extern int textureHeight2;
extern unsigned int *texture2;

extern Matrix envyMatrix;

__inline float getBary(int x, int y, const Vertex &v0, const Vertex &v1, const Vertex &v2) {
  float dx = v2.sx - v1.sx;
  float dy = v2.sy - v1.sy;
  float nx = dy;
  float ny = -dx;
  float nl = sqrt(nx*nx+ny*ny);
  if (nl > 0) {
    nx /= nl;
    ny /= nl;
  }
  float l = (v0.sx-v1.sx)*nx + (v0.sy-v1.sy)*ny;
  float p = (x-v1.sx)*nx + (y-v1.sy)*ny;
  return p/l;
}

__inline int colorFloatToInt(float v) {
  int r = (int)floor(v*255.f);
  if (r < 0) r = 0;
  if (r > 255) r = 255;
  return r;
}

bool backfacing(const Vertex &v0, const Vertex &v1, const Vertex &v2) {
  return false;
  const float dx0 = v1.sx - v0.sx;
  const float dy0 = v1.sy - v0.sy;
  const float dx1 = v2.sx - v0.sx;
  const float dy1 = v2.sy - v0.sy;
  return (dx0*dy1 - dy0*dx1)>=0;
}

void drawFlatPoly(Poly *poly) {
  __POLY_BACKFACE_CULL_RETURN__(poly)
  __POLYMINMAX__(poly);
  __FRAMEBUFFERCLAMP__;
  __RETURN_ON_FULLYCLAMPED__

  float v0z = 1.f / poly->v[0].p.z;
  float v1z = 1.f / poly->v[1].p.z;
  float v2z = 1.f / poly->v[2].p.z;

  float iz;
  unsigned int rgba = poly->c[0].rgba32();

  __PAINTPOLYQUAD_BEGINY__(poly)
  __PAINTPOLYQUAD_INITBARYFORX__(poly)
  __PAINTPOLYQUAD_BEGINX__
  __PAINTPOLYQUAD_BARY_IF__ {
    iz = 1.f/(__BARY0__(v0z)+__BARY1__(v1z)+__BARY2__(v2z));
    __PAINTPOLYQUAD_ZBUFFER_IF__(iz) {
      __PAINTPOLYQUAD_SETPIXEL__(rgba);
    }
  }
  __PAINTPOLYQUAD_ENDX__
  __PAINTPOLYQUAD_ENDY__
}

void drawGouraudPoly(Poly *poly) {
  __POLY_BACKFACE_CULL_RETURN__(poly)
  __POLYMINMAX__(poly);
  __FRAMEBUFFERCLAMP__;
  __RETURN_ON_FULLYCLAMPED__

  float v0z = 1.f / poly->v[0].p.z;
  float v1z = 1.f / poly->v[1].p.z;
  float v2z = 1.f / poly->v[2].p.z;

  float r0 = poly->v[0].c[0].red * v0z;
  float g0 = poly->v[0].c[0].green * v0z;
  float b0 = poly->v[0].c[0].blue * v0z;
  float a0 = poly->v[0].c[0].alpha * v0z;

  float r1 = poly->v[1].c[0].red * v1z;
  float g1 = poly->v[1].c[0].green * v1z;
  float b1 = poly->v[1].c[0].blue * v1z;
  float a1 = poly->v[1].c[0].alpha * v1z;

  float r2 = poly->v[2].c[0].red * v2z;
  float g2 = poly->v[2].c[0].green * v2z;
  float b2 = poly->v[2].c[0].blue * v2z;
  float a2 = poly->v[2].c[0].alpha * v2z;

  float iz;
  int r,g,b,a;
  __PAINTPOLYQUAD_BEGINY__(poly)
  __PAINTPOLYQUAD_INITBARYFORX__(poly)
  __PAINTPOLYQUAD_BEGINX__
  __PAINTPOLYQUAD_BARY_IF__ {
    iz = 1.f/(__BARY0__(v0z)+__BARY1__(v1z)+__BARY2__(v2z));
    __PAINTPOLYQUAD_ZBUFFER_IF__(iz) {
      r = colorFloatToInt((__BARY0__(r0)+__BARY1__(r1)+__BARY2__(r2))*iz);
      g = colorFloatToInt((__BARY0__(g0)+__BARY1__(g1)+__BARY2__(g2))*iz);
      b = colorFloatToInt((__BARY0__(b0)+__BARY1__(b1)+__BARY2__(b2))*iz);
      a = colorFloatToInt((__BARY0__(a0)+__BARY1__(a1)+__BARY2__(a2))*iz);
      __PAINTPOLYQUAD_SETPIXEL__(r|(g<<8)|(b<<16)|(a<<24));
    }
  }
  __PAINTPOLYQUAD_ENDX__
  __PAINTPOLYQUAD_ENDY__
}

void drawTexturedPoly(Poly *poly) {
  __POLY_BACKFACE_CULL_RETURN__(poly)
  __POLYMINMAX__(poly);
  __FRAMEBUFFERCLAMP__;
  __RETURN_ON_FULLYCLAMPED__

  float v0z = 1.f / poly->v[0].p.z;
  float v1z = 1.f / poly->v[1].p.z;
  float v2z = 1.f / poly->v[2].p.z;

  float u0 = poly->v[0].u[0] * v0z;
  float v0 = poly->v[0].v[0] * v0z;

  float u1 = poly->v[1].u[0] * v1z;
  float v1 = poly->v[1].v[0] * v1z;

  float u2 = poly->v[2].u[0] * v2z;
  float v2 = poly->v[2].v[0] * v2z;

  int tWidth = textureWidth;
  int tHeight = textureHeight;

  float iz;
  int u,v;
  __PAINTPOLYQUAD_BEGINY__(poly)
  __PAINTPOLYQUAD_INITBARYFORX__(poly)
  __PAINTPOLYQUAD_BEGINX__
  __PAINTPOLYQUAD_BARY_IF__ {
    iz = 1.f/(__BARY0__(v0z)+__BARY1__(v1z)+__BARY2__(v2z));
    __PAINTPOLYQUAD_ZBUFFER_IF__(iz) {
      u = ((int)floor((__BARY0__(u0)+__BARY1__(u1)+__BARY2__(u2))*iz*textureWidth)+tWidth*256) % tWidth;
      v = ((int)floor((__BARY0__(v0)+__BARY1__(v1)+__BARY2__(v2))*iz*textureHeight)+tHeight*256) % tHeight;
      __PAINTPOLYQUAD_SETPIXEL__(texture[u+v*textureWidth]);
    }
  }
  __PAINTPOLYQUAD_ENDX__
  __PAINTPOLYQUAD_ENDY__
}

void drawEnvyPoly(Poly *poly) {
  __POLY_BACKFACE_CULL_RETURN__(poly)
  __POLYMINMAX__(poly);
  __FRAMEBUFFERCLAMP__;
  __RETURN_ON_FULLYCLAMPED__

  float v0z = 1.f / poly->v[0].p.z;
  float v1z = 1.f / poly->v[1].p.z;
  float v2z = 1.f / poly->v[2].p.z;

  float nx0 = poly->v[0].n.x * v0z;
  float ny0 = poly->v[0].n.y * v0z;
  float nz0 = poly->v[0].n.z * v0z;

  float nx1 = poly->v[1].n.x * v1z;
  float ny1 = poly->v[1].n.y * v1z;
  float nz1 = poly->v[1].n.z * v1z;

  float nx2 = poly->v[2].n.x * v2z;
  float ny2 = poly->v[2].n.y * v2z;
  float nz2 = poly->v[2].n.z * v2z;

  float iz;
  int u,v;
  __PAINTPOLYQUAD_BEGINY__(poly)
  __PAINTPOLYQUAD_INITBARYFORX__(poly)
  __PAINTPOLYQUAD_BEGINX__
  __PAINTPOLYQUAD_BARY_IF__ {
    iz = 1.f/(__BARY0__(v0z)+__BARY1__(v1z)+__BARY2__(v2z));
    __PAINTPOLYQUAD_ZBUFFER_IF__(iz) {
      Vector n;
      n.x = __BARY0__(nx0)+__BARY1__(nx1)+__BARY2__(nx2);
      n.y = __BARY0__(ny0)+__BARY1__(ny1)+__BARY2__(ny2);
      n.z = __BARY0__(nz0)+__BARY1__(nz1)+__BARY2__(nz2);
      n.normalize();
      n = transform3x3(n,envyMatrix);
      int u = n.x/(n.z+2.0)*textureWidth/2+textureWidth/2;
      int v = n.y/(n.z+2.0)*textureHeight/2+textureHeight/2;
      if (u<0) u = 0;
      if (v<0) v = 0;
      if (u>textureWidth-1) u = textureWidth-1;
      if (v>textureHeight-1) v = textureHeight-1;
      __PAINTPOLYQUAD_SETPIXEL__(texture[u+v*textureWidth]);
    }
  }
  __PAINTPOLYQUAD_ENDX__
  __PAINTPOLYQUAD_ENDY__
}

void drawEMBMPoly(Poly *poly) {
  __POLY_BACKFACE_CULL_RETURN__(poly)
  __POLYMINMAX__(poly);
  __FRAMEBUFFERCLAMP__;
  __RETURN_ON_FULLYCLAMPED__

  float v0z = 1.f / poly->v[0].p.z;
  float v1z = 1.f / poly->v[1].p.z;
  float v2z = 1.f / poly->v[2].p.z;

  float nx0 = poly->v[0].n.x * v0z;
  float ny0 = poly->v[0].n.y * v0z;
  float nz0 = poly->v[0].n.z * v0z;

  float nx1 = poly->v[1].n.x * v1z;
  float ny1 = poly->v[1].n.y * v1z;
  float nz1 = poly->v[1].n.z * v1z;

  float nx2 = poly->v[2].n.x * v2z;
  float ny2 = poly->v[2].n.y * v2z;
  float nz2 = poly->v[2].n.z * v2z;

  float u0 = poly->v[0].u[0] * v0z;
  float v0 = poly->v[0].v[0] * v0z;

  float u1 = poly->v[1].u[0] * v1z;
  float v1 = poly->v[1].v[0] * v1z;

  float u2 = poly->v[2].u[0] * v2z;
  float v2 = poly->v[2].v[0] * v2z;

  float iz;
  int u,v;
  __PAINTPOLYQUAD_BEGINY__(poly)
  __PAINTPOLYQUAD_INITBARYFORX__(poly)
  __PAINTPOLYQUAD_BEGINX__
  __PAINTPOLYQUAD_BARY_IF__ {
    iz = 1.f/(__BARY0__(v0z)+__BARY1__(v1z)+__BARY2__(v2z));
    __PAINTPOLYQUAD_ZBUFFER_IF__(iz) {
      int ut = (int)floor((__BARY0__(u0)+__BARY1__(u1)+__BARY2__(u2))*iz*textureWidth2);
      int vt = (int)floor((__BARY0__(v0)+__BARY1__(v1)+__BARY2__(v2))*iz*textureHeight2);
      if (ut<0) ut = 0;
      if (vt<0) vt = 0;
      if (ut>textureWidth2-1) ut = textureWidth2-1;
      if (vt>textureHeight2-1) vt = textureHeight2-1;
      unsigned int embm = texture2[ut+vt*textureWidth2];
      Vector n;
      n.x = __BARY0__(nx0)+__BARY1__(nx1)+__BARY2__(nx2);
      n.y = __BARY0__(ny0)+__BARY1__(ny1)+__BARY2__(ny2);
      n.z = __BARY0__(nz0)+__BARY1__(nz1)+__BARY2__(nz2);
      n.normalize();
      n = transform3x3(n,envyMatrix);
      int u = n.x/(n.z+2.0)*textureWidth/2+textureWidth/2;
      int v = n.y/(n.z+2.0)*textureHeight/2+textureHeight/2;
      float s = 1.0;
      u += ((int)(embm & 255)-128)*s;
      v += (((int)(embm>>8) & 255)-128)*s;
      if (u<0) u = 0;
      if (v<0) v = 0;
      if (u>textureWidth-1) u = textureWidth-1;
      if (v>textureHeight-1) v = textureHeight-1;
      __PAINTPOLYQUAD_SETPIXEL__(texture[u+v*textureWidth]);
    }
  }
  __PAINTPOLYQUAD_ENDX__
  __PAINTPOLYQUAD_ENDY__
}
