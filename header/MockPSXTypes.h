//
// Fills in the gap between psx sdk and this mock project
//

#include "Logger.h"

#ifndef HACKSX_MOCKPSXTYPES_H
#define HACKSX_MOCKPSXTYPES_H

typedef unsigned char u_char;

typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned int u_int;

void FntPrint(char *c, ...);

u_short GetTPage(int tp, int abr, int x, int y);
u_short GetClut(int x, int y);

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

typedef struct GsSPRITE {
    unsigned long attribute;
    short x, y;
    unsigned short w, h;
    unsigned short tpage;
    unsigned char u, v;
    short cx, cy;
    unsigned char r, g, b;
    short mx, my;
    short scalex, scaley;
    long rotate;
} GsSPRITE;

typedef struct {
    unsigned long offset;
    unsigned long point;
} GsOT;

typedef struct GsCELL {
    unsigned char u, v;
    unsigned short cba;
    unsigned short flag;
    unsigned short tpage;
} GsCELL;

typedef struct GsMAP {
    unsigned char cellw, cellh;
    unsigned short ncellw, ncellh;
    GsCELL *base;
    unsigned short *index;
} GsMAP;

typedef struct GsBG {
    unsigned long attribute;
    short x, y;
    short w, h;
    short scrollx, scrolly;
    unsigned char r, g, b;
    GsMAP *map;
    short mx, my;
    short scalex, scaley;
    long rotate;
} GsBG;


#define DrawPrim(tile) \
logr_log(INFO, "MockPSXTypes.h", "DrawPrim", "Drawing Primes")

#define GsSortFastSprite(sprite, ot, prio) \
logr_log(INFO, "MockPSXTypes.h", "GsSortGastSprite", "Drawing Sprite")

#define SetTile(tile) \
logr_log(INFO, "MockPSXTypes.h", "SetTile", "Setting Tile")

#define setRGB0(object, r, g, b) \
logr_log(INFO, "MockPSXTypes.h", "setRGB0", "Setting rgb")

#define ONE         4096    /* GTE regards 4096 as 1.0 */

#define PADLdown    0
#define PADLup      1
#define PADLright   2
#define PADLleft    3
#define PADselect   4

#endif //HACKSX_MOCKPSXTYPES_H
