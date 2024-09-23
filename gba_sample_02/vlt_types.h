/*
 *  vlt_types.h
 *  takehiko.terada@gmail.com
 * 
 *  Copyright (c) 2022, Takehiko Terada
 *  All rights reserved.
 */

#ifndef __VLT_TYPES_H__
#define __VLT_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
// Data types
*/
typedef unsigned char           u8_t;     /* Unsigned 8 bit value       */
typedef unsigned short int      u16_t;    /* Unsigned 16 bit value      */
typedef unsigned int            u32_t;    /* Unsigned 32 bit value      */
typedef signed char             i8_t;     /* Signed 8 bit value */
typedef signed short int        i16_t;    /* Signed 16 bit value        */
typedef signed int              i32_t;    /* Signed 32 bit value        */
typedef volatile u8_t           vu8_t;    /* volatile Unsigned 8 bit value      */
typedef volatile u16_t          vu16_t;   /* volatile Unigned 16 bit value      */
typedef volatile u32_t          vu32_t;   /* volatile Unsigned 32 bit value     */
typedef volatile i8_t           vi8_t;    /* volatile Signed 8 bit value        */
typedef volatile i16_t          vi16_t;   /* volatile Signed 8 bit value        */
typedef volatile i32_t          vi32_t;   /* volatile Signed 8 bit value        */
typedef int                     fix_t;    /* 16.16 fixed point */
#ifndef __cplusplus
/*! C++ compatible bool for C */
typedef enum { false, true } bool;
#endif

/* Structs */
typedef struct {
  fix_t x, y;
} vlt_vec2_t;

typedef struct {
  fix_t x, y, z;
} vlt_vec3_t;

typedef struct {
  fix_t v[4];
} vlt_vec4_t;

typedef struct {
  fix_t m[4][4]; /* = {
    { FIXONE, FIXZERO, FIXZERO, FIXZERO },
    { FIXZERO, FIXONE, FIXZERO, FIXZERO },
    { FIXZERO, FIXZERO, FIXONE, FIXZERO },
    { FIXZERO, FIXZERO, FIXZERO, FIXONE }
  }; */
} vlt_mat44_t;

typedef struct {
  i32_t num_vtx;
  vlt_vec3_t *vtx_list;
  vlt_vec3_t *nrm_list;
  vlt_vec2_t *uv_list;
  vlt_vec2_t *proj_list;
} vlt_vtx_t;

enum VLT_LINE_INTERSECTION_TYPE {
  VLT_LINE_DONT_INTERSECT = -1,
  VLT_LINE_COLLINEAR = 0,
  VLT_LINE_INTERSECT = 1,
};


enum VLT_BOX_INTERSECT_EDGE {
  LEFT  = 0x01,
  RIGHT = 0x02,
  ABOVE = 0x04,
  BELOW = 0x08
};


typedef struct {
  vlt_vec2_t p1, p2;
} vlt_line2_t;

typedef struct {
  i32_t t[3];
} vlt_tri_t;

enum VLT_BOX_INTERSECTION_TYPE {
  VLT_BOX_INSIDE = 0,
  VLT_BOX_OUTSIDE = 1,
  VLT_BOX_INTERSECT = 2,
  VLT_BOX_INTERSECT_TWICE = 3,
};

typedef struct {
  fix_t x1, y1, x2, y2;
} vlt_box_t;

typedef struct {
  i32_t num_tri;
  vlt_vtx_t  *vtx;
  vlt_tri_t  *tri;
} vlt_mesh_t;

typedef struct {
  vlt_vec3_t   *pos;
  vlt_vec4_t   *q;   // quaternion
  bool  flip;
  fix_t   *m;   // camera matrix
  void (*update)();
  void (*track)();
  void (*project)();
} vlt_camera_t;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __VLT_TYPES_H__