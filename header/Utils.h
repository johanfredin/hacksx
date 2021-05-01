//
// Created by lowrider on 4/4/2021.
//

#ifndef BADJSON_UTILS_H
#define BADJSON_UTILS_H

#define STREQ(str1, str2) strcmp(str1, str2) == 0
#define STR_LEN(i, str) while ((char) str[i] != '\0') i++
#define FREE_AND_NULL(ptr) if(ptr != NULL) free(ptr); ptr = NULL

#define FREE_LINKED_LIST(linked_list_root, T)   \
while (linked_list_root != NULL) {              \
    T *linked_list_aux = linked_list_root;      \
    linked_list_root = linked_list_root->next;  \
    FREE_AND_NULL(linked_list_aux);             \
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
#define MALLOC_AND_MOVE_TO_NEXT_IF_MORE_DATA(ptr_to_check, curr_entry, ptr_type)     \
if (ptr_to_check->next == NULL) {                                                    \
    curr_entry->next = NULL;                                                         \
} else {                                                                             \
    curr_entry->next = malloc(sizeof(ptr_type));                                     \
    curr_entry = curr_entry->next;                                                   \
}

#endif //BADJSON_UTILS_H
