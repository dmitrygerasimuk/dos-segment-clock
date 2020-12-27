#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
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

void draw_segment(int x, int y, byte color, byte rotation)
{
int i,j;
int i_max=27,j_max=6;
if (rotation > 0) { i_max=6;j_max=27; }

for (i=0; i<i_max; i++) {
	for (j=0; j<j_max; j++) {

		if  ( ( (x+i)>0 &&  (y+j) > 0 ) && ( (x+i) < X_MAX ) && (y+j) < Y_MAX ) plot_pixel_fast(x+i,y+j,color);
	 }
  }


}


void draw_number(int x, int y, int color)
{

x=x-15;
y=y-58;


draw_segment(15+x, 58+y,  color,0);
draw_segment(15+x, 93+y,  color+1,0);
draw_segment(15+x, 130+y, color+2,0);

draw_segment( 8+x, 101+y, color+3,1);
draw_segment( 8+x,  65+y, color+4,1);
draw_segment(43+x, 101+y, color+5,1);
draw_segment(43+x,  65+y, color+6,1);



}
