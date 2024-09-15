/*
 * GBA self-contained program
 * by takehiko.terada@gmail.com
 * 
 * This program is a self-contained program for GBA.
 * It is written in C language and compiled with GCC.
 * It does not require any external data.
 * It try to be as independent as possible by using it's own crt0.s and .ld linker script.
 */


int main()
{
  *(unsigned int*)0x04000000=0x0403; /* video mode 3 (0x03, 240x160 16bit), enable BG2(0x04) */
  ((unsigned short*)0x06000000)[120+80*240]=0x001f; /* Directly write to the framebuffer */
  ((unsigned short*)0x06000000)[136+80*240]=0x03e0;
  ((unsigned short*)0x06000000)[120+96*240]=0x7c00;
  while(1);
}
