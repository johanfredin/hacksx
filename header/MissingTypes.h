//
// Fills in the gap between psx sdk and this mock project
//

#ifndef HACKSX_MISSINGTYPES_H
#define HACKSX_MISSINGTYPES_H

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

#endif //HACKSX_MISSINGTYPES_H
