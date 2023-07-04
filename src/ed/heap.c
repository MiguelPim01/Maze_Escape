#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

#define CAPACIDADE 10

typedef struct HeapNode {
    void *data;
    double prioridade;
} HeapNode;

struct Heap {
    HeapNode *nodes;
    HashTable *hash_table;
    int size;
    int capacidade;
};

void _change_hash_index(Heap *h, int i, int parent)
{
    int *change_index_var = NULL, *destroy_index_var = NULL;

    change_index_var = (int *)malloc(sizeof(int));
    *change_index_var = i;
    destroy_index_var = hash_table_set(h->hash_table, h->nodes[i].data, change_index_var);
    free(destroy_index_var);

    change_index_var = (int *)malloc(sizeof(int));
    *change_index_var = parent;
    destroy_index_var = hash_table_set(h->hash_table, h->nodes[parent].data, change_index_var);
    free(destroy_index_var);
}

void _correct_heap(Heap *h, int index)
{
    HeapNode node = {NULL, 0.0};

    for (int i = index, parent = (int)((i - 1)/2); i != 0; i = parent, parent = (int)((parent - 1)/2))
    {
        if (h->nodes[i].prioridade < h->nodes[parent].prioridade)
        {
            node = h->nodes[i];
            h->nodes[i] = h->nodes[parent];
            h->nodes[parent] = node;

            _change_hash_index(h, i, parent);
        }
        else
            break;
    }
}

Heap *heap_construct(HashTable *hash)
{
    Heap *h = (Heap *)malloc(sizeof(Heap));

    h->nodes = (HeapNode *)calloc(CAPACIDADE, sizeof(HeapNode));
    h->capacidade = CAPACIDADE;
    h->size = 0;
    h->hash_table = hash;

    return h;
}

void *heap_push(Heap *h, void *data, double prioridade)
{
    int *index = hash_table_get(h->hash_table, data);

    if (index != NULL)
    {
        if (prioridade < h->nodes[*index].prioridade)
        {
            h->nodes[*index].prioridade = prioridade;

            _correct_heap(h, *index);
        }

        return data;
    }

    if (h->size == h->capacidade)
    {
        h->capacidade *= 2;
        h->nodes = (HeapNode *)realloc(h->nodes, sizeof(HeapNode)*h->capacidade);
    }

    h->nodes[h->size].data = data;
    h->nodes[h->size].prioridade = prioridade;

    index = (int *)malloc(sizeof(int));
    *index = h->size;

    hash_table_set(h->hash_table, data, index);

    _correct_heap(h, h->size);

    h->size++;
    return NULL;
}

void *heap_pop(Heap *h)
{
    void *rtn = h->nodes[0].data;

    int left_child = 1, right_child = 2, i = 0;
    HeapNode node = {NULL, 0.0};

    int *index = hash_table_pop(h->hash_table, h->nodes[0].data);
    free(index);

    if (h->size == 1)
    {
        h->size--;
        return rtn;
    }

    index = (int *)malloc(sizeof(int));
    *index = 0;
    index = hash_table_set(h->hash_table, h->nodes[h->size-1].data, index);
    free(index);

    h->size--;
    h->nodes[0] = h->nodes[h->size];

    while (1)
    {
        if (left_child < h->size && right_child < h->size)
        {
            if (h->nodes[left_child].prioridade <= h->nodes[right_child].prioridade)
            {
                if (h->nodes[i].prioridade > h->nodes[left_child].prioridade)
                {
                    node = h->nodes[left_child];
                    h->nodes[left_child] = h->nodes[i];
                    h->nodes[i] = node;

                    _change_hash_index(h, i, left_child);

                    i = left_child;
                    left_child = 2*i + 1;
                    right_child = 2*i + 2;
                }
                else
                    break;
            }
            else
            {
                if (h->nodes[i].prioridade > h->nodes[right_child].prioridade)
                {
                    node = h->nodes[right_child];
                    h->nodes[right_child] = h->nodes[i];
                    h->nodes[i] = node;

                    _change_hash_index(h, i, right_child);

                    i = right_child;
                    left_child = 2*i + 1;
                    right_child = 2*i + 2;
                }
                else
                    break;
            }
        }
        else if (left_child < h->size)
        {
            if (h->nodes[i].prioridade > h->nodes[left_child].prioridade)
            {
                node = h->nodes[left_child];
                h->nodes[left_child] = h->nodes[i];
                h->nodes[i] = node;

                _change_hash_index(h, i, left_child);
            }
            break;
        }
        else
            break;
    }

    return rtn;
}

int heap_empty(Heap *h)
{
    if (h->size == 0)
        return 1;
    return 0;
}

double heap_min_priority(Heap *h)
{
    return h->nodes[0].prioridade;
}

void heap_destroy(Heap *h, void (*destroy)(void *))
{
    for (int i = 0; i < h->size; i++)
        destroy(h->nodes[i].data);

    free(h->nodes);
    free(h);
}