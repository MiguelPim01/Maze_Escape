#ifndef _NODE_H_
#define _NODE_H_

typedef void *data_type;

typedef struct Node Node;

Node *node_construct(data_type value, Node *next);

data_type node_value(Node *n);

data_type *node_value_adress(Node *n);

Node *node_next(Node *n);

void node_set_next(Node *n, Node *next);

void node_destroy(Node *n);

#endif