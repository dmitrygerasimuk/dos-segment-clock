#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#define X_MAX 320
#define Y_MAX 200
#define PALETTE_MASK  0x3C6
#define PALETTE_READ  0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_INDEX       0x03c8
#define PALETTE_DATA        0x03c9
#define VRETRACE	0x08
#define INPUT_STATUS	0x03da
#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define WRITE_DOT           0x0C      /* BIOS func to plot a pixel. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */
#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define NUM_COLORS          256       /* number of colors in mode 0x13 */


typedef unsigned char  byte;
typedef unsigned short word;
byte *VGA=(byte *)0xA0000000L;        /* this points to video memory. */
word *my_clock=(word *)0x0000046C;    /* this points to the 18.2hz system
					 */
 int stop;
 byte glyph[11][9] = {
	{ 1,0,1,1,1,1,1},
	{ 0,0,0,0,0,1,1},
	{ 1,1,1,1,0,0,1}, //two
	{ 1,1,1,0,0,1,1},
	{ 0,1,0,0,1,1,1},
	{ 1,1,1,0,1,1,0}, //five
	{ 1,1,1,1,1,1,0},
	{ 1,0,0,0,1,1,1},
	{ 1,1,1,1,1,1,1},
	{ 1,1,1,0,1,1,1}, //nine
	{ 0,0,0,0,0,0,0}
	};

 struct RGB_Color {

      unsigned char red;
      unsigned char green;
      unsigned char blue;
  };

void wait_retrace(void)
{


while ((inp(INPUT_STATUS) & VRETRACE)) {};
while (!(inp(INPUT_STATUS) & VRETRACE)) {};
}

void blue_palette();


   void SetPaletteRegister(int index, struct RGB_Color *color)
  {
      /* Disable interrupts */
      /* Okay to remove this line or substitute */
      /* asm cli; */
      disable();

      outp(PALETTE_MASK, 0xFF);

      /* Send color palette register index to write to */
      outp(PALETTE_WRITE, index);

      /* Write the color palette entry */
      outp(PALETTE_DATA, color->red);
      outp(PALETTE_DATA, color->green);
      outp(PALETTE_DATA, color->blue);

      /* Enable interrupts */
      /* Okay to remove this line or substitute */
      /* asm sti; */
      enable();
  }

  /* Gets the palette register value */
  void GetPaletteRegister(int index, struct RGB_Color *color)
  {
      /* Disable interrupts */
      /* Okay to remove this line or substitute */
      /* asm cli; */
      disable();

      outp(PALETTE_MASK, 0xFF);

      /* Write color palette register index to read from */
      outp(PALETTE_READ, index);

      /* Read color palette entry */
      color->red   = inp(PALETTE_DATA);
      color->green = inp(PALETTE_DATA);
      color->blue  = inp(PALETTE_DATA);

      /* Enable interrupts */
      /* Okay to remove this line or substitute */
      /* asm sti; */
      enable();
  }


void set_mode(byte mode)
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



void cycleColors(struct RGB_Color *c,int reg)
{
  for(c->red=0; c->red<64; c->red++)
      for(c->blue=0; c->blue<64; c->blue++)
	for(c->green=0; c->green<64; c->green++) {
	  SetPaletteRegister(reg, c);
	}


}

void draw_segment_size(int x,int y,int i_max,int j_max, byte color, byte rotation)
{
int i,j;
byte i_max_=i_max,j_max_=j_max;
if (rotation > 0) { i_max=j_max_;j_max=i_max_; }



for (i=0; i<i_max; i++) {
	for (j=0; j<j_max; j++) {

		if  ( ( (x+i)>0 &&  (y+j) > 0 ) && ( (x+i) < X_MAX ) && (y+j) < Y_MAX )  plot_pixel_fast(x+i,y+j,color);
	}


}
}

void draw_segment(int x, int y, byte color, byte rotation)
{
draw_segment_size(x,y,27,6,color,rotation);
}




void segment(byte a[7], int offset,int on_color, int off_color)
{
 int i;
 struct RGB_Color pal;
 struct RGB_Color off;
 off.red=0;off.green=off_color;off.blue=off_color;
 pal.red=0;
 pal.green=on_color;
 pal.blue=on_color;

 for (i=0; i<=7;i++) {

  if (a[i]>0) SetPaletteRegister(i+offset,&pal); else SetPaletteRegister(i+offset,&off);

}




}

void blue_palette(int i)

{
int b;
struct RGB_Color a;
a.red=0;
a.green=i;
a.blue=0;


for (b=1; b<=10;b++) {
SetPaletteRegister(b,&a);
}
}

