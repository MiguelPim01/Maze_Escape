#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "deque.h"

struct Queue {
    Deque *deque;
    int size;
};

Queue *queue_construct()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));

    q->deque = deque_construct();
    q->size = 0;

    return q;
}

void queue_push(Queue *q, void *data)
{
    deque_push_back(q->deque, data);
    q->size++;
}

bool queue_empty(Queue *q)
{
    return q->size == 0;
}

void *queue_pop(Queue *q)
{
    if (q->size == 0)
        return NULL;

    void *rtn = deque_pop_front(q->deque);
    q->size--;

    return rtn;
}

void queue_destroy(Queue *q, void (*fptr_destroy)(void *))
{
    deque_destroy(q->deque, fptr_destroy);

    free(q);
}