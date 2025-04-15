/**
 * @file llist.h
 * @author Aditya Rajagopal
 * @brief Contains an implementation of a linked list
 * @details
 * @version 0.0.1
 * @date 2024-02-17
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */

#include "fracture/core/defines.h"

// TODO: Add a tail node reference as well to allow using append and searching from the back
// TODO: Maybe also store a length value to be tracked
typedef struct llist_head {
    struct llist_node* head;
    struct llist_node* tail;
} llist_head;

typedef struct llist_node {
    struct llist_node* next;
    struct llist_node* prev;
} llist_node;

#define fr_llist_for_each(position, node) for (typeof(*(node))*(position) = (node); (pos); (pos) = (pos)->next)
#define fr_llist_for_each_safe(pos, node) \
    for (typeof(*(node))*(pos) = (node), *n; (pos) && (n = (pos)->next, 1); (pos) = n)

FR_API void fr_core_llist_push(struct llist_node* new, struct llist_head* head);

// TODO: Create a push/pop/insert_at/pop_at/append/length
// TODO: Decide on if the removal API is going to require a position or a reference to the node
// TODO: Decide if doubly linked list is a different module or to be made within this module
// TODO: Add a bit more syntactic sugar to help access elements in the linked list easier?
