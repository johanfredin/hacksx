#ifndef PSX_ASSET_MANAGER_H
#define PSX_ASSET_MANAGER_H

#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

#include "CdReader.h"
#include "Tiled.h"

#define ASMG_COLOR_BITS_4 0x4
#define ASMG_COLOR_BITS_8 0x8
#define ASMG_COLOR_BITS_16 0x10
#define ASMG_SOUND_MALLOC_MAX 0xA
#define ASMG_VAG_HEADER_BYTE_SIZE 0x30
#define ASMG_BLEND_NORMAL 128

typedef struct VramAsset {
    RECT *frame_buffer;
    RECT *clut;
    u_short t_width_multiplier;
    u_short color_mode;
    u_char is_clut_mode;
    u_long sprite_attr;
} VramAsset;

#define ASMG_FREE_VRAM_ASSET(asset)             \
    MEM_FREE_3_AND_NULL((asset)->frame_buffer); \
    MEM_FREE_3_AND_NULL((asset)->clut);         \
    MEM_FREE_3_AND_NULL(asset)

void asmg_init_tim_data(GsIMAGE *tim_data, CdrData *cdr_data, u_short num_color_bits);
void asmg_load_asset(VramAsset *vram_asset, CdrData *cdr_data, u_short num_color_bits);
void asmg_get_region(GsSPRITE *base, GsSPRITE *region, u_short x, u_short y, u_short u, u_short v, u_short w, u_short h);

GsSPRITE *asmg_load_8_bit_sprite(CdrData *cdr_data, u_short x, u_short y);
GsSPRITE *asmg_load_sprite(CdrData *cdr_data, u_short x, u_short y, u_short blend, u_short num_color_bits);

void asmg_audio_init();
void asmg_transfer_vag_to_spu(CdrData *cdr_data, u_long voice_channel);

#define ASMG_AUDIO_PLAY_IF_NOT_PLAYING_ALREADY(voice_channel) \
if(SpuGetKeyStatus(voice_channel) == SpuOff) {                 \
    ASMG_AUDIO_PLAY(voice_channel);                           \
}

#define ASMG_AUDIO_PLAY(voice_channel) SpuSetKey(SpuOn, voice_channel)

#define ASMG_AUDIO_STOP(voice_channel) SpuSetKey(SpuOff, voice_channel)

#define ASMG_AUDIO_FREE(spu_address) SpuFree(spu_address)

/**
 * All VAGs have a 48 byte header, which must be removed for playback. This header should not be removed
 * before converting VAGs to VABs on the Mac or PC; otherwise, improper conversion will occur.
 */
#define ASMG_AUDIO_SKIP_VAG_HEADER(vag) ((vag) + ASMG_VAG_HEADER_BYTE_SIZE)

#endif // PSX_ASSET_MANAGER_H
