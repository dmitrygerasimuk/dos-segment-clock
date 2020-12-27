 
#include <dos.h>
#include "palette.h"

void setPaletteRegister(int index, struct RGB_Color *color)
  {
      /* Disable interrupts */
      /* asm cli; */
      disable();
      outp(PALETTE_MASK, 0xFF);
      outp(PALETTE_WRITE, index); /* Send color palette register index to write to */

      /* Write the color palette entry */
      outp(PALETTE_DATA, color->red);
      outp(PALETTE_DATA, color->green);
      outp(PALETTE_DATA, color->blue);

      /* Enable interrupts */
      /* asm sti; */
      enable();
  }

 /* Gets the palette register value */
  void getPaletteRegister(int index, struct RGB_Color *color)
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

void turnOffPalette(void) 
{

struct RGB_Color black;
int i;
black.red=0;black.green=0;black.blue=0;
for (i=0;i<=100;i++)
{
 setPaletteRegister(i,&black);
}

}






