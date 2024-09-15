/*
 *  vlt_gba.h
 *  takehiko.terada@gmail.com
 *  
 *  GBA related definitions
 */
#ifndef VLT_GBA_H
#define VLT_GBA_H

/*
// 0x00000000 - 0x00003FFF – 16KB System ROM（Executable but not readable）
// 0x02000000 - 0x02030000 – 256 KB EWRAM (External Work RAM)
// 0x03000000 - 0x03007FFF – 32 KB IWRAM（Internal Work RAM）
// 0x04000000 - 0x040003FF – I/O registers
// 0x05000000 - 0x050003FF – 1KB Color Palette RAM
// 0x06000000 - 0x06017FFF – 96KB VRAM （Video RAM）
// 0x07000000 - 0x070003FF – 1KB OAM RAM （Object Attribute Memory）
// 0x08000000 - 0x???????? – Game Pak ROM (0 to 32MB)
// 0x0E000000 - 0x???????? – Game Pak RAM
*/


/*
// Data types
*/
typedef unsigned char           u8;     /**< Unsigned 8 bit value       */
typedef unsigned short int      u16;    /**< Unsigned 16 bit value      */
typedef unsigned int            u32;    /**< Unsigned 32 bit value      */
typedef signed char             i8;     /**< Signed 8 bit value */
typedef signed short int        i16;    /**< Signed 16 bit value        */
typedef signed int              i32;    /**< Signed 32 bit value        */
typedef volatile u8             vu8;    /**< volatile Unsigned 8 bit value      */
typedef volatile u16            vu16;   /**< volatile Unigned 16 bit value      */
typedef volatile u32            vu32;   /**< volatile Unsigned 32 bit value     */
typedef volatile i8             vi8;    /**< volatile Signed 8 bit value        */
typedef volatile i16            vi16;   /**< volatile Signed 8 bit value        */
typedef volatile i32            vi32;   /**< volatile Signed 8 bit value        */
#ifndef __cplusplus
/*! C++ compatible bool for C */
typedef enum { false, true } bool;
#endif

#define ALIGN(p)	__attribute__((aligned(p)))

#define	REG_BASE				0x04000000

#define REG_DISPCNT		(vu16 *)REG_BASE
#define REG_VCOUNT		(vu16 *)0x04000006
#define REG_DISPSTAT	(vu16 *)0x04000004

vu16 *scanlineCounter = REG_VCOUNT;

//#define MODE5
#ifdef MODE5
#define DISP_MODE5	0x05
#define WIDTH 160
#define HEIGHT 128
#else // MODE3
#define DISP_MODE3	0x03
#define WIDTH 240
#define HEIGHT 160
#endif // MODE5


// Background rotation / scale
#define REG_BG2PA		(vu16 *)0x04000020
#define REG_BG2PB		(vu16 *)0x04000022
#define REG_BG2PC		(vu16 *)0x04000024
#define REG_BG2PD		(vu16 *)0x04000026
#define REG_BG2X		(vu32 *)0x04000028
#define REG_BG2Y		(vu32 *)0x0400002C

#define REG_BG2CNT		(vu16 *)0x0400000C

// Timer data addresses
#define REG_TM0D				(vu16 *)0x04000100
#define REG_TM1D				(vu16 *)0x04000104
#define REG_TM2D				(vu16 *)0x04000108
#define REG_TM3D				(vu16 *)0x0400010C
// Timer status addresses
#define REG_TM0CNT				(vu16 *)0x04000102
#define REG_TM1CNT				(vu16 *)0x04000106
#define REG_TM2CNT				(vu16 *)0x0400010A
#define REG_TM3CNT				(vu16 *)0x0400010E
// Timer constants
#define TIMER_FREQUENCY_SYSTEM	0x00
#define TIMER_FREQUENCY_64		0x01
#define TIMER_FREQUENCY_256		0x02
#define TIMER_FREQUENCY_1024	0x03
#define TIMER_OVERFLOW			0x04
#define TIMER_IRQ_ENABLE		0x40
#define TIMER_ENABLE			0x80


// DMA
#define REG_DMA0SAD	*(vu32*)(REG_BASE + 0x0b0)
#define REG_DMA0DAD	*(vu32*)(REG_BASE + 0x0b4)
#define REG_DMA0CNT	*(vu32*)(REG_BASE + 0x0b8)
//--
#define REG_DMA1SAD	*(vu32*)(REG_BASE + 0x0bc)
#define REG_DMA1DAD	*(vu32*)(REG_BASE + 0x0c0)
#define REG_DMA1CNT	*(vu32*)(REG_BASE + 0x0c4)
//--
#define REG_DMA2SAD	*(vu32*)(REG_BASE + 0x0c8)
#define REG_DMA2DAD	*(vu32*)(REG_BASE + 0x0cc)
#define REG_DMA2CNT	*(vu32*)(REG_BASE + 0x0d0)
//--
#define REG_DMA3SAD	*(vu32*)(REG_BASE + 0x0d4)
#define REG_DMA3DAD	*(vu32*)(REG_BASE + 0x0d8)
#define REG_DMA3CNT	*(vu32*)(REG_BASE + 0x0dc)
//--
#define DMA_DST_INC		(0<<21)
#define DMA_DST_DEC		(1<<21)
#define DMA_DST_FIXED	(2<<21) // 固定
#define DMA_DST_RELOAD	(3<<21)

#define DMA_SRC_INC		(0<<23)
#define DMA_SRC_DEC		(1<<23)
#define DMA_SRC_FIXED	(2<<23) // 固定

#define DMA_REPEAT		(1<<25)
#define DMA_IRQ			(1<<30)
#define DMA_ENABLE		(1<<31)
#define DMA_IMMEDIATE	(0<<28)
#define DMA_VBLANK		(1<<28)
#define DMA_HBLANK		(2<<28)
#define DMA_SPECIAL		(3<<28)
#define DMA_16			0x00000000
#define DMA_32			0x04000000

#ifdef DEBUG
#define CODE_IN_IWRAM
#else // DEBUG
#define CODE_IN_IWRAM __attribute__ ((section (".iwram"), long_call))
#endif // DEBUG

// VRAM address
vu16 *vram;
#define VRAM_ADRS		(vu16 *)0x06000000
#define RGB(r,g,b) ((((b)>>3)<<10)+(((g)>>3)<<5)+((r)>>3))
//u16 surface[WIDTH * HEIGHT] ALIGN(4);
u16 clear_color ALIGN(4);
//u8 *buffer;
u16 *buffer;

#define drawPixel(x, y, c)	(buffer[y*WIDTH+x] = c)
#define SETPIXEL(x, y, c)	(buffer[y*WIDTH+x] = c)
void drawLineEFLA(i32 x1, i32 y1, i32 x2, i32 y2, u16 color);

// Controller buttons
volatile u32 *BUTTONS = (volatile u32*)0x04000130;
#define BUTTON_A		1
#define BUTTON_B		2
#define BUTTON_SELECT	4
#define BUTTON_START	8
#define BUTTON_RIGHT	16
#define BUTTON_LEFT		32
#define BUTTON_UP		64
#define BUTTON_DOWN		128
#define BUTTON_R		256
#define BUTTON_L		512

#endif // VLT_GBA_H