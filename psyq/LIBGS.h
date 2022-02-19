#ifndef _HACKSX_MOCK_GS_
#define _HACKSX_MOCK_GS_

#include "../header/Logger.h"

#ifndef NULL
#define NULL 0
#endif

typedef struct GsIMAGE {
    unsigned long pmode;
    short px, py;
    unsigned short pw, ph;
    unsigned long *pixel;
    short cx, cy;
    unsigned short cw, ch;
    unsigned long *clut;
} GsIMAGE;

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

void GsGetTimInfo(unsigned long *im, GsIMAGE *tim);
extern int LoadImage(RECT *rect, u_long *p);

#define GsSortFastSprite(sprite, ot, prio) \
LOGR_LOG_GS_OBJ(INFO, sprite)

#define GsSortFastBg(bg, ot, pri) LOGR_LOG_GS_BG(TRACE, bg)

#define GsInit3D()  logr_log(TRACE, "Logger.h", "LIBGS.h", "GsInit3D called")

#define GsSortPoly(poly, ot, prio) \
logr_log(TRACE, "Logger.h", "LIBGS.h", "poly sorted")

#endif