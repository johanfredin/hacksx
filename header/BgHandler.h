#ifndef _BgHandler_
#define _BgHandler_

#include "GPUBase.h"
#include "GameObject.h"
#include "AssetManager.h"

/*
Havent yet figured scolling out, so keeping what i have so far as reference
*/

GsBG* gs_bg;
u_long* assets[4];

void loadBG();

void loadBG() {
    GsCELL gsCells[10*3];
    GsMAP gsMap;
    VramAsset asset;
    u_short xCoord;
    u_short yCoord;
    u_short tileW = 16;
    u_short tileH = 16;
    u_short mapW;
    u_short mapH;
    u_short index;
    gs_bg = (GsBG*) malloc3(sizeof(GsBG));
    asmg_load_asset(&asset, "TILES_8", assets[0], COLOR_BITS_8);
    
    mapW = asset.frame_buffer->w;
    mapH = asset.frame_buffer->h;

    // Fetch cells
    index = 0;
    printf("\n------------------------------\nStarting retrieval of map cells\n------------------------------\n");
    for(yCoord = 0; yCoord < mapH; yCoord += tileH) {
        for(xCoord = 0; xCoord < mapW; xCoord += tileW) {
            GsCELL cell;
            cell.u = xCoord / 2;
            cell.v = yCoord;
            cell.cba = GetClut(asset.clut->x, asset.clut->y);
            cell.flag = 0;
            cell.tpage = GetTPage(asset.color_mode, 1, asset.frame_buffer->x, asset.frame_buffer->y);
            gsCells[index] = cell;
            index++;
            printf("Cell added at xCoord=%d, yCoord=%d: u=%d, v=%d, cx=%d, cy=%d, index=%d\n", xCoord, yCoord, cell.u, cell.v, asset.clut->x, asset.clut->y, index);
        }    
    }

    // Setup map
    gsMap.cellw = 16;
    gsMap.cellh = 16;
    gsMap.ncellw = 10;
    gsMap.ncellh = 3;
    gsMap.base = gsCells;
    gsMap.index = &gsCells[0];
    printf("\n------------------------------\nMap stats: {cellW:%d, cellH:%d, nCellW:%d, nCellH:%d, base:%p, index:%d}\n", gsMap.cellw, gsMap.cellh, gsMap.ncellw, gsMap.ncellh, gsMap.base, gsMap.index);

    // Setup BG
    gs_bg->attribute = asset.sprite_attr;
    gs_bg->x = 0;
    gs_bg->y = 0;
    gs_bg->w = asset.frame_buffer->w * asset.t_width_multiplier;
    gs_bg->h = asset.frame_buffer->h;
    gs_bg->mx = 0;
    gs_bg->my = 0;
    gs_bg->scalex = ONE * 1;
    gs_bg->scaley = ONE * 1;
    gs_bg->rotate = GPUB_ROT_ONE * 0;
    gs_bg->r = gs_bg->g = gs_bg->b = 128;
    gs_bg->scrollx = 0;
    gs_bg->scrolly = 0;
    gs_bg->map = &gsMap;
}

#endif