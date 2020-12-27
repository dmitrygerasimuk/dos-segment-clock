#ifndef SEGMENT_H
#define SEGMENT_H
typedef unsigned char  byte;
typedef unsigned short word;

#define X_MAX 320
#define Y_MAX 200

extern int glyph[11][9];


void segment(int a[7], int offset,int on_color, int off_color);
void draw_segment(int x, int y, byte color, byte rotation);
void draw_number(int x, int y, int color);

#endif