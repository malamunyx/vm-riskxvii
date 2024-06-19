#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define HEAP_START (0xb700)
#define HEAP_CAPACITY (0x2000) // 8192
#define BANK_SIZE (64)

/* HEAP HISTORY */
enum node_type {
    SENTINEL,
    NODE
};

struct node {
    uint32_t idx;
    uint32_t size;
    struct node *next;
    enum node_type type;
};

uint32_t rnd_up_banks(uint32_t size);
struct node *hist_init();
uint32_t list_add(struct node *head, uint32_t size);
void list_delete(struct node *head, uint32_t idx);
void list_free(struct node *head);
struct node *get_access(struct node *head, uint32_t idx);
struct node *get_free(struct node *head, uint32_t idx);

#endif