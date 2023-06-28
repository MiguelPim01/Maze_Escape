
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "types.h"

typedef struct Queue Queue;

Queue *queue_construct();
void queue_push(Queue *queue, void *data);
bool queue_empty(Queue *queue);
int queue_size(Queue *queue);
void *queue_pop(Queue *queue);
void queue_destroy(Queue *queue, void (*fptr_destroy)(void *));

#endif