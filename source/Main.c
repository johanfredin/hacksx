#include <Frames.h>
#include <MemUtils.h>
#include <stdio.h>


int main() {
    SpriteLayer *l1 = MEM_MALLOC_3(SpriteLayer);
    SpriteLayer *l2 = MEM_MALLOC_3(SpriteLayer);
    SpriteLayer *l3 = MEM_MALLOC_3(SpriteLayer);
    SpriteLayer *root = NULL;

    l1->prio = 2;
    l2->prio = 5;
    l3->prio = 1;

    frames_insert_sl_sorted(&root, l1);
    frames_insert_sl_sorted(&root, l2);
    frames_insert_sl_sorted(&root, l3);

    SpriteLayer *curr;
    for (curr = root; curr != NULL; curr = curr->next) {
        printf("Layer prio=%d\n", curr->prio);
    }

    return 0;
}
