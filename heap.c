#include "heap.h"

uint32_t rnd_up_banks(uint32_t size) {
    if (size == 0) {
        return 0;
    }

    uint32_t mod = size % BANK_SIZE;

    if (mod == 0) {
        return size;
    }

    return size + (BANK_SIZE - mod);
}

struct node *hist_init() {
    struct node *tmp = malloc(sizeof(struct node));

    if (tmp == NULL) {
        return NULL;
    }

    tmp->idx = 0;
    tmp->size = 0;
    tmp->next = 0;
    tmp->type = SENTINEL;
    return tmp;
}

uint32_t list_add(struct node *head, uint32_t size) {
    /* Cannot allocate an empty (or negative) size */
    if (size <= 0) {
        return 0;
    }

    struct node *n1 = head;
    struct node *n2 = NULL;

    while ((n2 = n1->next) != NULL) {
        if (size <= (n2->idx - (n1->idx + rnd_up_banks(n1->size)))) {
            break;
        }
        n1 = n1->next;
    }

    uint32_t n_idx = n1->idx + rnd_up_banks(n1->size);
    if (n_idx + size > HEAP_CAPACITY) {
        return 0;
    }

    struct node *n3 = malloc(sizeof(struct node));
    n3->idx = n_idx;
    n3->size = size;
    n3->type = NODE;

    n1->next = n3;
    n3->next = n2; // can be null, or next val.

    return n3->idx + HEAP_START; // Index position.
}

void list_delete(struct node *head, uint32_t idx) {
    /* Cannot act on a NULL */
    if (head == NULL) {
        return;
    }

    /* Headptr is a sentinel we want to start after it. */ 
    struct node *prev = head;
    struct node *del = head->next; 

    /* The node is empty. */
    if (del == NULL) {
        return;
    }

    /* Iterate through the list until we either find end of list, or node. */
    while (del != NULL && del->idx != idx) {
        prev = del;
        del = del->next;
    }

    // Delete index is found
    if (del->idx == idx) {
        prev->next = del->next;
        free(del);
    }
}

void list_free(struct node *head) {
    if (head == NULL) {
        return;
    }

    struct node *cursor = head;
    while (cursor != NULL) {
        struct node *next = cursor->next;
        free(cursor);
        cursor = next;
    }
}

/* Helper function for get */
int32_t in_bounds(int32_t var, int32_t boundary1, int32_t boundary2) {
    return (var >= boundary1 && var < boundary2);
}
struct node *get_access(struct node *head, uint32_t idx) {
    struct node *cursor = head->next;
    while (cursor != NULL) {
        if (in_bounds(idx, cursor->idx, cursor->idx + cursor->size)) {
            break;
        }
        cursor = cursor->next;
    }
        
    if (cursor == NULL) {
        return NULL;
    } else {
        return cursor;
    }
}
struct node *get_free(struct node *head, uint32_t idx) {
    struct node *cursor = head->next;
    while (cursor != NULL) {
        if (idx == cursor->idx) {
            break;
        }
        cursor = cursor->next;
    }
        
    if (cursor == NULL) {
        return NULL;
    } else {
        return cursor;
    }
}

