/*
 * GBA Graphics Example 01
 * by takehiko.terada@gmail.com
 * 
 * This example draw random lines on the screen.
 * Press A button to clear the screen.
 */

#include "vlt_gba.h"
#include <stdlib.h>

// Controller buttons state
bool buttons[10];

// Simple random number generator.
unsigned int _num = 0;
void CODE_IN_IWRAM xorshiftSeed(unsigned int seed)
{
	_num = seed;
}

unsigned int CODE_IN_IWRAM xorshift32()
{
	_num = _num ^ (_num << 13);
	_num = _num ^ (_num >> 17);
	_num = _num ^ (_num << 15);
	return _num;
}

void CODE_IN_IWRAM waitForVBlank()
{
#if 1
	while(!(*scanlineCounter));
	while(*scanlineCounter);
#else
    __asm
    {
     mov r0,#0x04000004
     wait_retrace:
       ldrh r1,[r0]
       tst r1,#1
     bne wait_retrace
    }
#endif
}

void CODE_IN_IWRAM *mallocAligned(u32 size, int align)
{
	void *p = malloc(size + align);
	if ((u32)p % align) p += ((u32)p % align);
	return p;
}


void CODE_IN_IWRAM drawLineEFLA(i32 x1, i32 y1, i32 x2, i32 y2, u16 color)
{
  bool yLonger = false;
  int shortLen = y2 - y1;
  int longLen = x2 - x1;
  int decInc;
#if 1
  if ((shortLen ^ (shortLen >> 31)) - (shortLen >> 31) > (longLen ^ (longLen >> 31)) - (longLen >> 31)) {
    shortLen ^= longLen;
    longLen ^= shortLen;
    shortLen ^= longLen;
    yLonger = true;
  }
#else
  if (abs(shortLen) > abs(longLen)) {
    int swap = shortLen;
    shortLen = longLen;
    longLen = swap;
    yLonger = true;
  }
#endif
  if (longLen == 0) decInc = 0;
  else decInc = (shortLen << 16) / longLen;

  if (yLonger) {
    if (longLen > 0) {
      longLen += y1;
      for (int j = 0x8000 + (x1 << 16); y1 <= longLen; ++y1) {
        SETPIXEL((j >> 16), y1, color);
        j+= decInc;
      }
      return;
    }
    longLen += y1;
    for (int j = 0x8000 + (x1 << 16); y1 >= longLen; --y1) {
      SETPIXEL((j >> 16), y1, color);
      j -= decInc;
    }
    return;
  }

  if (longLen > 0) {
    longLen += x1;
    for (int j = 0x8000 + (y1 << 16); x1 <= longLen; ++x1) {
      SETPIXEL(x1, (j >> 16), color);
      j += decInc;
    }
    return;
  }
  longLen += x1;
  for (int j = 0x8000 + (y1 << 16); x1 >= longLen; --x1) {
    SETPIXEL(x1, (j >> 16), color);
    j -= decInc;
  }
}

void CODE_IN_IWRAM checkButtons()
{
	buttons[0] = !((*BUTTONS) & BUTTON_A);
	buttons[1] = !((*BUTTONS) & BUTTON_B);
	buttons[2] = !((*BUTTONS) & BUTTON_SELECT);
	buttons[3] = !((*BUTTONS) & BUTTON_START);
	buttons[4] = !((*BUTTONS) & BUTTON_RIGHT);
	buttons[5] = !((*BUTTONS) & BUTTON_LEFT);
	buttons[6] = !((*BUTTONS) & BUTTON_UP);
	buttons[7] = !((*BUTTONS) & BUTTON_DOWN);
	buttons[8] = !((*BUTTONS) & BUTTON_R);
	buttons[9] = !((*BUTTONS) & BUTTON_L);
}

void CODE_IN_IWRAM DMAFastCopy(void *source, void *dest, u32 count, u32 mode)
{
	REG_DMA3SAD = (u32)source;
	REG_DMA3DAD = (u32)dest;
	REG_DMA3CNT = count | mode;
}


int main()
{
  vram = VRAM_ADRS;
  xorshiftSeed(0x12345678); // Seed the random number generator

  *(unsigned int*)0x04000000=0x0403; /* video mode 3 (0x03, 240x160 16bit), enable BG2(0x04) */
  ((unsigned short*)0x06000000)[120+80*240]=0x001f; /* Directly write to the framebuffer */
  ((unsigned short*)0x06000000)[136+80*240]=0x03e0;
  ((unsigned short*)0x06000000)[120+96*240]=0x7c00;

  buffer = (u16*)mallocAligned(WIDTH * HEIGHT * sizeof(u16), 4);
  clear_color = 0x0000;
	DMAFastCopy(clear_color, buffer, WIDTH*HEIGHT, (DMA_SRC_FIXED | DMA_DST_INC | DMA_16 | DMA_ENABLE));

  while(1) {
		checkButtons();
		// Clear the draw surface
    if (buttons[0]) // A button
    {
		  DMAFastCopy(clear_color, buffer, WIDTH*HEIGHT, (DMA_SRC_FIXED | DMA_DST_INC | DMA_16 | DMA_ENABLE));
    }

    u32 x1 = xorshift32() % WIDTH;
    u32 y1 = xorshift32() % HEIGHT;
    u32 x2 = xorshift32() % WIDTH;
    u32 y2 = xorshift32() % HEIGHT;
    u16 color = xorshift32() & 0xffff;
    drawLineEFLA(x1, y1, x2, y2, color);
 		waitForVBlank();
		DMAFastCopy(buffer, vram, WIDTH*HEIGHT, (DMA_IMMEDIATE | DMA_16 | DMA_ENABLE));
  }
}
