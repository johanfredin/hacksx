#ifndef PSX_GAME_OBJECT_H
#define PSX_GAME_OBJECT_H

#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

#define GOBJ_TYPE_PLAYER 0x0
#define GOBJ_TYPE_NPC 0x1

#define GOBJ_HEADING_NONE 0x0
#define GOBJ_HEADING_UP 0x1
#define GOBJ_HEADING_DOWN 0x2
#define GOBJ_HEADING_LEFT 0x4
#define GOBJ_HEADING_RIGHT 0x8

typedef struct GameObject {
    GsSPRITE *sprite;
    u_short w, h;
    u_char spawn_x, spawn_y, type;
    short x_speed, y_speed;
    u_char health: 7;
    u_char can_move: 1;
    u_char heading;
} GameObject;

typedef struct Animation {
    u_char key_frames;
    u_short ticks_per_frame;
    u_short curr_u;
    u_short curr_v;
    u_short acc_ticks;
} Animation;

typedef struct Player {
    Animation *anim;
    GameObject *gobj;
    u_char p_num;
    u_long curr_btn_pressed;
} Player;

#define GOBJ_RESET_POS(gobj)                \
    (gobj)->sprite->x = (gobj)->spawn_x;    \
    (gobj)->sprite->y = (gobj)->spawn_y

#define GOBJ_SWITCH_DIR(gobj)   \
    (gobj)->x_speed *= -1;      \
    (gobj)->y_speed *= -1

#define GOBJ_SWITCH_X_DIR(gobj) ((gobj)->x_speed *= -1)
#define GOBJ_SWITCH_Y_DIR(gobj) ((gobj)->y_speed *= -1)

#define GOBJ_IS_MOVING(gobj) ((gobj)->heading | GOBJ_HEADING_NONE)

#define GOBJ_ANIM_CYCLE_COMPLETE(anim) anim->acc_ticks >= (anim)->ticks_per_frame

#define GOBJ_IS_NOT_MOVING(gobj) !GOBJ_IS_MOVING(gobj)

/**
 * Create a new animation object on the heap with given properties
 * @param curr_u the starting texture frame x offset of the sprite we will work with
 * @param curr_v the starting texture frame y offset of the sprite we will work with
 * @param key_frames the amount of frames on the x axis of the sprite sheet passed in.
 * Eg. if sprite w=96 and keyFrames=3 then animation object will assume game object sprite has a width of 32 pixels and update animation in that fashion.
 * @param ticks_per_frame how many rendering iterations until its time to update frame.
 * @return pointer to a new animation object on the heap with given properties
 */
Animation *gobj_anim_init(u_short curr_u, u_short curr_v, u_char key_frames, u_short ticks_per_frame);

/**
 * Update passed in object with current animation state
 * @param anim animation object that will update sprite of passed in game object
 * @param gobj game object we want to animate.
 */
void gobj_anim_tick(Animation *anim, GameObject *gobj);

/**
 * Allocates heap memory for new game object
 * @param sprite the sprite
 * @param w width of sprite. If passing in a number smaller than full
 * width then only a portion of sprite will be displayed. Use this when
 * object takes in a sprite sheet.
 * @param h width of sprite. If passing in a number smaller than full
 * height then only a portion of sprite will be displayed. Use this when
 * object takes in a sprite sheet.
 * @param x_speed movement speed of the game object on the x axis
 * @param y_speed movement speed of the game object on the y axis
 * @param health health of the game object
 * @param type what type of game object this is (e.g player, NPC, key...
 * @return a pointer to a heap allocated game object with given parameters
 */
GameObject *gobj_init(GsSPRITE *sprite, short w, short h, short x_speed, short y_speed, u_char health, u_char type);
/**
 * Draw the game object
 * @param game_object the game object sprite to draw
 */
void gobj_draw(GameObject *game_object);

/**
 * Update the game object
 * @param game_object the game object to update
 * @param player how to interact with player
 */
void gobj_tick(GameObject *game_object);

Player *gobj_player_init(Animation *anim, GameObject *gobj, u_char p_num);
void gobj_player_draw(Player *p);
void gobj_player_tick(Player *p);

#endif // PSX_GAME_OBJECT_H

