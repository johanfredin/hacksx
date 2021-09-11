#ifndef PSX_GAME_OBJECT_H
#define PSX_GAME_OBJECT_H

#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

#define GOBJ_TYPE_PLAYER 0
#define GOBJ_TYPE_NPC 1
#define GOBJ_FNTPRINT_ANIMATION 0 // Set to 1 to FntPrint player animation to screen

typedef struct Heading {
    u_int left : 1;
    u_int right : 1;
    u_int up : 1;
    u_int down : 1;
} Heading;

typedef struct {
    GsSPRITE *sprite;
    u_short w, h;
    u_char spawn_x, spawn_y, type;
    short x_speed, y_speed;
    u_char health;
    Heading heading;
} GameObject;

typedef struct {
    u_char key_frames;
    u_short ticks_per_frame;
    u_short curr_u;
    u_short curr_v;
    u_short acc_ticks;
} Animation;

typedef struct {
    Animation *anim;
    GameObject *gobj;
    u_char p_num;
    u_long curr_btn_pressed;
} Player;

#define GOBJ_RESET_POS(gobj)         \
    gobj->sprite->x = gobj->spawn_x; \
    gobj->sprite->y = gobj->spawn_y

#define GOBJ_SWITCH_DIR(gobj) \
    gobj->x_speed *= -1;      \
    gobj->y_speed *= -1

#define GOBJ_SWITCH_X_DIR(gobj) gobj->x_speed *= -1
#define GOBJ_SWITCH_Y_DIR(gobj) gobj->y_speed *= -1

/**
 * Create a new animation object on the heap with given properties
 * @param curr_u the starting texture frame x offset of the sprite we will work with
 * @param curr_v the starting texture frame y offset of the sprite we will work with
 * @param key_frames the amount of map_frames on the x axis of the spritesheet passed in. Eg. if sprite w=96 and keyFrames=3 then animation object will assume gameobject sprite has a width of 32 pixels and update annimation in that fassion.
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
GameObject *gobj_init(GsSPRITE *sprite, short w, short h, char x_speed, char y_speed, u_char health, u_char type);
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
void gobj_tick(GameObject *game_object, Player *player);

Player *gobj_player_init(Animation *anim, GameObject *gobj, u_char p_num);
void gobj_player_draw(Player *p);
void gobj_player_tick(Player *p);

#endif // PSX_GAME_OBJECT_H

