#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include "vga.h"


byte *VGA=(byte *)0xA0000000L; 
word *my_clock=(word *)0x0000046C;

void waitRetrace(void)
{
while ((inp(INPUT_STATUS) & VRETRACE)) {};
while (!(inp(INPUT_STATUS) & VRETRACE)) {};
}

void setMode(byte mode)
{
  union REGS regs;
  regs.h.ah = SET_MODE;
  regs.h.al = mode;
  int86(VIDEO_INT, &regs, &regs);
}

void plot_pixel_fast(int x,int y,byte color)
{
  VGA[y*SCREEN_WIDTH+x]=color;
}
