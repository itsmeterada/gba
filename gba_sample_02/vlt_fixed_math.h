/*
 *  vlt_fixed_math.h
 *  takehiko.terada@gmail.com
 *  Contains fixed point math functions.
 */
#ifndef VLT_FIXED_MATH_H
#define VLT_FIXED_MATH_H

#include "vlt_types.h"

//#define FIX_IS_FLOAT

#ifdef FIX_IS_FLOAT
#include <math.h>
typedef float           fix;
#define FIXZERO         0.0f
#define FIXONE          1.0f
#define FIXHALF         0.5f
#define TOFIX(x)        ((float)x)
#define FIX2INT(x)      ((int)x)
#define FIXMUL(a, b)    (a * b)
#define FIXDIV(a, b)    (a / b)   // Assuming both x and d has been converted using TOFIX(x)
#define FIXDIV2(x)      (x * 0.5f)   // Assuming x has been converted using TOFIX(x)
#define FIXSIN(x)       (sinf(x))
#define FIXCOS(x)       (cosf(x))
#else // FIX_IS_FLOAT
#define FIXZERO         0x00000000
#define FIXONE          0x00010000
#define FIXHALF         0x00008000
#define FIXBITS         16
#define FIXROUNDBIT     (1 << (FIXBITS - 1))
#define FIXMAX          0x7fffffff // 2,147,48,3647 
#define FIXMIN          0x80000000 // -2,147,48,3648
#define FIXMASK         0x0000ffff
#define FIXOVERFLOW     0x80000000
#define INTPART(x)      (x & 0xffff0000)
#define DECIMALPART(x)  (x & FIXMASK)
#define TOFIX(x)        (x<<FIXBITS)
#define FIX2INT(x)      (x>>FIXBITS)
#define FLOAT2FIX(x)    ((fix_t)(x * 65536))
#define FIX2FLOAT(x)    (x / 65536.f)
//#define FIXMUL(a, b)    (a * b)
//#define FIXMULFIX(a, b) (a * FIX2INT(b))
// #define FIXDIV(a, b)    (TOFIX(a / b)) // Assuming both x and d has been converted using TOFIX(x)
// #define FIXDIV(a, b)    (TOFIX(((a>>1) / (b>>1))<<1)) // Assuming both x and d has been converted using TOFIX(x)
// #define FIXDIV2(x)      (x >> 1)  // Assuming x has been converted using TOFIX(x)
// #define FIXSIN(x)       FINXSINTABLE(FIX2INT(x))
// #define FIXCOS(x)       FINXCOSTABLE(FIX2INT(x))
#define SINTABLESIZE    256
#define MY_PI           3.1415926535897932384626433832795f
#endif // FIX_IS_FLOAT


static inline fix_t vlt_fix_from_int(int x) 
  { return x << FIXBITS; }
static inline int vlt_fix_to_int(fix_t x) 
  { return x >> FIXBITS; }
static inline fix_t vlt_fix_from_float(float x)
  { return (fix_t)(x * 65536.0f); }
static inline float vlt_fix_to_float(fix_t x)
  { return (float)x / 65536.0f; }
static inline fix_t vlt_fix_abs(fix_t x)
  { return (x < 0) ? -x : x; }
static inline fix_t vlt_fix_min(fix_t x, fix_t y)
  { return (x < y) ? x : y; }
static inline fix_t vlt_fix_max(fix_t x, fix_t y)
  { return (x > y) ? x : y; }
static inline fix_t vlt_fix_floor(fix_t x)
  { return x & 0xffff0000; }
static inline fix_t vlt_fix_ceil(fix_t x)
  { return (x & 0xffff0000) + (x & 0x0000ffff ? FIXONE : FIXZERO); }
static inline fix_t vlt_fix_clamp(fix_t x, fix_t min, fix_t max)
  { return vlt_fix_min(vlt_fix_max(x, min), max); }

#define FIX16(x) ((fix_t)(((x) >= 0) ? ((x) * 65536.0 + 0.5) : ((x) * 65536.0 - 0.5)))

fix_t vlt_fix_add(fix_t a, fix_t b);
fix_t vlt_fix_sub(fix_t a, fix_t b);
fix_t vlt_fix_mul(fix_t a, fix_t b);
fix_t vlt_fix_div(fix_t a, fix_t b);
fix_t vlt_fix_round(fix_t x);

fix_t vlt_fix_sin(fix_t angle);
fix_t vlt_fix_cos(fix_t angle);
fix_t vlt_fix_tan(fix_t angle);
fix_t vlt_fix_asin(fix_t angle);
fix_t vlt_fix_acos(fix_t angle);
fix_t vlt_fix_atan(fix_t angle);
fix_t vlt_fix_atan2(fix_t angle);
fix_t vtx_deg_to_rad(fix_t degree);
fix_t vtx_rag_to_deg(fix_t radian);

fix_t vlt_fix_sqrt(fix_t x);
fix_t vlt_fix_square(fix_t x);
fix_t vlt_fix_exp(fix_t x);
fix_t vlt_fix_log(fix_t x);
fix_t vlt_fix_pow(fix_t x, fix_t y);
fix_t vlt_fix_log10(fix_t x);
fix_t vlt_fix_log2(fix_t x);
fix_t vlt_fix_lerp(fix_t a, fix_t b, fix_t t);




#endif // VLT_FIXED_MATH_H