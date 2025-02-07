/*
 *  MicroVex - Minumum 3D Engine for Low End Edge Device
 *  Takehiko TERADA (takehiko.terada@gmail.com)
 */

#ifndef __MICROVEX_H__
#define __MICROVEX_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DEBUG

/* Define types here */
typedef signed char     i8;
typedef unsigned char   u8;
typedef signed short    i16;
typedef unsigned short  u16;
typedef signed int      i32;
typedef unsigned int    u32;


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
typedef int             fix;
#define FIXZERO         0x00000000
#define FIXONE          0x00010000
#define FIXHALF         0x00008000
#define FIXBITS         16
#define FIXROUNDBIT     (1 << (FIXBITS - 1))
#define FIXMASK         0x0000ffff
#define INTPART(x)      (x & 0xffff0000)
#define DECIMALPART(x)  (x & FIXMASK)
#define TOFIX(x)        (x<<FIXBITS)
#define FIX2INT(x)      (x>>FIXBITS)
#define FLOAT2FIX(x)    ((fix)(x * 65536))
#define FIX2FLOAT(x)    (x / 65536.f)
#define FIXDIV(a, b)    (TOFIX(a / b)) // Assuming both x and d has been converted using TOFIX(x)
#define FIXDIV2(x)      (x >> 1)  // Assuming x has been converted using TOFIX(x)
#define FIXSIN(x)       FINXSINTABLE(FIX2INT(x))
#define FIXCOS(x)       FINXCOSTABLE(FIX2INT(x))
#define SINTABLESIZE    256
#define MY_PI           3.1415926535897932384626433832795f
#endif // FIX_IS_FLOAT

#include "sinTable.h"

/* Structs */
typedef struct {
  fix x, y;
} mvx_vec2_t;

typedef struct {
  fix x, y, z;
} mvx_vec3_t;

typedef struct {
  fix v[4];
} mvx_vec4_t;

typedef struct {
  fix m[4][4];
} mvx_mat44_t;

typedef struct {
  i32 num_vtx;
  mvx_vec3_t *vtx_list;
  mvx_vec3_t *nrm_list;
  mvx_vec2_t *uv_list;
  mvx_vec2_t *proj_list;
} mvx_vtx_t;

typedef struct {
  i32 t[3];
} mvx_tri_t;

typedef struct {
  i32 num_tri;
  mvx_vtx_t  *vtx;
  mvx_tri_t  *tri;
} mvx_mesh_t;

typedef struct {
  mvx_vec3_t   *pos;
  mvx_vec4_t   *q;   // quaternion
  bool  flip;
  fix   *m;   // camera matrix
  void (*update)();
  void (*track)();
  void (*project)();
} mvx_camera_t;


/* Global variables */
u32 __num = 0;

/* Function prototypes */

fix FIXMUL(fix a, fix b)
{
  fix ah = (a >> FIXBITS);
  fix bh = (b >> FIXBITS);
  fix al = (a & FIXMASK);
  fix bl = (b & FIXMASK);
  return ((ah * bh) << FIXBITS) + ((ah * bl) + (bh * al) + ((al * bl) >> FIXBITS));
}

fix FIXMUL2(fix a, fix b)
{
  return ((a * b + FIXROUNDBIT) >> FIXBITS);
}

void __xor_shift_seed(u32 seed)
{
	__num = seed;
}

u32 __xorshift32(void)
{
  __num = __num ^ (__num << 13);
  __num = __num ^ (__num >> 17);
  __num = __num ^ (__num << 15);
  return __num;	
}

fix __ceil(fix x)
{
	if (x & 0x0000ffff)
		return (x + 0x00010000) & 0xffff0000;
	else
		return x & 0xffff0000;
}

fix __flr(fix x)
{
	return (x & 0xFFFF0000);
}

fix __rnd(fix x)
{
	static bool init = false;
	if (!init) {
		init = true;
		__xor_shift_seed(524287);
	}

	u32 v = __xorshift32() & 0xffff;
	fix tmpv = FIXMUL(v, x);

	return tmpv;
}

