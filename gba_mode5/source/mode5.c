/*
 * GBA Voxel Terrain sample
 * by takehiko.terada@gmail.com
 * 
 * This example terrain rendering using voxel data.
 */

#include "vlt_gba.h"
#include <stdlib.h>

// Controller buttons state
bool buttons[10];
typedef struct {
  vu16_t *front;
  vu16_t *back;
} gba_surface_t;

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

void CODE_IN_IWRAM *mallocAligned(u32_t size, int align)
{
	void *p = malloc(size + align);
	if ((u32_t)p % align) p += ((u32_t)p % align);
	return p;
}


void CODE_IN_IWRAM drawLineEFLA(i32_t x1, i32_t y1, i32_t x2, i32_t y2, u16_t color)
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
	buttons[0] = (~(*BUTTONS) & (1 << BUTTON_A));
	buttons[1] = (~(*BUTTONS) & (1 << BUTTON_B));
	buttons[2] = (~(*BUTTONS) & (1 << BUTTON_SELECT));
	buttons[3] = (~(*BUTTONS) & (1 << BUTTON_START));
	buttons[4] = (~(*BUTTONS) & (1 << BUTTON_RIGHT));
	buttons[5] = (~(*BUTTONS) & (1 << BUTTON_LEFT));
	buttons[6] = (~(*BUTTONS) & (1 << BUTTON_UP));
	buttons[7] = (~(*BUTTONS) & (1 << BUTTON_DOWN));
	buttons[8] = (~(*BUTTONS) & (1 << BUTTON_R));
	buttons[9] = (~(*BUTTONS) & (1 << BUTTON_L));
}

void CODE_IN_IWRAM clearBuffer(u16_t color)
{
  for (int i = 0; i < HEIGHT * WIDTH; i++)
    buffer[i] = color;
}

void swapBuffers(gba_surface_t *surface)
{
  vu16_t *tmp = surface->front;
  surface->front = surface->back;
  surface->back = tmp;
  buffer = surface->back;
  *(REG_DISPCNT) ^= 0x10;
}

void CODE_IN_IWRAM DMAFastCopy(void *source, void *dest, u32_t count, u32_t mode)
{
	REG_DMA3SAD = (u32_t)source;
	REG_DMA3DAD = (u32_t)dest;
	REG_DMA3CNT = count | mode;
}

int main()
{
  bool useDmaToClear = true;
  gba_surface_t surface;
  surface.front = VRAM_ADRS1;
  surface.back = VRAM_ADRS2;
  buffer = surface.back;
  xorshiftSeed(0x12345678); // Seed the random number generator

  *(REG_DISPCNT) = 0x0405; /* video mode 5 (0x05, 160x120 16bit), enable BG2(0x04) */
  *(REG_BG2PA) = 0;
  *(REG_BG2PB) = -128;
  *(REG_BG2PC) = 128;
  *(REG_BG2PD) = 0;
  *(REG_BG2X) = (120 << 8);
  *(REG_BG2Y) = (4 << 8);

  clear_color = 0x0000;
  clearBuffer(clear_color);
  swapBuffers(&surface);
  clearBuffer(clear_color);

  u32_t x1 = xorshift32() % WIDTH;
  u32_t y1 = xorshift32() % HEIGHT;
  u32_t x2 = xorshift32() % WIDTH;
  u32_t y2 = xorshift32() % HEIGHT;
  u32_t vx1 = 1;
  u32_t vy1 = 1;
  u32_t vx2 = -1;
  u32_t vy2 = -1;
  u16_t color = xorshift32() & 0xffff;

  while(1) {
		checkButtons();
		// Clear the draw surface
    if (buttons[BUTTON_A]) // A button
    {
      if (buttons[BUTTON_B]) // B button
      {
        useDmaToClear = false;
      }
      if (!useDmaToClear) {
        clearBuffer(clear_color);
      } else {
        DMAFastCopy(clear_color, buffer, WIDTH*HEIGHT, (DMA_SRC_FIXED | DMA_DST_INC | DMA_16 | DMA_ENABLE));
      }
    }

    clearBuffer(clear_color);
    drawLineEFLA(x1, y1, x2, y2, color);
    x1 += vx1; y1 += vy1; x2 += vx2; y2 += vy2;
    if (x1 >= WIDTH || x1 <= 0) vx1 = -vx1;
    if (y1 >= HEIGHT || y1 <= 0) vy1 = -vy1;
    if (x2 >= WIDTH || x2 <= 0) vx2 = -vx2;
    if (y2 >= HEIGHT || y2 <= 0) vy2 = -vy2;
 		waitForVBlank();
    swapBuffers(&surface);
		// DMAFastCopy(buffer, vram, WIDTH*HEIGHT, (DMA_IMMEDIATE | DMA_16 | DMA_ENABLE));
  }
}
