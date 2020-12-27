#ifndef VGA_H
#define VGA_H
#define X_MAX 320
#define Y_MAX 200
#define VRETRACE	  0x08
#define INPUT_STATUS  0x03da
#define VIDEO_INT     0x10          /* the BIOS video interrupt. */
#define WRITE_DOT     0x0C          /* BIOS func to plot a pixel. */
#define SET_MODE      0x00          /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE  0x13    /* use to set 256-color mode. */
#define TEXT_MODE     0x03      /* use to set 80x25 text mode. */
#define SCREEN_WIDTH  320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT 200       /* height in pixels of mode 0x13 */
#define NUM_COLORS    256       /* number of colors in mode 0x13 */

typedef unsigned char  byte;
typedef unsigned short word;
extern byte *VGA;      /* this points to video memory. */
extern word *my_clock;    /* this points to the 18.2hz system */

void waitRetrace(void);
void setMode(byte mode);
void plot_pixel_fast(int x,int y,byte color);// here comes your file DFH_lib.cpp content
 
#endif