fix mvx_sin(fix angle)
{
#if 0
  return FLOAT2FIX(sin(angle * MY_PI / 180.0f));
#else
  if (angle < 0)
    angle = -angle;
  while(angle >= SINTABLESIZE) {
    angle -= SINTABLESIZE;
  }
  return sinTable[angle];
#endif
}

fix mvx_cos(fix angle)
{
#if 0
  return FLOAT2FIX(cosf(angle * (MY_PI / 180.0f)));
#else
  return mvx_sin(angle + (SINTABLESIZE / 4));
#endif
}

void mvx_mat44_identity(mvx_mat44_t *m)
{
  m->m[0][0] = FIXONE;  m->m[0][1] = FIXZERO; m->m[0][2] = FIXZERO; m->m[0][3] = FIXZERO;
  m->m[1][0] = FIXZERO; m->m[1][1] = FIXONE;  m->m[1][2] = FIXZERO; m->m[1][3] = FIXZERO;
  m->m[2][0] = FIXZERO; m->m[2][1] = FIXZERO; m->m[2][2] = FIXONE;  m->m[2][3] = FIXZERO;
  m->m[3][0] = FIXZERO; m->m[3][1] = FIXZERO; m->m[3][2] = FIXZERO; m->m[3][3] = FIXONE;
}

void mvx_mat44_mul(mvx_mat44_t *dst, mvx_mat44_t *src)
{
  fix m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;
  m00 = FIXMUL(dst->m[0][0], src->m[0][0]) + FIXMUL(dst->m[0][1], src->m[1][0]) + FIXMUL(dst->m[0][2], src->m[2][0]) + FIXMUL(dst->m[0][3], src->m[3][0]);
  m01 = FIXMUL(dst->m[0][0], src->m[0][1]) + FIXMUL(dst->m[0][1], src->m[1][1]) + FIXMUL(dst->m[0][2], src->m[2][1]) + FIXMUL(dst->m[0][3], src->m[3][1]);
  m02 = FIXMUL(dst->m[0][0], src->m[0][2]) + FIXMUL(dst->m[0][1], src->m[1][2]) + FIXMUL(dst->m[0][2], src->m[2][2]) + FIXMUL(dst->m[0][3], src->m[3][2]);
  m03 = FIXMUL(dst->m[0][0], src->m[0][3]) + FIXMUL(dst->m[0][1], src->m[1][3]) + FIXMUL(dst->m[0][2], src->m[2][3]) + FIXMUL(dst->m[0][3], src->m[3][3]);

  m10 = FIXMUL(dst->m[1][0], src->m[0][0]) + FIXMUL(dst->m[1][1], src->m[1][0]) + FIXMUL(dst->m[1][2], src->m[2][0]) + FIXMUL(dst->m[1][3], src->m[3][0]);
  m11 = FIXMUL(dst->m[1][0], src->m[0][1]) + FIXMUL(dst->m[1][1], src->m[1][1]) + FIXMUL(dst->m[1][2], src->m[2][1]) + FIXMUL(dst->m[1][3], src->m[3][1]);
  m12 = FIXMUL(dst->m[1][0], src->m[0][2]) + FIXMUL(dst->m[1][1], src->m[1][2]) + FIXMUL(dst->m[1][2], src->m[2][2]) + FIXMUL(dst->m[1][3], src->m[3][2]);
  m13 = FIXMUL(dst->m[1][0], src->m[0][3]) + FIXMUL(dst->m[1][1], src->m[1][3]) + FIXMUL(dst->m[1][2], src->m[2][3]) + FIXMUL(dst->m[1][3], src->m[3][3]);

  m20 = FIXMUL(dst->m[2][0], src->m[0][0]) + FIXMUL(dst->m[2][1], src->m[1][0]) + FIXMUL(dst->m[2][2], src->m[2][0]) + FIXMUL(dst->m[2][3], src->m[3][0]);
  m21 = FIXMUL(dst->m[2][0], src->m[0][1]) + FIXMUL(dst->m[2][1], src->m[1][1]) + FIXMUL(dst->m[2][2], src->m[2][1]) + FIXMUL(dst->m[2][3], src->m[3][1]);
  m22 = FIXMUL(dst->m[2][0], src->m[0][2]) + FIXMUL(dst->m[2][1], src->m[1][2]) + FIXMUL(dst->m[2][2], src->m[2][2]) + FIXMUL(dst->m[2][3], src->m[3][2]);
  m23 = FIXMUL(dst->m[2][0], src->m[0][3]) + FIXMUL(dst->m[2][1], src->m[1][3]) + FIXMUL(dst->m[2][2], src->m[2][3]) + FIXMUL(dst->m[2][3], src->m[3][3]);

  m30 = FIXMUL(dst->m[3][0], src->m[0][0]) + FIXMUL(dst->m[3][1], src->m[1][0]) + FIXMUL(dst->m[3][2], src->m[2][0]) + FIXMUL(dst->m[3][3], src->m[3][0]);
  m31 = FIXMUL(dst->m[3][0], src->m[0][1]) + FIXMUL(dst->m[3][1], src->m[1][1]) + FIXMUL(dst->m[3][2], src->m[2][1]) + FIXMUL(dst->m[3][3], src->m[3][1]);
  m32 = FIXMUL(dst->m[3][0], src->m[0][2]) + FIXMUL(dst->m[3][1], src->m[1][2]) + FIXMUL(dst->m[3][2], src->m[2][2]) + FIXMUL(dst->m[3][3], src->m[3][2]);
  m33 = FIXMUL(dst->m[3][0], src->m[0][3]) + FIXMUL(dst->m[3][1], src->m[1][3]) + FIXMUL(dst->m[3][2], src->m[2][3]) + FIXMUL(dst->m[3][3], src->m[3][3]);

  dst->m[0][0] = m00; dst->m[0][1] = m01; dst->m[0][2] = m02; dst->m[0][3] = m03;
  dst->m[1][0] = m10; dst->m[1][1] = m11; dst->m[1][2] = m12; dst->m[1][3] = m13;
  dst->m[2][0] = m20; dst->m[2][1] = m21; dst->m[2][2] = m22; dst->m[2][3] = m23;
  dst->m[3][0] = m30; dst->m[3][1] = m31; dst->m[3][2] = m32; dst->m[3][3] = m33;
}

