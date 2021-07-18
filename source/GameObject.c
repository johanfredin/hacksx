#include "../header/GameObject.h"
#include "../header/GPUBase.h"
#include "../header/MemUtils.h"
#include "../header/Logger.h"

#include <LIBETC.H>

/**
 * Update the Heading struct of the passed in game object
 * @param gobj the game object owning the heading struct
 * @param l are we heading left?
 * @param r are we heading right?
 * @param u are we heading up?
 * @param d are we heading down?
 */
void set_heading(GameObject *gobj, u_char l, u_char r, u_char u, u_char d);

char is_moving(GameObject *gobj);

GameObject *gobj_init(GsSPRITE *sprite, short w, short h, char x_speed, char y_speed, u_char health, u_char type) {
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
    set_heading(gobj, 0, 0, 0, 0);
    LOGR_LOG_GOBJ(DEBUG, gobj);
    return gobj;
}

void gobj_draw(GameObject *game_object) {
    // if (GPUB_PRINT_COORDS) {
    //     FntPrint("gobj x=%d, gobj y=%d\n", game_object->sprite->x, game_object->sprite->y);
    // }
    GsSortFastSprite(game_object->sprite, gpub_curr_ot(), 1);
}

void gobj_tick(GameObject *game_object, Player *player) {
    game_object->sprite->x += game_object->x_speed;
    game_object->sprite->y += game_object->y_speed;
}

void set_heading(GameObject *gobj, u_char l, u_char r, u_char u, u_char d) {
    gobj->heading.left = l;
    gobj->heading.right = r;
    gobj->heading.up = u;
    gobj->heading.down = d;
}

char is_moving(GameObject *gobj) {
    return gobj->heading.left == 1 || gobj->heading.right == 1 || gobj->heading.up == 1 || gobj->heading.down == 1;
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
    if (GPUB_PRINT_COORDS) {
        FntPrint("x=%d, y=%d\n", p->gobj->sprite->x, p->gobj->sprite->y);
        FntPrint("left=%d, right=%d, up=%d, down=%d\n", p->gobj->heading.left, p->gobj->heading.right, p->gobj->heading.up, p->gobj->heading.down);
    }
    GsSortFastSprite(p->gobj->sprite, gpub_curr_ot(), 1);
}

void gobj_player_tick(Player *p) {
    short x_speed = 0, pxSpeed = p->gobj->x_speed;
    short y_speed = 0, pySpeed = p->gobj->y_speed;
    p->curr_btn_pressed = PadRead(p->p_num);
    set_heading(p->gobj, 0, 0, 0, 0);
    if (p->curr_btn_pressed & PADLdown) {
        y_speed = pySpeed;
        set_heading(p->gobj, 0, 0, 0, 1);
    }
    if (p->curr_btn_pressed & PADLup) {
        y_speed = -pySpeed;
        set_heading(p->gobj, 0, 0, 1, 0);
    }
    if (p->curr_btn_pressed & PADLright) {
        x_speed = pxSpeed;
        set_heading(p->gobj, 0, 1, 0, 0);
    }
    if (p->curr_btn_pressed & PADLleft) {
        x_speed = -pxSpeed;
        set_heading(p->gobj, 1, 0, 0, 0);
    }

    gobj_anim_tick(p->anim, p->gobj);
    p->gobj->sprite->x += x_speed;
    p->gobj->sprite->y += y_speed;
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
    short u = anim->curr_u; // Fetch current x offset
    short v = anim->curr_v; // Fetch current y offset
    short sprite_w = gobj->w;
    short sprite_h = gobj->h;
    if (gobj->heading.left) {
        v = sprite_h; // Move y offset down by the height of the objgect to next spritesheet row
    } else if (gobj->heading.right) {
        v = sprite_h * 2;
    } else if (gobj->heading.up) {
        v = sprite_h * 3;
    } else if (gobj->heading.down) {
        v = 0;
    }
    gobj->sprite->v = v;
    anim->curr_v = v;

    anim->acc_ticks += 1;

    if (GOBJ_FNTPRINT_ANIMATION) {
        FntPrint("Acc=%d, curr_u=%d, moving=%d\n", anim->acc_ticks, anim->curr_u, is_moving(gobj));
    }

    if (anim->acc_ticks >= anim->ticks_per_frame) {
        anim->acc_ticks = 0;
        if (u >= ((anim->key_frames * sprite_w) - sprite_w)) { // If x offset is at last horizontal frame, reset it to 0
            u = 0;
        } else {
            if (is_moving(gobj)) {
                u += sprite_w; // Move to next frame when not at the end of horizontal sheet.
            }
        }
    }
    anim->curr_u = u;
    gobj->sprite->u = u;
}
