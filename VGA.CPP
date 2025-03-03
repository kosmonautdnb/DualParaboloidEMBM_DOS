#include <i86.h>
#include <conio.h>

unsigned int palette[256] = {0};

void setBiosGraphicsMode(int mode) {
    union REGS regs;
    regs.x.eax = mode;
    int386(0x10, &regs, &regs);
}

int getBiosGraphicsMode() {
    union REGS regs;
    regs.w.ax = 0x0f00;
    int386 (0x10, &regs, &regs);
    return regs.h.al;
}

void setBiosCursor(int x, int y) {
    union REGS regs;
    regs.x.eax = 0x200;
    regs.h.bh = 0;
    regs.h.dl = x;
    regs.h.dh = y;
    int386(0x10, &regs, &regs);
}

void setPalette(int index, unsigned int color) {
  outp(0x3c8,index);
  outp(0x3c9,(color & 255)>>2);
  outp(0x3c9,((color>>8) & 255)>>2);
  outp(0x3c9,((color>>16) & 255)>>2);
  palette[index] = color;
}

unsigned int getPalette(int index) {
  outp(0x3c8,index);
  int r = inp(0x3c9);
  int g = inp(0x3c9);
  int b = inp(0x3c9);
  return 0xff000000|(r<<2)|(g<<10)|(b<<18);
}

unsigned char getPaletteIndex(unsigned int rgba) {
  const int r = rgba & 255;rgba>>=8;
  const int g = rgba & 255;rgba>>=8;
  const int b = rgba & 255;rgba>>=8;
  int ld = 256*256*4;
  int ri = 0;
  for (int i = 0; i < 256; i++) {
    unsigned int pal = palette[i];
    const int r2 = pal & 255;pal >>= 8;
    const int g2 = pal & 255;pal >>= 8;
    const int b2 = pal & 255;pal >>= 8;
    const int d = (r2-r)*(r2-r)+(g2-g)*(g2-g)+(b2-b)*(b2-b);
    if (d < ld) {
      ld = d;
      ri = i;
    }
  }
  return ri;
}