void mvx_mat44_mul2src(mvx_mat44_t *dst, mvx_mat44_t *src1, mvx_mat44_t *src2)
{
  int i, j;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      dst->m[i][j] = FIXMUL(src1->m[0][j], src2->m[i][0]) +
                     FIXMUL(src1->m[1][j], src2->m[i][1]) +
                     FIXMUL(src1->m[2][j], src2->m[i][2]) +
                     FIXMUL(src1->m[3][j], src2->m[i][3]);
    }
  }
}

void mvx_mat44_rot_x(mvx_mat44_t *mat, int angle)
{
  fix s = mvx_sin(angle);
  fix c = mvx_cos(angle);
  mvx_mat44_t m;
  m.m[0][0] = FIXONE;  m.m[0][1] = FIXZERO; m.m[0][2] = FIXZERO; m.m[0][3] = FIXZERO;
  m.m[1][0] = FIXZERO; m.m[1][1] = c;       m.m[1][2] = s;       m.m[1][3] = FIXZERO;
  m.m[2][0] = FIXZERO; m.m[2][1] = -s;      m.m[2][2] = c;       m.m[2][3] = FIXZERO;
  m.m[3][0] = FIXZERO; m.m[3][1] = FIXZERO; m.m[3][2] = FIXZERO; m.m[3][3] = FIXONE;
  mvx_mat44_mul(mat, &m);
}

