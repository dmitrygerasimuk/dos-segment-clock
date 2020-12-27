#ifndef PALETTE_H
#define PALETTE_H

#define PALETTE_MASK  0x3C6
#define PALETTE_READ  0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_INDEX 0x03c8
#define PALETTE_DATA  0x03c9

extern struct RGB_Color {
      unsigned char red;
      unsigned char green;
      unsigned char blue;
  };
void getPaletteRegister(int index, struct RGB_Color *color);
void turnOffPalette(void);
void setPaletteRegister(int index, struct RGB_Color *color);

#endif
