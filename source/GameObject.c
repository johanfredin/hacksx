#include "../header/GameObject.h"
#include "../header/GPUBase.h"
#include "../header/MemUtils.h"
#include "../header/FntLogger.h"
#include "../header/AssetManager.h"
#include "../header/SoundEffect.h"

#include <LIBETC.H>

GameObject *gobj_init(GsSPRITE *sprite, short w, short h, short x_speed, short y_speed, u_char health, u_char type) {
    GameObject *gobj = MEM_MALLOC_3(GameObject);
    gobj->w = w;
    gobj->h = h;
    gobj->sprite = sprite;
    gobj->sprite->w = w; // Set width portion of sprite to render
    gobj->sprite->h = h; // Set height portion of sprite to render
    gobj->x_speed = x_speed;
    gobj->y_speed = y_speed;
    gobj->health = health;
    gobj->spawn_x = sprite->x;
    gobj->spawn_y = sprite->y;
    gobj->type = type;
    gobj->heading = 0x0;
    LOGR_LOG_GOBJ(DEBUG, gobj);
    return gobj;
}

void gobj_draw(GameObject *game_object) {
    FNT_PRINT_GOBJ_COORDS(game_object)
    GPUB_GS_SORT_FAST_OBJ(game_object->sprite);
}

void gobj_tick(GameObject *game_object) {
    game_object->sprite->x = (short) (game_object->sprite->x + game_object->x_speed);
    game_object->sprite->y = (short) (game_object->sprite->y + game_object->y_speed);
}

Player *gobj_player_init(Animation *anim, GameObject *gobj, u_char p_num) {
    Player *p = MEM_MALLOC_3(Player);
    p->anim = anim;
    p->gobj = gobj;
    p->p_num = p_num;
    p->curr_btn_pressed = 0;
    return p;
}

void gobj_player_draw(Player *p) {
    FNT_PRINT_GOBJ_COORDS(p->gobj)
    GPUB_GS_SORT_FAST_OBJ(p->gobj->sprite);
}

void gobj_player_tick(Player *p) {
    short x_speed = 0, pxSpeed = p->gobj->x_speed;
    short y_speed = 0, pySpeed = p->gobj->y_speed;
    p->curr_btn_pressed = PadRead(p->p_num);
    p->gobj->heading=0x0;
    if (p->curr_btn_pressed & PADLdown) {
        y_speed = pySpeed;
        p->gobj->heading = GOBJ_HEADING_DOWN;
    }
    if (p->curr_btn_pressed & PADLup) {
        y_speed = (short) -pySpeed;
        p->gobj->heading = GOBJ_HEADING_UP;
    }
    if (p->curr_btn_pressed & PADLleft) {
        x_speed = (short) -pxSpeed;
        p->gobj->heading = GOBJ_HEADING_LEFT;
    }
    if (p->curr_btn_pressed & PADLright) {
        x_speed = pxSpeed;
        p->gobj->heading = GOBJ_HEADING_RIGHT;
    }

    gobj_anim_tick(p->anim, p->gobj);
    p->gobj->sprite->x = (short)(p->gobj->sprite->x + x_speed);
    p->gobj->sprite->y = (short)(p->gobj->sprite->y + y_speed);
}

Animation *gobj_anim_init(u_short curr_u, u_short curr_v, u_char key_frames, u_short ticks_per_frame) {
    Animation *anim = MEM_MALLOC_3(Animation);
    anim->curr_u = curr_u;
    anim->curr_v = curr_v;
    anim->key_frames = key_frames;
    anim->ticks_per_frame = ticks_per_frame;
    anim->acc_ticks = 0;
    return anim;
}

void gobj_anim_tick(Animation *anim, GameObject *gobj) {
    u_short u = anim->curr_u; // current x offset
    u_short v = anim->curr_v; // current y offset
    u_short sprite_w = gobj->w;
    u_short sprite_h = gobj->h;

    if (GOBJ_IS_NOT_MOVING(gobj)) {
        // Set x offset and acquired ticks to 0 when not moving
        gobj->sprite->u = 0;
        anim->acc_ticks = 0;
        anim->curr_u = 0;
        return;
    }

    // Handle y offset
    if (gobj->heading & GOBJ_HEADING_LEFT) {
        v = sprite_h;       // 2nd row in sprite-sheet
    } else if (gobj->heading & GOBJ_HEADING_RIGHT) {
        v = sprite_h << 1;  // 3rd row in sprite-sheet
    } else if (gobj->heading & GOBJ_HEADING_UP) {
        v = sprite_h * 3;   // 4th row in sprite-sheet
    } else if (gobj->heading & GOBJ_HEADING_DOWN) {
        v = 0;              // 1st row in sprite-sheet
    }

    gobj->sprite->v = v;
    anim->curr_v = v;
    FNT_PRINT_ANIMATION()

    if (GOBJ_ANIM_CYCLE_COMPLETE(anim)) {
        anim->acc_ticks = 0;
        if (u >= ((anim->key_frames * sprite_w) - sprite_w)) { // If x offset is at last horizontal frame, reset it to 0
            u = 0;
        } else {
            u += sprite_w; // Move to next frame when not at the end of horizontal sheet.
        }
        if(u & sprite_w) {
            ASMG_AUDIO_PLAY(SFX_WALK);
        }
    } else {
        anim->acc_ticks++;
    }

    anim->curr_u = u;
    gobj->sprite->u = u;
}
