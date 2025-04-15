#include "llist.h"

void fr_core_llist_push(struct llist_node* new, struct llist_head* head) {
    new->next = head->head;
    head->head = new;
    if (!head->tail) {
        head->tail = new;
    }
}
