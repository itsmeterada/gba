/*
 *  vlt.h - A small game engine library
 *  takehiko.terada@gmail.com
 * 
 *  Copyright (c) 2022, Takehiko Terada
 *  All rights reserved.
 */

#ifndef __VLT_H__
#define __VLT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#define DBG(fmt,...) printf(fmt, __VA_ARGS__)
#else
#define DBG(...)
#endif // DEBUG

#include "vlt_types.h"
#include "sin_table.h"
#include "vlt_fixed_math.h"

/* Global variables */
u32_t vlt_num = 0;

#define vlt_fix_mul(a, b)    vlt_fix_mul(a, b)

/* Function prototypes */


void vlt_xor_shift_seed(u32_t seed)
{
	vlt_num = seed;
}

u32_t vlt_xorshift32(void)
{
  vlt_num = vlt_num ^ (vlt_num << 13);
  vlt_num = vlt_num ^ (vlt_num >> 17);
  vlt_num = vlt_num ^ (vlt_num << 15);
  return vlt_num;	
}

fix_t vlt_rnd(fix_t x)
{
	static bool init = false;
	if (!init) {
		init = true;
		// __xor_shift_seed(time(NULL));
		vlt_xor_shift_seed(524287); // 127, 8191, 131071, 524287, 6700417, 2147483647
	}

	u32_t v = vlt_xorshift32() & 0xffff;
	fix_t tmpv = vlt_fix_mul(v, x);

	return tmpv;
}

fix_t vlt_sin(fix_t angle)
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

fix_t vlt_cos(fix_t angle)
{
#if 0
  return FLOAT2FIX(cosf(angle * (MY_PI / 180.0f)));
#else
  return vlt_sin(angle + (SINTABLESIZE / 4));
#endif
}

void vlt_mat44_identity(vlt_mat44_t *m)
{
  m->m[0][0] = FIXONE;  m->m[0][1] = FIXZERO; m->m[0][2] = FIXZERO; m->m[0][3] = FIXZERO;
  m->m[1][0] = FIXZERO; m->m[1][1] = FIXONE;  m->m[1][2] = FIXZERO; m->m[1][3] = FIXZERO;
  m->m[2][0] = FIXZERO; m->m[2][1] = FIXZERO; m->m[2][2] = FIXONE;  m->m[2][3] = FIXZERO;
  m->m[3][0] = FIXZERO; m->m[3][1] = FIXZERO; m->m[3][2] = FIXZERO; m->m[3][3] = FIXONE;
}