void mvx_mat44_rot_y(mvx_mat44_t *mat, int angle)
{
  fix s = mvx_sin(angle);
  fix c = mvx_cos(angle);
  mvx_mat44_t m;
  m.m[0][0] = c;       m.m[0][1] = FIXZERO; m.m[0][2] = -s;      m.m[0][3] = FIXZERO;
  m.m[1][0] = FIXZERO; m.m[1][1] = FIXONE;  m.m[1][2] = FIXZERO; m.m[1][3] = FIXZERO;
  m.m[2][0] = s;       m.m[2][1] = FIXZERO; m.m[2][2] = c;       m.m[2][3] = FIXZERO;
  m.m[3][0] = FIXZERO; m.m[3][1] = FIXZERO; m.m[3][2] = FIXZERO; m.m[3][3] = FIXONE;
  mvx_mat44_mul(mat, &m);
}

void mvx_mat44_rot_z(mvx_mat44_t *mat, int angle)
{
  fix s = mvx_sin(angle);
  fix c = mvx_cos(angle);
  mvx_mat44_t m;
  m.m[0][0] = c;       m.m[0][1] = s;       m.m[0][2] = FIXZERO; m.m[0][3] = FIXZERO;
  m.m[1][0] = -s;      m.m[1][1] = c;       m.m[1][2] = FIXZERO; m.m[1][3] = FIXZERO;
  m.m[2][0] = FIXZERO; m.m[2][1] = FIXZERO; m.m[2][2] = FIXONE;  m.m[2][3] = FIXZERO;
  m.m[3][0] = FIXZERO; m.m[3][1] = FIXZERO; m.m[3][2] = FIXZERO; m.m[3][3] = FIXONE;
  mvx_mat44_mul(mat, &m);
}

void mvx_mat44_translate(mvx_mat44_t *mat, fix x, fix y, fix z)
{
  mvx_mat44_t m;
  m.m[0][0] = FIXONE;  m.m[0][1] = FIXZERO; m.m[0][2] = FIXZERO; m.m[0][3] = FIXZERO;
  m.m[1][0] = FIXZERO; m.m[1][1] = FIXONE;  m.m[1][2] = FIXZERO; m.m[1][3] = FIXZERO;
  m.m[2][0] = FIXZERO; m.m[2][1] = FIXZERO; m.m[2][2] = FIXONE;  m.m[2][3] = FIXZERO;
  m.m[3][0] = x;       m.m[3][1] = y;       m.m[3][2] = z;       m.m[3][3] = FIXONE;
  mvx_mat44_mul(mat, &m);
}

void mvx_mat44_scale(mvx_mat44_t *mat, fix sx, fix sy, fix sz)
{
  mvx_mat44_t m;
  m.m[0][0] = sx;      m.m[0][1] = FIXZERO; m.m[0][2] = FIXZERO; m.m[0][3] = FIXZERO;
  m.m[1][0] = FIXZERO; m.m[1][1] = sy;      m.m[1][2] = FIXZERO; m.m[1][3] = FIXZERO;
  m.m[2][0] = FIXZERO; m.m[2][1] = FIXZERO; m.m[2][2] = sz;      m.m[2][3] = FIXZERO;
  m.m[3][0] = FIXZERO; m.m[3][1] = FIXZERO; m.m[3][2] = FIXZERO; m.m[3][3] = FIXONE;
  mvx_mat44_mul(mat, &m);
}

void mvx_vec3_transform(mvx_vec3_t *dst, const mvx_vec3_t *src, const mvx_mat44_t *mat)
{
  dst->x = FIXMUL(src->x, mat->m[0][0]) + FIXMUL(src->y, mat->m[1][0]) + FIXMUL(src->z, mat->m[2][0]) + mat->m[3][0];
  dst->y = FIXMUL(src->x, mat->m[0][1]) + FIXMUL(src->y, mat->m[1][1]) + FIXMUL(src->z, mat->m[2][1]) + mat->m[3][1];
  dst->z = FIXMUL(src->x, mat->m[0][2]) + FIXMUL(src->y, mat->m[1][2]) + FIXMUL(src->z, mat->m[2][2]) + mat->m[3][2];
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* __MICROVEX_H__ */
