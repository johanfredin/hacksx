#ifndef _HACKSX_MOCK_GPU_H_
#define _HACKSX_MOCK_GPU_H_

#include "SYS/TYPES.h"
#include "../header/Logger.h"

typedef struct RECT {
    short x, y;        /* offset point on VRAM */
    short w, h;        /* width and height */
} RECT;

typedef struct TILE {
    u_char r0, g0, b0;
    short x0, y0;
    short w, h;
} TILE;

typedef struct POLY_G4 {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char r1, g1, b1, pad1;
    short x1, y1;
    u_char r2, g2, b2, pad2;
    short x2, y2;
    u_char r3, g3, b3, pad3;
    short x3, y3;
} POLY_G4;

#define setRGB0(p, _r0, _g0, _b0)                        \
    (p)->r0 = _r0,(p)->g0 = _g0,(p)->b0 = _b0

#define setXYWH(p, _x0, _y0, _w, _h)                    \
    (p)->x0 = (_x0),      (p)->y0 = (_y0),                \
    (p)->x1 = (_x0)+(_w), (p)->y1 = (_y0),                \
    (p)->x2 = (_x0),      (p)->y2 = (_y0)+(_h),            \
    (p)->x3 = (_x0)+(_w), (p)->y3 = (_y0)+(_h)

#define setXY0(p, _x0, _y0)                        \
    (p)->x0 = (_x0), (p)->y0 = (_y0)                \

#define setRGB1(p, _r1, _g1, _b1)                        \
    (p)->r1 = _r1,(p)->g1 = _g1,(p)->b1 = _b1

#define setRGB2(p, _r2, _g2, _b2)                        \
    (p)->r2 = _r2,(p)->g2 = _g2,(p)->b2 = _b2

#define setRGB3(p, _r3, _g3, _b3)                        \
    (p)->r3 = _r3,(p)->g3 = _g3,(p)->b3 = _b3


#define DrawPrim(p) \
logr_log(DEBUG, "LIBGPU.H", "DrawPrim", "Drawing prime")

extern void SetPolyG4(POLY_G4 *p);
extern int LoadImage(RECT *rect, u_long *p);
extern u_short GetClut(int x, int y);
extern u_short GetTPage(int tp, int abr, int x, int y);
extern int FntPrint(char *msg, ...);
extern void SetTile(TILE *p);

#endif