void vlt_mat44_mul(vlt_mat44_t *dst, vlt_mat44_t *src)
{
  fix_t m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;
  m00 = vlt_fix_mul(dst->m[0][0], src->m[0][0]) + vlt_fix_mul(dst->m[0][1], src->m[1][0]) + vlt_fix_mul(dst->m[0][2], src->m[2][0]) + vlt_fix_mul(dst->m[0][3], src->m[3][0]);
  m01 = vlt_fix_mul(dst->m[0][0], src->m[0][1]) + vlt_fix_mul(dst->m[0][1], src->m[1][1]) + vlt_fix_mul(dst->m[0][2], src->m[2][1]) + vlt_fix_mul(dst->m[0][3], src->m[3][1]);
  m02 = vlt_fix_mul(dst->m[0][0], src->m[0][2]) + vlt_fix_mul(dst->m[0][1], src->m[1][2]) + vlt_fix_mul(dst->m[0][2], src->m[2][2]) + vlt_fix_mul(dst->m[0][3], src->m[3][2]);
  m03 = vlt_fix_mul(dst->m[0][0], src->m[0][3]) + vlt_fix_mul(dst->m[0][1], src->m[1][3]) + vlt_fix_mul(dst->m[0][2], src->m[2][3]) + vlt_fix_mul(dst->m[0][3], src->m[3][3]);

  m10 = vlt_fix_mul(dst->m[1][0], src->m[0][0]) + vlt_fix_mul(dst->m[1][1], src->m[1][0]) + vlt_fix_mul(dst->m[1][2], src->m[2][0]) + vlt_fix_mul(dst->m[1][3], src->m[3][0]);
  m11 = vlt_fix_mul(dst->m[1][0], src->m[0][1]) + vlt_fix_mul(dst->m[1][1], src->m[1][1]) + vlt_fix_mul(dst->m[1][2], src->m[2][1]) + vlt_fix_mul(dst->m[1][3], src->m[3][1]);
  m12 = vlt_fix_mul(dst->m[1][0], src->m[0][2]) + vlt_fix_mul(dst->m[1][1], src->m[1][2]) + vlt_fix_mul(dst->m[1][2], src->m[2][2]) + vlt_fix_mul(dst->m[1][3], src->m[3][2]);
  m13 = vlt_fix_mul(dst->m[1][0], src->m[0][3]) + vlt_fix_mul(dst->m[1][1], src->m[1][3]) + vlt_fix_mul(dst->m[1][2], src->m[2][3]) + vlt_fix_mul(dst->m[1][3], src->m[3][3]);

  m20 = vlt_fix_mul(dst->m[2][0], src->m[0][0]) + vlt_fix_mul(dst->m[2][1], src->m[1][0]) + vlt_fix_mul(dst->m[2][2], src->m[2][0]) + vlt_fix_mul(dst->m[2][3], src->m[3][0]);
  m21 = vlt_fix_mul(dst->m[2][0], src->m[0][1]) + vlt_fix_mul(dst->m[2][1], src->m[1][1]) + vlt_fix_mul(dst->m[2][2], src->m[2][1]) + vlt_fix_mul(dst->m[2][3], src->m[3][1]);
  m22 = vlt_fix_mul(dst->m[2][0], src->m[0][2]) + vlt_fix_mul(dst->m[2][1], src->m[1][2]) + vlt_fix_mul(dst->m[2][2], src->m[2][2]) + vlt_fix_mul(dst->m[2][3], src->m[3][2]);
  m23 = vlt_fix_mul(dst->m[2][0], src->m[0][3]) + vlt_fix_mul(dst->m[2][1], src->m[1][3]) + vlt_fix_mul(dst->m[2][2], src->m[2][3]) + vlt_fix_mul(dst->m[2][3], src->m[3][3]);

  m30 = vlt_fix_mul(dst->m[3][0], src->m[0][0]) + vlt_fix_mul(dst->m[3][1], src->m[1][0]) + vlt_fix_mul(dst->m[3][2], src->m[2][0]) + vlt_fix_mul(dst->m[3][3], src->m[3][0]);
  m31 = vlt_fix_mul(dst->m[3][0], src->m[0][1]) + vlt_fix_mul(dst->m[3][1], src->m[1][1]) + vlt_fix_mul(dst->m[3][2], src->m[2][1]) + vlt_fix_mul(dst->m[3][3], src->m[3][1]);
  m32 = vlt_fix_mul(dst->m[3][0], src->m[0][2]) + vlt_fix_mul(dst->m[3][1], src->m[1][2]) + vlt_fix_mul(dst->m[3][2], src->m[2][2]) + vlt_fix_mul(dst->m[3][3], src->m[3][2]);
  m33 = vlt_fix_mul(dst->m[3][0], src->m[0][3]) + vlt_fix_mul(dst->m[3][1], src->m[1][3]) + vlt_fix_mul(dst->m[3][2], src->m[2][3]) + vlt_fix_mul(dst->m[3][3], src->m[3][3]);

  dst->m[0][0] = m00;
  dst->m[0][1] = m01;
  dst->m[0][2] = m02;
  dst->m[0][3] = m03;

  dst->m[1][0] = m10;
  dst->m[1][1] = m11;
  dst->m[1][2] = m12;
  dst->m[1][3] = m13;

  dst->m[2][0] = m20;
  dst->m[2][1] = m21;
  dst->m[2][2] = m22;
  dst->m[2][3] = m23;

  dst->m[3][0] = m30;
  dst->m[3][1] = m31;
  dst->m[3][2] = m32;
  dst->m[3][3] = m33;
}

void vlt_mat44_mul2src(vlt_mat44_t *dst, vlt_mat44_t *src1, vlt_mat44_t *src2)
{
  int i, j;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      dst->m[i][j] = vlt_fix_mul(src1->m[0][j], src2->m[i][0]) +
                     vlt_fix_mul(src1->m[1][j], src2->m[i][1]) +
                     vlt_fix_mul(src1->m[2][j], src2->m[i][2]) +
                     vlt_fix_mul(src1->m[3][j], src2->m[i][3]);
    }
  }
}

