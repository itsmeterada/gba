/*
 *
 */

#include "vlt_fixed_math.h"

// static inline fix_t vlt_fix_from_int(int x) 
// { return x << FIXBITS; }

// static inline int vlt_fix_to_int(fix_t x) 
// { return x >> FIXBITS; }

// static inline fix_t vlt_fix_from_float(float x)
// { return (fix_t)(x * 65536.0f); }

// static inline float vlt_fix_to_float(fix_t x)
// { return (float)x / 65536.0f; }

// static inline fix_t vlt_fix_abs(fix_t x)
// { return (x < 0) ? -x : x; }

// static inline fix_t vlt_fix_min(fix_t x, fix_t y)
// { return (x < y) ? x : y; }

// static inline fix_t vlt_fix_max(fix_t x, fix_t y)
// { return (x > y) ? x : y; }

// static inline fix_t vlt_fix_floor(fix_t x)
// { return x & 0xffff0000; }

// static inline fix_t vlt_fix_ceil(fix_t x)
// { return (x & 0xffff0000) + (x & 0x0000ffff ? FIXONE : FIXZERO); }

// static inline fix_t vlt_fix_clamp(fix_t x, fix_t min, fix_t max)
// { return vlt_fix_min(vlt_fix_max(x, min), max); }


fix_t vlt_fix_add(fix_t a, fix_t b)
{ return a + b; }

fix_t vlt_fix_sub(fix_t a, fix_t b)
{ return a - b; }

fix_t vlt_fix_mul(fix_t a, fix_t b)
{
#define SIMPLE_FIXMUL
#ifdef SIMPLE_FIXMUL
  fix_t ah = (a >> FIXBITS);
  fix_t bh = (b >> FIXBITS);
  fix_t al = (a & FIXMASK);
  fix_t bl = (b & FIXMASK);
  return ((ah * bh) << FIXBITS) + ((ah * bl) + (bh * al) + ((al * bl) >> FIXBITS));
#else // SIMPLE_FIXMUL
	i32_t A = (a >> 16), C = (b >> 16);
	u32_t B = (a & 0xFFFF), D = (b & 0xFFFF);
	
	i32_t AC = A*C;
	i32_t AD_CB = A*D + C*B;
	u32_t BD = B*D;
	
	i32_t product_hi = AC + (AD_CB >> 16);
	
	// Handle carry from lower 32 bits to upper part of result.
	u32_t ad_cb_temp = AD_CB << 16;
	u32_t product_lo = BD + ad_cb_temp;
	if (product_lo < BD)
		product_hi++;
	
	// The upper 17 bits should all be the same (the sign).
	if (product_hi >> 31 != product_hi >> 15)
		return fix16_overflow;
	
	return (product_hi << 16) | (product_lo >> 16);
#endif // SIMPLE_FIXMUL
}

static u8_t clz(u32_t x)
{
  u8_t n = 0;
	if (x == 0) return 32;
	while (!(x & 0xf0000000)) { n += 4; x <<= 4; }
	while (!(x & 0x80000000)) { n += 1; x <<= 1; }
  return n;
}

fix_t vlt_fix_div(fix_t a, fix_t b)
{ 
	// This uses the basic binary restoring division algorithm.
	// It appears to be faster to do the whole division manually than
	// trying to compose a 64-bit divide out of 32-bit divisions on
	// platforms without hardware divide.
	
	if (b == 0)
		return FIXMIN;
	
  u32_t remainder = vlt_fix_abs(a);
  u32_t divider = vlt_fix_abs(b);

	u32_t quotient = 0;
	u32_t bit = 0x10000;
	
	/* The algorithm requires D >= R */
	while (divider < remainder)
	{
		divider <<= 1;
		bit <<= 1;
	}
	
	#ifndef FIXMATH_NO_OVERFLOW
	if (!bit)
		return FIXOVERFLOW;
	#endif
	
	if (divider & 0x80000000)
	{
		// Perform one step manually to avoid overflows later.
		// We know that divider's bottom bit is 0 here.
		if (remainder >= divider)
		{
				quotient |= bit;
				remainder -= divider;
		}
		divider >>= 1;
		bit >>= 1;
	}
	
	/* Main division loop */
	while (bit && remainder)
	{
		if (remainder >= divider)
		{
				quotient |= bit;
				remainder -= divider;
		}
		
		remainder <<= 1;
		bit >>= 1;
	}	 
			
	#ifndef FIXMATH_NO_ROUNDING
	if (remainder >= divider)
	{
		quotient++;
	}
	#endif
	
	fix_t result = quotient;
	
	/* Figure out the sign of result */
	if ((a ^ b) & 0x80000000)
	{
		#ifndef FIXMATH_NO_OVERFLOW
		if (result == FIXMIN)
				return FIXOVERFLOW;
		#endif
		
		result = -result;
	}
	
	return result;
}
