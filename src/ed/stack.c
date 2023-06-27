#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "deque.h"

struct Stack {
    Deque *deque;
    int size;
};

Stack *stack_construct()
{
    Stack *s = (Stack *)malloc(sizeof(Stack));

    s->deque = deque_construct();
    s->size = 0;

    return s;
}

void stack_push(Stack *s, void *value)
{
    deque_push_back(s->deque, value);
    s->size++;
}

bool stack_empty(Stack *s)
{
    return s->size == 0;
}

int stack_size(Stack *stack)
{
    return stack->size;
}

void *stack_pop(Stack *s)
{
    if (s->size == 0)
        return NULL;

    void *rtn = deque_pop_back(s->deque);
    s->size--;

    return rtn;
}

void stack_destroy(Stack *s, void (*fprt_destroy)(void *))
{
    deque_destroy(s->deque, fprt_destroy);

    free(s);
}