void vlt_mat44_rot_x(vlt_mat44_t *mat, int angle)
{
  fix_t s = vlt_sin(angle);
  fix_t c = vlt_cos(angle);
  vlt_mat44_t m;
  m.m[0][0] = FIXONE;  m.m[0][1] = FIXZERO; m.m[0][2] = FIXZERO; m.m[0][3] = FIXZERO;
  m.m[1][0] = FIXZERO; m.m[1][1] = c;       m.m[1][2] = s;       m.m[1][3] = FIXZERO;
  m.m[2][0] = FIXZERO; m.m[2][1] = -s;      m.m[2][2] = c;       m.m[2][3] = FIXZERO;
  m.m[3][0] = FIXZERO; m.m[3][1] = FIXZERO; m.m[3][2] = FIXZERO; m.m[3][3] = FIXONE;
  vlt_mat44_mul(mat, &m);
}

void vlt_mat44_rot_y(vlt_mat44_t *mat, int angle)
{
  fix_t s = vlt_sin(angle);
  fix_t c = vlt_cos(angle);
  vlt_mat44_t m;
  m.m[0][0] = c;       m.m[0][1] = FIXZERO; m.m[0][2] = -s;      m.m[0][3] = FIXZERO;
  m.m[1][0] = FIXZERO; m.m[1][1] = FIXONE;  m.m[1][2] = FIXZERO; m.m[1][3] = FIXZERO;
  m.m[2][0] = s;       m.m[2][1] = FIXZERO; m.m[2][2] = c;       m.m[2][3] = FIXZERO;
  m.m[3][0] = FIXZERO; m.m[3][1] = FIXZERO; m.m[3][2] = FIXZERO; m.m[3][3] = FIXONE;
  vlt_mat44_mul(mat, &m);
}

void vlt_mat44_rot_z(vlt_mat44_t *mat, int angle)
{
  fix_t s = vlt_sin(angle);
  fix_t c = vlt_cos(angle);
  vlt_mat44_t m;
  m.m[0][0] = c;       m.m[0][1] = s;       m.m[0][2] = FIXZERO; m.m[0][3] = FIXZERO;
  m.m[1][0] = -s;      m.m[1][1] = c;       m.m[1][2] = FIXZERO; m.m[1][3] = FIXZERO;
  m.m[2][0] = FIXZERO; m.m[2][1] = FIXZERO; m.m[2][2] = FIXONE;  m.m[2][3] = FIXZERO;
  m.m[3][0] = FIXZERO; m.m[3][1] = FIXZERO; m.m[3][2] = FIXZERO; m.m[3][3] = FIXONE;
  vlt_mat44_mul(mat, &m);
}

void vlt_mat44_translate(vlt_mat44_t *mat, fix_t x, fix_t y, fix_t z)
{
  vlt_mat44_t m;
  m.m[0][0] = FIXONE;  m.m[0][1] = FIXZERO; m.m[0][2] = FIXZERO; m.m[0][3] = FIXZERO;
  m.m[1][0] = FIXZERO; m.m[1][1] = FIXONE;  m.m[1][2] = FIXZERO; m.m[1][3] = FIXZERO;
  m.m[2][0] = FIXZERO; m.m[2][1] = FIXZERO; m.m[2][2] = FIXONE;  m.m[2][3] = FIXZERO;
  m.m[3][0] = x;       m.m[3][1] = y;       m.m[3][2] = z;       m.m[3][3] = FIXONE;
  vlt_mat44_mul(mat, &m);
}

void vlt_mat44_scale(vlt_mat44_t *mat, fix_t sx, fix_t sy, fix_t sz)
{
  vlt_mat44_t m;
  m.m[0][0] = sx;      m.m[0][1] = FIXZERO; m.m[0][2] = FIXZERO; m.m[0][3] = FIXZERO;
  m.m[1][0] = FIXZERO; m.m[1][1] = sy;      m.m[1][2] = FIXZERO; m.m[1][3] = FIXZERO;
  m.m[2][0] = FIXZERO; m.m[2][1] = FIXZERO; m.m[2][2] = sz;      m.m[2][3] = FIXZERO;
  m.m[3][0] = FIXZERO; m.m[3][1] = FIXZERO; m.m[3][2] = FIXZERO; m.m[3][3] = FIXONE;
  vlt_mat44_mul(mat, &m);
}