void draw_small_number(int x, int y, byte size, int color)
{

//byte size=15;
byte Xsize=size*4;
byte Ysize=size;
//byte gap = size / 2;
//byte gap=1;

 draw_segment_size(x+Ysize, y-Ysize, Xsize, Ysize,	      color,0);
 draw_segment_size(x+Ysize,y+Xsize,Xsize ,Ysize ,	      color+1,0);
 draw_segment_size(x+Ysize, y+Xsize+Xsize+Ysize, Xsize, Ysize,color+2,0);
 draw_segment_size(x,y+Xsize+Ysize,  Xsize,Ysize ,	      color+3,1);
 draw_segment_size(x            ,    y,Xsize,Ysize,	      color+4,1);
 draw_segment_size(x+Xsize+Ysize, y+Xsize+Ysize, Xsize ,Ysize ,color+5,1);
 draw_segment_size(x+Xsize+Ysize,y, Xsize,Ysize ,color+6,1);



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


int lightch(int lol) {

randomize();
return ( 70 + lol + random(3324) ) % 126;


}

void reset_segs()
{
 int i,j;

 for (i=0; i<8; i++) {
	randomize();
	for (j=0;j<15;j++) {

		draw_small_number(15+(20*j),4+(29*i),1,90+random(j-i)*10+2);
	}
 }


}

int darkch(int lol) {

randomize();

return ( lol + random(10) ) % 0x05;


}

void reset_glyphs()
{
int i,j;

for (i=0;i<20;i++){
 segment(glyph[random(10)] ,i*10+2,0xFF,0x02 );
}

}


void print_time(struct dostime_t *t)
{
byte h1,h2,m1,m2,s1,s2,ms1,ms2;
byte flash_h1,flash_h2,flash_m1,flash_m2,flash_s1,flash_ms1,flash_ms2;
byte luminance;
int dark_glitch;
randomize();
dark_glitch=darkch(t->hsecond);
luminance=( (t -> hsecond / 4) % 0xFF )*3;


//luminance=127;

if ( luminance > 127 ) luminance = 120;
if (t -> hsecond > 80) luminance = 126;




h1 = t->hour / 10; h2 = t->hour % 10;
m1 = t->minute / 10;m2 = t->minute % 10;
s1 = t->second / 10;s2 = t->second % 10;
ms1 = t->hsecond / 10;ms2 = t->hsecond % 10;


if ( s2==0 ) flash_s1=luminance; else flash_s1=0xFF;
if ( s1==0 && s2==0 ) flash_m2=luminance; else flash_m2=0xFF;


if ( m2==0 && s2==0 && s1==0) flash_m1=luminance; else flash_m1=0xFF;

if ( m1==0 && m2==0 && s2==0 && s1==0) flash_h2=luminance; else flash_h2=0xFF;
if ( h2==0 && m2==0 && s2==0 && s1==0) flash_h1=luminance; else flash_h1=0xFF;
//if ( h1==0 ) flash_h1=luminance; else flash_h1=0xFF;

{

segment(glyph[0] ,2,    flash_h1,darkch(t->hsecond * 2+  2) % 0x02 );
segment(glyph[0] ,12,    flash_h1,darkch(t->hsecond * 2+  2) % 0x02 );

segment(glyph[h1],22,    flash_h1,darkch(t->hsecond * 2+  2) % 0x02 );
segment(glyph[h2],32,    flash_h2,darkch(t->hsecond *5 + 2) % 0x02 );
segment(glyph[m1],42,    flash_m1,darkch(t->hsecond * 8+ 3) % 0x02 );
segment(glyph[m2],52,flash_m2,darkch(t->hsecond * 10+ 4) % 0x02 );
segment(glyph[s1],62,flash_s1,darkch(t->hsecond*3 + 1) % 0x02);
segment(glyph[s2],72,luminance,darkch(t->hsecond/3 + 4) % 0x02);
segment(glyph[ms1],82,26,darkch(t->hsecond/3 + 4) % 0x02);
segment(glyph[ms2],92,26,darkch(t->hsecond/3 + 4) % 0x02);



segment(glyph[s1],112,10,darkch(t->hsecond*3 + 1) % 0x02);
segment(glyph[s2],122,10,darkch(t->hsecond/3 + 4) % 0x02);
segment(glyph[ms1],132,10,darkch(t->hsecond/3 + 4) % 0x02);
segment(glyph[ms2],142,10,darkch(t->hsecond/3 + 4) % 0x02);
segment(glyph[s1],152,10      ,darkch(t->hsecond*3 + 1) % 0x02);
segment(glyph[s2],162,10       ,darkch(t->hsecond/3 + 4) % 0x02);
segment(glyph[ms1],172,10,darkch(t->hsecond/3 + 4) % 0x02);
segment(glyph[ms2],182,10,darkch(t->hsecond/3 + 4) % 0x02);
segment(glyph[ms1],192,11,darkch(t->hsecond/3 + 4) % 0x02);
segment(glyph[ms2],202,10,darkch(t->hsecond/3 + 4) % 0x02);
segment(glyph[ms1],212, 9,darkch(t->hsecond/3 + 4) % 0x02);
segment(glyph[ms2],102, 8,darkch(t->hsecond/3 + 4) % 0x02);


//segment(glyph[10],82,darkch(t->hsecond),darkch(t->hsecond + 6) % 0x02 );
//segment(glyph[10],92,darkch(t->hsecond+1),darkch(t->hsecond /3 + 2) % 0x02 );
//reset_segs();
//printf(" %d-",dark_glitch);
}
}


void draw_line(int x, int y, int size)
{
  int i;
  byte step=29;
  byte lines=8;
  x++;y++;size++;

  for (i=0;i<lines;i++) {
      randomize();
  draw_small_number(15,4+(step*i),1,random(10)*10+2);
  draw_small_number(35,4+(step*i),1,random(10)*10+2);
  draw_small_number(55, 4+(step  *i),1, random(10)*10+2);
  draw_small_number(75, 4+(  step*i),1,random(10)*10+2);
  draw_small_number(95, 4+(  step*i),1,random(10)*10+2);
  draw_small_number(115, 4+(  step*i),1,random(10)*10+2);
  draw_small_number(135, 4+(  step*i),1,122);
  draw_small_number(155, 4+(  step*i),1,132);
  draw_small_number(175, 4+(  step*i),1, 82);
  draw_small_number(195, 4+(  step*i),1, 92);
  draw_small_number(215, 4+(  step*i),1,162);
  draw_small_number(235, 4+(  step*i),1,172);
  draw_small_number(255, 4+(  step*i),1,182);
  draw_small_number(275, 4+(  step*i),1,192);
  draw_small_number(295, 4+(  step*i),1,162);



}
}
void draw_numbers()
{
draw_number(15,58, 22);
draw_number(67,58, 32);
draw_number(119, 58, 42);
draw_number(171,58,52);
draw_number(223,58,62);
draw_number(275,58,72);

}

int main() {

int c;
int i;
int exit;
//int one[7] = { 1,1,1,1,1,1,1 };
struct dostime_t t;



struct RGB_Color col;

set_mode(VGA_256_COLOR_MODE);
wait_retrace();


//draw_small_number(10,30,10,1);
//draw_small_number(110,30,9,1);

//draw_line(2,4,2);
//reset_segs();
//reset_glyphs();
draw_numbers();
/*

draw_number(15,-35,172);
draw_number(67,-30,142);
draw_number(119,-30,102);
draw_number(171,-30,112);
draw_number(223,-30,122);
draw_number(275,-30,132);

draw_number(15,150, 142);
draw_number(67, 150,152);
draw_number(119,150,162);
draw_number(171,150,172);
draw_number(223,150,182);
draw_number(275,150,192);
*/

/*
draw_small_number(15,4,2,92);
draw_small_number(35,4,2,62);
draw_small_number(55,4,2,72);
draw_small_number(75,4,2,32);
draw_small_number(95,4,2,92);
draw_small_number(115,4,2,82);
draw_small_number(135,4,2,92);
draw_small_number(155,4,2,82);
draw_small_number(175,4,2,72);
draw_small_number(195,4,2,62);
draw_small_number(215,4,2,72);
draw_small_number(235,4,2,72);
draw_small_number(255,4,2,82);
draw_small_number(275,4,2,72);
draw_small_number(295,4,2,62);
*/


//draw_small_number(250,30,2,92);
//draw_small_number(270,30,2,82);

 while ( exit != 1 ) {
 _dos_gettime(&t);


 if (stop != 0) wait_retrace();

 //reset_segs();



 if (stop != 1) print_time(&t);



if (kbhit()) {
 //  printf("keycode: %d\n",c);

c = getch();
 if (c == 27) { blue_palette(0xFF); c=0; }
 if (c == 49) { reset_segs(); }
  if (c == 50) { reset_glyphs(); }
  exit=0;
  switch (c) {
  case 32:     if (stop==0) stop=1; else stop=0;   break;

  case 49:     reset_glyphs(); break;
  case 50:     reset_segs(); draw_numbers(); break;

  case 51:     blue_palette(0xEE);break;
  case 0:                   break;
  default:    exit=1;
  exit=1;
}


}

}

    set_mode(TEXT_MODE);

    printf("keycode: %d\n",c);
    printf("lock: %d\n",*my_clock);

    getch();
return 0;


}

