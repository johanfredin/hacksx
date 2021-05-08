#ifndef PSX_MEM_UTILS_H
#define PSX_MEM_UTILS_H

#include <stdlib.h>

#define MEM_HEAP_DEFAULT_START_ADDR 0x800F8000
#define MEM_HEAP_DEFAULT_SIZE 0x00100000

#define MEM_INIT_HEAP_3() InitHeap((unsigned long *)MEM_HEAP_DEFAULT_START_ADDR, MEM_HEAP_DEFAULT_SIZE)

#define MEM_MALLOC_3(type) (type *)malloc(sizeof(type))
#define MEM_MALLOC_3_CUS_SIZE(type, size) (type *)malloc(size)
#define MEM_CALLOC_3_PTRS(n, t) (t **)calloc((size_t)n, sizeof(t *))
#define MEM_CALLOC_3(n, t) (t *)calloc((size_t)n, sizeof(t))
#define MEM_FREE_3_AND_NULL(ptr) \
    if (ptr != NULL)             \
        free(ptr);              \
    ptr = NULL

#define MEM_FREE_LINKED_LIST(linked_list_root, T)  \
    while (linked_list_root != NULL) {             \
        T *linked_list_aux = linked_list_root;     \
        linked_list_root = linked_list_root->next; \
        MEM_FREE_3_AND_NULL(linked_list_aux);      \
    }

/**
 * Create another entry in the linked list we are creating if the linked
 * list we are iterating has a next entry. This macro depends on both
 * ptr_to_check and curr_entry to be linked list entries with a *next pointer
 * property in them.
 * @param ptr_to_check the current entry in the linked list we are iterating
 * @param curr_entry the current entry in the linked list we are creating
 * @param ptr_type the type of the curr_entry
 */
#define MEM_MALLOC_3_AND_MOVE_TO_NEXT_IF_MORE_DATA(ptr_to_check, curr_entry, ptr_type) \
    if (ptr_to_check->next == NULL) {                                                  \
        curr_entry->next = NULL;                                                       \
    } else {                                                                           \
        curr_entry->next = MEM_MALLOC_3(ptr_type);                                     \
        curr_entry = curr_entry->next;                                                 \
    }

#endif // PSX_MEM_UTILS_H