void vlt_vec3_transform(vlt_vec3_t *dst, const vlt_vec3_t *src, const vlt_mat44_t *mat)
{
  dst->x = vlt_fix_mul(src->x, mat->m[0][0]) + vlt_fix_mul(src->y, mat->m[1][0]) + vlt_fix_mul(src->z, mat->m[2][0]) + mat->m[3][0];
  dst->y = vlt_fix_mul(src->x, mat->m[0][1]) + vlt_fix_mul(src->y, mat->m[1][1]) + vlt_fix_mul(src->z, mat->m[2][1]) + mat->m[3][1];
  dst->z = vlt_fix_mul(src->x, mat->m[0][2]) + vlt_fix_mul(src->y, mat->m[1][2]) + vlt_fix_mul(src->z, mat->m[2][2]) + mat->m[3][2];
}

void vlt_convert_to_screen(vlt_vec3_t *dst, const vlt_vec3_t *src, int width, int height)
{
  dst->x = vlt_fix_div(vlt_fix_mul(src->x, FIXONE), src->z) + (FIXONE >> 1);
  dst->y = vlt_fix_div(vlt_fix_mul(src->y, FIXONE), src->z) + (FIXONE >> 1);
  dst->z = src->z;
}


#define SAME_SIGNS(a, b) (((a) ^ (b)) >= 0)

// int ul_line_intersect_2d(const ul_vec2_t *p1, const ul_vec2_t *p2, const ul_vec2_t *p3, const ul_vec2_t *p4, ul_vec2_t *intersect)
enum VLT_LINE_INTERSECTION_TYPE vlt_line_intersect_fix2d(const vlt_line2_t *l1, const vlt_line2_t *l2, vlt_vec2_t *intersect)
{
#if 1
  fix_t a1, a2, b1, b2, c1, c2;
  fix_t r1, r2, r3, r4;
  fix_t denom, offset, num;

  a1 = l1->p2.y - l1->p1.y;
  b1 = l1->p1.x - l1->p2.x;
  c1 = vlt_fix_mul(l1->p2.x, l1->p1.y) - vlt_fix_mul(l1->p1.x, l1->p2.y);

  r3 = vlt_fix_mul(a1, l2->p1.x) + vlt_fix_mul(b1, l2->p1.y) + c1;
  r4 = vlt_fix_mul(a1, l2->p2.x) + vlt_fix_mul(b1, l2->p2.y) + c1;

  if ((r3 != FIXZERO) && (r4 != FIXZERO) && SAME_SIGNS(r3, r4))
    return VLT_LINE_DONT_INTERSECT;

  a2 = l2->p2.y - l2->p1.y;
  b2 = l2->p1.x - l2->p2.x;
  c2 = vlt_fix_mul(l2->p2.x, l2->p1.y) - vlt_fix_mul(l2->p1.x, l2->p2.y);

  r1 = vlt_fix_mul(a2, l1->p1.x) + vlt_fix_mul(b2, l1->p1.y) + c2;
  r2 = vlt_fix_mul(a2, l1->p2.x) + vlt_fix_mul(b2, l1->p2.y) + c2;

  if ((r1 != FIXZERO) && (r2 != FIXZERO) && SAME_SIGNS(r1, r2))
    return VLT_LINE_DONT_INTERSECT;

  denom = vlt_fix_mul(a1, b2) - vlt_fix_mul(a2, b1);
  if (denom == FIXZERO)
    return VLT_LINE_COLLINEAR;
  offset = denom < FIXZERO ? -(denom >> 1) : (denom >> 1);

  DBG("denom = %f\n", FIX2FLOAT(denom));
  DBG("offset = %f\n", FIX2FLOAT(offset));

