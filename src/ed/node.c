#include <stdlib.h>

#include "node.h"

struct Node {
    data_type value;
    Node *next;
};

Node *node_construct(data_type value, Node *next)
{
    Node *n = (Node *)malloc(sizeof(Node));
    n->value = value;
    n->next = next;
    return n;
}

data_type node_value(Node *n)
{
    return n->value;
}

data_type *node_value_adress(Node *n)
{
    return &n->value;
}

Node *node_next(Node *n)
{
    return n->next;
}

void node_set_next(Node *n, Node *next)
{
    n->next = next;
}

void node_destroy(Node *n)
{
    free(n);
}