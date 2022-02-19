#include <stdio.h>
#include "../header/Logger.h"
#include "../header/MemUtils.h"
#include "../header/AssetManager.h"
#include "../header/GPUBase.h"

GsIMAGE TexInfo;
GsBG BGData;
GsMAP BGMap;

CdrData data_tim, data_cel, data_bgd;

void initBG();

int main() {
    Color bg_color = {20, 90, 80};
    gpub_init_screen(320, 240, GPUB_MODE_NTSC, GPUB_FPS_60);
    gpub_set_bg_color(&bg_color);
    GsInit3D();
    MEM_INIT_HEAP_3()
    CDR_INIT()
    data_tim = *cdr_read_file("SAMPLE.TIM");
    data_cel = *cdr_read_file("SAMPLE.CEL");
    data_bgd = *cdr_read_file("SAMPLE.BGD");
    CDR_CLOSE()

    asmg_init_tim_data(&TexInfo, &data_tim, ASMG_COLOR_BITS_4);
    initBG();

    while (1) {
        gpub_clear_display();
        GPUB_GS_SORT_FAST_BG(&BGData);
        gpub_display();
    }
}

void initBG() {
    int i , j;
    u_char *cel;
    u_char *bgd;
    GsCELL *cell;

    cel = (u_char *) data_cel.file;
    cel += 8;
    cell = (GsCELL *) cel;

    bgd = (u_char *) data_bgd.file;
    bgd += 3;

    BGMap.ncellw = *bgd++;
    BGMap.ncellh = *bgd++;
    BGMap.cellw = *bgd++;
    BGMap.cellh = *bgd++;
    BGMap.base = cell;
    BGMap.index = (u_short *) bgd;

    BGData.attribute = ((TexInfo.pmode & 0x03) << 24);
    BGData.x = 0;
    BGData.y = 0;
    BGData.scrollx = BGData.scrolly = 0;
    BGData.r = BGData.g = BGData.b = ASMG_BLEND_NORMAL;
    BGData.map = &BGMap;
    BGData.mx = 320 / 2;
    BGData.my = 224 / 2;
    BGData.scalex = BGData.scaley = GPUB_ROT_ONE;
    BGData.rotate = 0;
    BGData.w = 320;
    BGData.h = 224;

    LOGR_LOG_GS_BG(INFO, &BGData);
    LOGR_LOG_GS_MAP(INFO, &BGMap);

    printf("Cell addr=%p\n", cell - MEM_HEAP_START_ADDR);
    printf("Index=%p\n", BGMap.index - MEM_HEAP_START_ADDR);
    for(i = 0; i < BGMap.ncellh; i++) {
        for(j = 0; j < BGMap.ncellw; j++) {
            printf("%p\t", &cell[i] - MEM_HEAP_START_ADDR);
        }
        printf("\n");
    }
}
