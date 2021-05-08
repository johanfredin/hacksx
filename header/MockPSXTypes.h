//
// Fills in the gap between psx sdk and this mock project
//

#include "Logger.h"

#ifndef HACKSX_MOCKPSXTYPES_H
#define HACKSX_MOCKPSXTYPES_H

#define DrawPrim(tile) \
logr_log(INFO, "Drawing Primes")

#define GsSortFastSprite(sprite, ot, prio) \
logr_log(INFO, "Drawing Sprite")

#define SetTile(tile) \
logr_log(INFO, "Setting Tile")

#define setRGB0(object, r, g, b) \
logr_log(INFO, "Setting rgb")

#define PADLdown 0
#define PADLup 1
#define PADLright 2
#define PADLleft 3
#define PADselect 4

typedef unsigned char u_char;
typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned int u_int;

void FntPrint(char *c, ...);
u_long PadRead(u_long btn);

typedef struct {
    short x, y;        /* offset point on VRAM */
    short w, h;        /* width and height */
} RECT;

typedef struct {
    u_char r0, g0, b0;
    short x0, y0;
    short w, h;
} TILE;

typedef struct {
    unsigned long attribute;
    short x, y, u, v;
    unsigned short w, h;
    long rotate;
} GsSPRITE;

typedef struct {
    unsigned long offset;
    unsigned long point;
} GsOT;

#endif //HACKSX_MOCKPSXTYPES_H
