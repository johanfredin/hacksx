#include <Map.h>
#include <stdio.h>
#include <ctype.h>
#include <StrUtils.h>
#include "../header/GPUBase.h"

int main() {
    gpub_init_screen(320, 240, GPUB_MODE_NTSC);
    map_init(1);
//    map_draw(NULL);

    return 0;
}
