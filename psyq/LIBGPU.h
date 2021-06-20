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

#define setRGB0(p, _r0, _g0, _b0)                        \
    (p)->r0 = _r0,(p)->g0 = _g0,(p)->b0 = _b0

#define DrawPrim(p) \
logr_log(DEBUG, "LIBGPU.H", "DrawPrim", "Drawing prime")

extern int LoadImage(RECT *rect, u_long *p);
extern u_short GetClut(int x, int y);
extern u_short GetTPage(int tp, int abr, int x, int y);
extern int FntPrint(char msg, ...);
extern void SetTile(TILE *p);

#endif