/*
 * GBA Graphics Example 01
 * by takehiko.terada@gmail.com
 * 
 * This example draw random lines on the screen.
 * Press A button to clear the screen.
 */

#include "vlt_gba.h"
#include "microvex.h"
#include <stdlib.h>

// Controller buttons state
bool buttons[10];

#define VERTEX_COUNT (8) // cube

enum {
  LINE_LIST = 0,
  LINE_STRIP = 1,
  END_OF_LIST = 2
} LINE_TYPE;

typedef struct {
  u8 type; // 0 = line list, 1 = line strip  
  u16 num_lines; // number of lines. In type 0, this is 2 * count. In type 1, this is count + 1.
  u16 *lines;
} line_list;

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

void create_cube(mvx_vec3_t *p, line_list *ll)
{
  int i;

  p[0].x = FLOAT2FIX(-1.0f); p[0].y = FLOAT2FIX(-1.0f); p[0].z = FLOAT2FIX(-1.0f);
  p[1].x = FLOAT2FIX(-1.0f); p[1].y = FLOAT2FIX( 1.0f); p[1].z = FLOAT2FIX(-1.0f);
  p[2].x = FLOAT2FIX( 1.0f); p[2].y = FLOAT2FIX(-1.0f); p[2].z = FLOAT2FIX(-1.0f);
  p[3].x = FLOAT2FIX( 1.0f); p[3].y = FLOAT2FIX( 1.0f); p[3].z = FLOAT2FIX(-1.0f);
  p[4].x = FLOAT2FIX(-1.0f); p[4].y = FLOAT2FIX(-1.0f); p[4].z = FLOAT2FIX( 1.0f);
  p[5].x = FLOAT2FIX(-1.0f); p[5].y = FLOAT2FIX( 1.0f); p[5].z = FLOAT2FIX( 1.0f);
  p[6].x = FLOAT2FIX( 1.0f); p[6].y = FLOAT2FIX(-1.0f); p[6].z = FLOAT2FIX( 1.0f);
  p[7].x = FLOAT2FIX( 1.0f); p[7].y = FLOAT2FIX( 1.0f); p[7].z = FLOAT2FIX( 1.0f);

  ll->type = LINE_LIST;
  ll->num_lines = 12;
  switch(ll->type) {
    case LINE_LIST:
      ll->lines = (u16 *)malloc(sizeof(u16) * ll->num_lines * 2);
      break;
    case LINE_STRIP:
      ll->lines = (u16 *)malloc(sizeof(u16) * ll->num_lines + 1);
      break;
    defaut:
      // printf("Invalid line type\n");
      ll->num_lines = 0;
      break;
  }

  // In this case we only care line list
  ll->lines[0] = 0; ll->lines[1] = 1;
  ll->lines[2] = 0; ll->lines[3] = 2;
  ll->lines[4] = 1; ll->lines[5] = 3;
  ll->lines[6] = 2; ll->lines[7] = 3;
  ll->lines[8] = 0; ll->lines[9] = 4;
  ll->lines[10] = 2; ll->lines[11] = 6;
  ll->lines[12] = 1; ll->lines[13] = 5;
  ll->lines[14] = 3; ll->lines[15] = 7;
  ll->lines[16] = 4; ll->lines[17] = 5;
  ll->lines[18] = 4; ll->lines[19] = 6;
  ll->lines[20] = 5; ll->lines[21] = 7;
  ll->lines[22] = 6; ll->lines[23] = 7;
}

