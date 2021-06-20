#include <stdarg.h>
#include "SYS/TYPES.h"
#include "LIBGPU.h"
#include "LIBETC.h"
#include "LIBGTE.h"
#include "LIBGS.h"

extern int FntPrint(char msg, ...) {
    logr_log(INFO, "MockPSXTypes.c", "FntPrint", "FNT print...");
    return 1;
}

u_long PadRead(u_long btn) {
    return 1L;
}

u_short GetTPage(int tp, int abr, int x, int y) {
    return 1;
}

u_short GetClut(int x, int y) {
    return 1;
}

void GsGetTimInfo(unsigned long *im, GsIMAGE *tim) {
    tim->px = tim->py = tim->cx = tim->cy = 0;
    tim->pw = tim->ph = tim->cw = tim->ch = 32;
    tim->clut = NULL;
    tim->pixel = NULL;
}

extern int LoadImage(RECT *rect, u_long *p) {
    return 1;
}

extern void SetTile(TILE *p) {
    // mock
}