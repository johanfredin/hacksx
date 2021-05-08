//
// Fills in the gap between psx sdk and this mock project
//

#include "Logger.h"

#ifndef HACKSX_MOCKPSXTYPES_H
#define HACKSX_MOCKPSXTYPES_H

#define FntPrint(c, args) \
logr_log(INFO, c, args)

#define DrawPrim(tile) \
logr_log(INFO, "Drawing Primes")

typedef unsigned char u_char;
typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned int u_int;

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
    short x, y;
    unsigned short w, h;
    long rotate;
} GsSPRITE;

typedef struct {
    unsigned long offset;
    unsigned long point;
} GsOT;

#endif //HACKSX_MOCKPSXTYPES_H