void render(u16 *fb, mvx_vec3_t *p, line_list *ll, fix rx, fix ry, fix rz)
{
  int i;
  mvx_mat44_t m;

  mvx_mat44_identity(&m);
  mvx_mat44_scale(&m, FLOAT2FIX(20.0f), FLOAT2FIX(20.0f), FLOAT2FIX(20.0f));
  mvx_mat44_rot_x(&m, FIX2INT(rx));
  mvx_mat44_rot_y(&m, FIX2INT(ry));
  mvx_mat44_rot_z(&m, FIX2INT(rz));
  mvx_mat44_translate(&m, FLOAT2FIX(WIDTH/2.0f), FLOAT2FIX(HEIGHT/2.0f), FIXZERO);

  u16 clr = 0xffff;
  mvx_vec3_t dst[VERTEX_COUNT];
  for (i = 0; i < VERTEX_COUNT; i++) {
    mvx_vec3_transform(&dst[i], &p[i], &m);
    int x = FIX2INT(dst[i].x);
    int y = FIX2INT(dst[i].y);
    int irx = FIX2INT(rx);
    int iry = FIX2INT(ry);
    int irz = FIX2INT(rz);
  }

  for (i = 0; i < ll->num_lines; i++) {
    int x1 = FIX2INT(dst[ll->lines[i*2]].x);
    int y1 = FIX2INT(dst[ll->lines[i*2]].y);
    int x2 = FIX2INT(dst[ll->lines[i*2+1]].x);
    int y2 = FIX2INT(dst[ll->lines[i*2+1]].y);
    if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT &&
        x2 >= 0 && x2 < WIDTH && y2 >= 0 && y2 < HEIGHT)
      drawLineEFLA(x1, y1, x2, y2, clr);
      // drawLineEFLA(fb, x1, y1, x2, y2, clr);
    //else
      //printf("%d %d %d %d\n", x1, y1, x2, y2);
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


void CODE_IN_IWRAM DMAFastCopy(void *source, void *dest, u32_t count, u32_t mode)
{
	REG_DMA3SAD = (u32_t)source;
	REG_DMA3DAD = (u32_t)dest;
	REG_DMA3CNT = count | mode;
}

int main()
{
  bool useDmaToClear = true;
  vram = VRAM_ADRS;
  xorshiftSeed(0x12345678); // Seed the random number generator

  *(unsigned int*)0x04000000=0x0403; /* video mode 3 (0x03, 240x160 16bit), enable BG2(0x04) */
  ((unsigned short*)0x06000000)[120+80*240]=0x001f; /* Directly write to the framebuffer */
  ((unsigned short*)0x06000000)[136+80*240]=0x03e0;
  ((unsigned short*)0x06000000)[120+96*240]=0x7c00;

  buffer = (u16_t*)mallocAligned(WIDTH * HEIGHT * sizeof(u16_t), 4);
  clear_color = 0x0000;
  clearBuffer(clear_color);
	DMAFastCopy((void*)clear_color, buffer, WIDTH*HEIGHT, (DMA_SRC_FIXED | DMA_DST_INC | DMA_16 | DMA_ENABLE));

  mvx_vec3_t pos[8];
  line_list ll;
  create_cube(pos, &ll);

  fix rx = FLOAT2FIX(0.0f);
  fix ry = FLOAT2FIX(0.0f);
  fix rz = FLOAT2FIX(0.0f);
  fix dx = FLOAT2FIX(1.0f);
  fix dy = FLOAT2FIX(2.0f);
  fix dz = FLOAT2FIX(3.0f);


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
        DMAFastCopy((void*)clear_color, buffer, WIDTH*HEIGHT, (DMA_SRC_FIXED | DMA_DST_INC | DMA_16 | DMA_ENABLE));
      }
    }
#if 0
    u32_t x1 = xorshift32() % WIDTH;
    u32_t y1 = xorshift32() % HEIGHT;
    u32_t x2 = xorshift32() % WIDTH;
    u32_t y2 = xorshift32() % HEIGHT;
    u16_t color = xorshift32() & 0xffff;
    drawLineEFLA(x1, y1, x2, y2, color);
#else
    clearBuffer(clear_color);
    render(buffer, pos, &ll, rx, ry, rz);
    rx += dx;
    ry += dy;
    rz += dz;
#endif
 		waitForVBlank();
		DMAFastCopy(buffer, vram, WIDTH*HEIGHT, (DMA_IMMEDIATE | DMA_16 | DMA_ENABLE));
  }
}