  fix_t tmpa = vlt_fix_mul(b1, c2);
  fix_t tmpb = vlt_fix_mul(b2, c1);
  num = vlt_fix_mul(b1, c2) - vlt_fix_mul(b2, c1);
  DBG("b1 = %f, c2 = %f, b1 = %f, c1 = %f, b1c2 = %f, b2c1 = %f \n", FIX2FLOAT(b1), FIX2FLOAT(c2), FIX2FLOAT(b2), FIX2FLOAT(c1), FIX2FLOAT(tmpa), FIX2FLOAT(tmpb));
  DBG("b1 = %d, c2 = %d, b1c2 = %d\n", b1, c2, tmpa);
  DBG("num x = %f\n", FIX2FLOAT(num));
  intersect->x = vlt_fix_div((num < FIXZERO ? (num - offset) : (num + offset)), denom);

  num = vlt_fix_mul(a2, c1) - vlt_fix_mul(a1, c2);
  DBG("num y = %f\n", FIX2FLOAT(num));
  intersect->y = vlt_fix_div((num < FIXZERO ? (num - offset) : (num + offset)), denom);

  return VLT_LINE_INTERSECT;
#else
  fix a1 = p2->y - p1->y;
  fix b1 = p1->x - p2->x;
  fix c1 = a1 * p1->x + b1 * p1->y;
  fix a2 = p4->y - p3->y;
  fix b2 = p3->x - p4->x;
  fix c2 = a2 * p3->x + b2 * p3->y;
  fix det = a1 * b2 - a2 * b1;
  if (det == FIXZERO)
    return 0;
  intersect->x = (b2 * c1 - b1 * c2) / det;
  intersect->y = (a1 * c2 - a2 * c1) / det;
  return 1;
#endif
}

enum VLT_LINE_INTERSECTION_TYPE vlt_line_intersect_2d(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int *x, int *y)
{
  int a1, a2, b1, b2, c1, c2;
  int r1, r2, r3, r4;
  int denom, offset, num;

  // Compute a1, b1, c1, where line joining (x1, y1) to (x2, y2) is forced through (a1 x + b1 y + c1 = 0)
  a1 = y2 - y1;
  b1 = x1 - x2;
  c1 = x2 * y1 - x1 * y2;

  // Compute r3 and r4
  r3 = a1 * x3 + b1 * y3 + c1;
  r4 = a1 * x4 + b1 * y4 + c1;

  // Check signs of r3 and r4. If both point 3 and point 4 lie on same side of line 1,
  // the line segments do not intersect.
  if ((r3 != 0) && (r4 != 0) && SAME_SIGNS(r3, r4)) {
    return VLT_LINE_DONT_INTERSECT;
  }

  // Compute a2, b2, c2
  a2 = y4 - y3;
  b2 = x3 - x4;
  c2 = x4 * y3 - x3 * y4;

  // Compute r1 and r2
  r1 = a2 * x1 + b2 * y1 + c2;
  r2 = a2 * x2 + b2 * y2 + c2;

  // Check signs of r1 and r2. If both point 1 and point 2 lie on same side of line 2,
  // the line segments do not intersect.
  if ((r1 != 0) && (r2 != 0) && SAME_SIGNS(r1, r2)) {
    return VLT_LINE_DONT_INTERSECT;
  }

  // Compute denom
  denom = a1 * b2 - a2 * b1;
  if (denom == 0) {
    return VLT_LINE_COLLINEAR;
  }

  // Compute offset
  offset = denom < 0 ? -denom / 2 : denom / 2;

  DBG("denom: %d\n", denom);
  DBG("offset: %d\n", offset);

  // The denominator is not 0, so the lines
  // intersect somewhere. Compute the x and y
  // coordinates of the intersection point.
  num = b1 * c2 - b2 * c1;
  DBG("b1 = %d, c2 = %d, b2 = %d, c1 = %d, b1c2 = %d, b2c1 = %d\n", b1, c2, b2, c1, b1 * c2, b2 * c1);
  DBG("num x: %d\n", num);
  *x = (num < 0 ? num - offset : num + offset) / denom;
  num = a2 * c1 - a1 * c2;
  DBG("num y: %d\n", num);
  *y = (num < 0 ? num - offset : num + offset) / denom;

  return VLT_LINE_INTERSECT;
}

enum VLT_BOX_INTERSECTION_TYPE vlt_line_box_intersect_2d(int lx1, int ly1, int lx2, int ly2, int bx1, int by1, int bx2, int by2, int *x1, int *y1, int *x2, int *y2)
{
  // TBD
  return 0;
}


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* __VLT_H__ */
