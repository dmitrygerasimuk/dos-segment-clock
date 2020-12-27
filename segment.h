#ifndef SEGMENT_H
#define SEGMENT_H
typedef unsigned char  byte;
typedef unsigned short word;

#define X_MAX 320
#define Y_MAX 200

extern int glyph[11][9];


void segment(int a[7], int offset,int on_color, int off_color);
void drawSegmentSize(int x,int y,int iX_max,int jY_max, byte color, byte rotation);
void drawSegment(int x, int y, byte color, byte rotation);
void drawNumber(int x, int y, int color);
void drawSmallNumber(int x, int y, byte size, int color);

#endif