 
#include "vga.h"
#include "segment.h"
#include "palette.h"

int glyph[11][9] = {
	{ 1,0,1,1,1,1,1}, /* zero  */
	{ 0,0,0,0,0,1,1}, /* one   */
	{ 1,1,1,1,0,0,1}, /* two   */
	{ 1,1,1,0,0,1,1}, /* three */
	{ 0,1,0,0,1,1,1}, /* four  */
	{ 1,1,1,0,1,1,0}, /* five  */
	{ 1,1,1,1,1,1,0}, /* six   */
	{ 1,0,0,0,1,1,1}, /* seven */
	{ 1,1,1,1,1,1,1}, /* eight */
	{ 1,1,1,0,1,1,1}, /* nine  */
    { 0,0,0,0,0,0,0}  /* empty */
	};

void segment(int a[7], int offset,int on_color, int off_color)
{
 byte i;
 struct RGB_Color pal;
 struct RGB_Color off;
 off.red=0;
 off.green=off_color;
 off.blue=off_color;
 
 pal.red=0;
 pal.green=on_color;
 pal.blue=on_color;

 for (i=0; i<=7;i++) {

  if (a[i]>0) setPaletteRegister(i+offset,&pal); else setPaletteRegister(i+offset,&off);

}

}
 

void drawSegmentSize(int x,int y,int iX_max,int jY_max, byte color, byte rotation)
{
int iX,jY;
byte iX_max_=iX_max,jY_max_=jY_max;
if (rotation > 0) { iX_max=jY_max_;jY_max=iX_max_; }



for (iX=0; iX<iX_max; iX++) {
	for (jY=0; jY<jY_max; jY++) {

		if  ( ( (x+iX)>0 &&  (y+jY) > 0 ) && ( (x+iX) < X_MAX ) && (y+jY) < Y_MAX )  plot_pixel_fast(x+iX,y+jY,color);
	}


}
}

void drawSegment(int x, int y, byte color, byte rotation)
{
drawSegmentSize(x,y,27,6,color,rotation);
}

void drawSmallNumber(int x, int y, byte size, int color)
{

//byte size=15;
byte Xsize=size*4;
byte Ysize=size;
//byte gap = size / 2;
//byte gap=1;

 drawSegmentSize(x+Ysize, y-Ysize, Xsize, Ysize,	      color,0);
 drawSegmentSize(x+Ysize,y+Xsize,Xsize ,Ysize ,	      color+1,0);
 drawSegmentSize(x+Ysize, y+Xsize+Xsize+Ysize, Xsize, Ysize,color+2,0);
 drawSegmentSize(x,y+Xsize+Ysize,  Xsize,Ysize ,	      color+3,1);
 drawSegmentSize(x            ,    y,Xsize,Ysize,	      color+4,1);
 drawSegmentSize(x+Xsize+Ysize, y+Xsize+Ysize, Xsize ,Ysize ,color+5,1);
 drawSegmentSize(x+Xsize+Ysize,y, Xsize,Ysize ,color+6,1);



}
void drawNumber(int x, int y, int color)
{

x=x-15;
y=y-58;


drawSegment(15+x, 58+y,  color,0);
drawSegment(15+x, 93+y,  color+1,0);
drawSegment(15+x, 130+y, color+2,0);

drawSegment( 8+x, 101+y, color+3,1);
drawSegment( 8+x,  65+y, color+4,1);
drawSegment(43+x, 101+y, color+5,1);
drawSegment(43+x,  65+y, color+6,1);



}
