//
// Created by lowrider on 6/18/2021.
//

#include <LinkedListTutorial.h>
#include <Frames.h>
#include <MemUtils.h>

void insert_beginning(SpriteLayer **root, u_short prio) {
    SpriteLayer *new_layer = MEM_MALLOC_3(SpriteLayer);
    new_layer->prio = prio;
    new_layer->next = *root;
    *root = new_layer;
}

void insert_after(SpriteLayer *layer, u_short prio) {
    SpriteLayer *new_layer = MEM_MALLOC_3(SpriteLayer);
    new_layer->prio = prio;
    new_layer->next = layer->next;
    layer->next = new_layer;
}

void insert_sorted(SpriteLayer **root, u_short prio) {
    if (*root == NULL || (*root)->prio >= prio) {
        insert_beginning(root, prio);
        return;
    }

    SpriteLayer *curr;
    for (curr = *root; curr->next != NULL; curr = curr->next) {
        if (curr->next->prio >= prio) {
            break;
        }
    }
    insert_after(curr, prio);
}

void lltut_run() {
    SpriteLayer *root = NULL;
    SpriteLayer *curr;
    insert_sorted(&root, 5);
    insert_sorted(&root, 4);
    insert_sorted(&root, 1);
    insert_sorted(&root, 17);
    insert_sorted(&root, 1);

    for (curr = root; curr != NULL; curr = curr->next) {
        logr_log(INFO, "Main.c", "main", "Curr prio=%d", curr->prio);
    }
}
