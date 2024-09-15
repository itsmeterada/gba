/*
 * GBA Graphics Example
 * by takehiko.terada@gmail.com
 * This code shows the beauty of GBA game console.
 * You can display graphics with this short code.
 * No libraries are used in this code.
 * Use this code with devkitARM.
 * To run the program, either write to a flashcart or use an emulator(mgba or etc).
 */


int main()
{
  *(unsigned int*)0x04000000=0x0403; /* video mode 3 (0x03, 240x160 16bit), enable BG2(0x04) */
  ((unsigned short*)0x06000000)[120+80*240]=0x001f; /* Directly write to the framebuffer */
  ((unsigned short*)0x06000000)[136+80*240]=0x03e0;
  ((unsigned short*)0x06000000)[120+96*240]=0x7c00;
  while(1);
}
