

/*                                     */
/*      SEGMENT CLOCK FOR DOS          */
/*      BY DMITRY GERASIMUK, 2020      */
/*      github.com/dmitrygerasimuk     */
/*      Borland C++ 3.1                */

/* 4 */


#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>
#include "vga.h"
#include "palette.h"
#include "segment.h"

 void printTime(struct dostime_t *t)
{
byte h1,h2,m1,m2,s1,s2;
byte flash_h1,flash_h2,flash_m1,flash_m2,flash_s1;
byte luminance;
luminance=( (t -> hsecond / 4) % 0xFF )*3;
if ( luminance > 127 ) luminance = 120;
if (t -> hsecond > 80) luminance = 126;

h1 = t->hour / 10; h2 = t->hour % 10;
m1 = t->minute / 10;m2 = t->minute % 10;
s1 = t->second / 10;s2 = t->second % 10;

if ( s2==0 ) flash_s1=luminance; else flash_s1=0xFF;
if ( s1==0 && s2==0 ) flash_m2=luminance; else flash_m2=0xFF;
if ( m2==0 && s2==0 && s1==0) flash_m1=luminance; else flash_m1=0xFF;
if ( m1==0 && m2==0 && s2==0 && s1==0) flash_h2=luminance; else flash_h2=0xFF;
if ( h2==0 && m2==0 && s2==0 && s1==0) flash_h1=luminance; else flash_h1=0xFF;

segment(glyph[h1],22,    flash_h1,0x02 );
segment(glyph[h2],32,    flash_h2,0x02 );
segment(glyph[m1],42,    flash_m1,0x02 );
segment(glyph[m2],52,flash_m2 ,0x02 );
segment(glyph[s1],62,0xFF,0x02);
segment(glyph[s2],72,0xFF,0x02);
/*
segment(glyph[10],82,dark_glitch,dark_glitch);
segment(glyph[10],92,flash_h1,darkch(t->hsecond + 2) % 0x02 );
segment(glyph[10],102,flash_h1,darkch(t->hsecond + 4) % 0x02);
segment(glyph[10],112,flash_h1,darkch(t->hsecond + 1) % 0x02);
segment(glyph[10],122,flash_h1,darkch(t->hsecond + 2) % 0x02);
segment(glyph[10],132,flash_h1,darkch(t->hsecond + 4) % 0x02);
*/

//printf(" %d-",dark_glitch);
}

int main() {

int c; int i,j;
struct dostime_t t;
struct RGB_Color col;
setMode(VGA_256_COLOR_MODE);

turnOffPalette();
waitRetrace();



/*
drawNumber(15,-35,82);
drawNumber(67,-30,92);
drawNumber(119,-30,102);
drawNumber(171,-30,112);
drawNumber(223,-30,122);
drawNumber(275,-30,132);

drawNumber(15,150,82);
drawNumber(67, 150,92);
drawNumber(119,150,102);
drawNumber(171,150,112);
drawNumber(223,150,122);
drawNumber(275,150,132);

*/
 

drawSmallNumber(15 ,4 ,2,72);
 

 while ( c > 127) {
 _dos_gettime(&t);
 waitRetrace();
 printTime(&t);

if (kbhit()) {
   c = getch();
 if (c == 72) {   c=0;} 

}

}

    setMode(TEXT_MODE);

    printf("keycode: %d\n",c);
return 0;


}


