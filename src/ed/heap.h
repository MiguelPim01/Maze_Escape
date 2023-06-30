#ifndef HEAP_H
#define HEAP_H

#include "hash.h"

typedef struct Heap Heap;

Heap *heap_construct(HashTable *hash);

void *heap_push(Heap *h, void *data, double prioridade);

void *heap_pop(Heap *h);

int heap_empty(Heap *h);

double heap_min_priority(Heap *h);

void heap_destroy(Heap *h);

#endif