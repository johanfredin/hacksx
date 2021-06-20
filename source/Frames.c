#include "Frames.h"

void frames_insert_beginning(SpriteLayer **root, SpriteLayer *new_layer) {
    new_layer->next = *root;
    *root = new_layer;
}

void frames_insert_after(SpriteLayer *layer, SpriteLayer *new_layer) {
    new_layer->next = layer->next;
    layer->next = new_layer;
}

void frames_insert_sl_sorted(SpriteLayer **root, SpriteLayer *new_layer) {
    if (*root == NULL || (*root)->prio >= new_layer->prio) {
        frames_insert_beginning(root, new_layer);
        return;
    }

    SpriteLayer *curr;
    for (curr = *root; curr->next != NULL; curr = curr->next) {
        if (curr->next->prio >= new_layer->prio) {
            break;
        }
    }
    frames_insert_after(curr, new_layer);
}

