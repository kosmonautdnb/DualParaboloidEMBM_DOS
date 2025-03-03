#ifndef __FLATPOLY_HPP__
#define __FLATPOLY_HPP__

#include "polys.hpp"

float getBary(int x, int y, const Vertex &v0, const Vertex &v1, const Vertex &v2);
int colorFloatToInt(float v);

void drawFlatPoly(Poly *poly);
void drawGouraudPoly(Poly *poly);
void drawTexturedPoly(Poly *poly);
void drawEnvyPoly(Poly *poly);
void drawEMBMPoly(Poly *poly);

#define __POLY_BACKFACE_CULL_RETURN__(__p__) if (backfacing((__p__)->v[0],(__p__)->v[1],(__p__)->v[2])) return;

#define __POLYMINMAX__(__p__) \
  int pminx,pminy,pmaxx,pmaxy;\
  for (int i = 0; i < 3; i++) {\
    if (i==0||(int)floor((__p__)->v[i].sx)<pminx) pminx=(int)floor((__p__)->v[i].sx);\
    if (i==0||(int)floor((__p__)->v[i].sy)<pminy) pminy=(int)floor((__p__)->v[i].sy);\
    if (i==0||(int)floor((__p__)->v[i].sx)>pmaxx) pmaxx=(int)floor((__p__)->v[i].sx);\
    if (i==0||(int)floor((__p__)->v[i].sy)>pmaxy) pmaxy=(int)floor((__p__)->v[i].sy);\
  }\
  pmaxx++;\
  pmaxy++;

#define __FRAMEBUFFERCLAMP__\
  bool fullyClipped = false;\
  if (pminx<0) pminx=0;\
  if (pmaxx<=0) {fullyClipped=true;pmaxx=0;}\
  if (pminy<0) pminy=0;\
  if (pmaxy<=0) {fullyClipped=true;pmaxy=0;}\
  if (pminx>=frameBufferWidth) {fullyClipped=true;pminx=frameBufferWidth-1;}\
  if (pmaxx>frameBufferWidth) pmaxx=frameBufferWidth;\
  if (pminy>=frameBufferHeight) {fullyClipped=true;pminy=frameBufferHeight-1;}\
  if (pmaxy>frameBufferHeight) pmaxy=frameBufferHeight;

#define __RETURN_ON_FULLYCLAMPED__ if (fullyClipped) return;


#define __PAINTPOLYQUAD_BEGINY__(__p__)\
  int x,y;\
  float bary0,bary1;\
  float baryAdd0,baryAdd1;\
  unsigned int*pDest;\
  float *zDest;\
  for (y = pminy;y<pmaxy;y++) {

#define __PAINTPOLYQUAD_INITBARYFORX__(__p__) \
  bary0 = getBary(pminx,y,(__p__)->v[0],(__p__)->v[1],(__p__)->v[2]);\
  bary1 = getBary(pminx,y,(__p__)->v[1],(__p__)->v[2],(__p__)->v[0]);\
  baryAdd0 = (getBary(pmaxx,y,(__p__)->v[0],(__p__)->v[1],(__p__)->v[2])-bary0)/(pmaxx-pminx);\
  baryAdd1 = (getBary(pmaxx,y,(__p__)->v[1],(__p__)->v[2],(__p__)->v[0])-bary1)/(pmaxx-pminx);
  
#define __PAINTPOLYQUAD_BEGINX__\
  pDest = &frameBuffer[pminx+y*frameBufferWidth];\
  zDest = &zBuffer[pminx+y*frameBufferWidth];\
  int dminx = pminx;\
  int dmaxx = pmaxx;\
  for (x = dminx;x<dmaxx;x++) {

#define __PAINTPOLYQUAD_BARY_IF__ if (bary0>=0.f&&bary1>=0.f&&(bary0+bary1)<1.f)
#define __PAINTPOLYQUAD_ZBUFFER_IF__(__z__) if (*zDest>(__z__) && (*zDest=(__z__))) 


#define __BARY0__(__v__) ((__v__)*bary0)
#define __BARY1__(__v__) ((__v__)*bary1)
#define __BARY2__(__v__) ((__v__)*(1.f-bary0-bary1))

#define __PAINTPOLYQUAD_SETPIXEL__(__rgba__) *pDest = (__rgba__);

#define __PAINTPOLYQUAD_ENDX__\
    pDest++;\
    zDest++;\
    bary0+=baryAdd0;\
    bary1+=baryAdd1;\
    }

#define __PAINTPOLYQUAD_ENDY__\
  }

#endif //__FLATPOLY_HPP__
