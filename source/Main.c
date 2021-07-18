#include <Frames.h>
#include <Map.h>
#include <stdio.h>
#include <ctype.h>
#include <StrUtils.h>
#include "../header/GPUBase.h"

int main() {
    gpub_init_screen(320, 240, GPUB_MODE_NTSC);
    map_init(1);
//    map_draw(NULL);

    char *source = "..\\/..\\/tilesets\\/ts8_tl.tsx";
    char tim[16] = "TS8_TL.TIM";

    // 1. To lower
    STR_TO_LOWERCASE(tim);
    printf("tim now=%s\n", tim);

    // 2. Substr until first .
    char substr[16];
    int count = 0;
    STR_READ_UNTIL(tim, substr, '.', count);
    printf("tim now=%s\n", substr);

    // 3. Look for a match in source
    if(STR_CONTAINS(source, substr)) {
        printf("WE have a match!\n");
    }

    return 0;
}
