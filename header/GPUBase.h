#ifndef PSX_GPU_BASE_H
#define PSX_GPU_BASE_H

#include "MemUtils.h"
#include "Logger.h"
#include "MockPSXTypes.h"

// Constants
#define GPUB_NUM_BUFFERS 2
#define GPUB_OT_LENGTH 5
#define GPUB_BUFFERS_LENGTH 1
#define GPUB_MODE_NTSC 0
#define GPUB_MODE_PAL 1
#define GPUB_ROT_ONE 4096
#define GPUB_PACKET_MAX 3000
#define GPUB_FRAME_BUFFER_WIDTH 1024
#define GPUB_FRAME_BUFFER_HEIGHT 512

#define GPUB_SET_BRIGHTNESS(rgb_holder, brightness) rgb_holder->r = rgb_holder->g = rgb_holder->b =  brightness

#define GPUB_SCREEN_WIDTH 256    // Width of screen
#define GPUB_SCREEN_HEIGHT 256   // Height of screen

#define GPUB_BG_COLOR_R 0

#define GPUB_BG_COLOR_G 0
#define GPUB_BG_COLOR_B 0

#define GPUB_IS_FNT_BG 0

#define GPUB_DRAW_BOUNDS 0           // Set to 1 to draw map bounds as red rectangles
#define GPUB_PRINT_COORDS 1          // Set to 1 to FntPrint player coordinates to screen


// Prototypes
void gpub_init_screen();
void gpub_display();
void gpub_clear_display();
GsOT* gpub_curr_ot();

#endif // PSX_GPU_BASE_H
