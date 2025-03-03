#ifndef __VGA_HPP__
#define __VGA_HPP__

extern unsigned int palette[256];

void setBiosGraphicsMode(int mode);
int  getBiosGraphicsMode();
void setBiosCursor(int x, int y);
void setPalette(int index, unsigned int color);
unsigned char getPaletteIndex(unsigned int rgba);

#endif //__VGA_HPP__