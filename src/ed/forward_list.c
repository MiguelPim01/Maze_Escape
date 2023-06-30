#include <stdlib.h>
#include <stdio.h>

#include "forward_list.h"

struct ForwardList {
    Node *head;
    int size;
};

struct ForwardListIterator {
    Node *current;
};

ForwardList *forward_list_construct()
{
    ForwardList *lista = (ForwardList *)malloc(sizeof(ForwardList));

    lista->head = NULL;
    lista->size = 0;

    return lista;
}

int forward_list_size(ForwardList *l)
{
    return l->size;
}

void forward_list_push_front(ForwardList *l, data_type data)
{
    Node *novo = node_construct(data, l->head);

    l->head = novo;
    l->size++;
}

void forward_list_print(ForwardList *l, void (*print_fn)(data_type))
{
    Node *nAux = l->head;

    printf("[");
    while (nAux != NULL)
    {
        print_fn(node_value(nAux));

        if (node_next(nAux) ==  NULL)
            break;
        
        printf(", ");

        nAux = node_next(nAux);
    }
    printf("]");
}

data_type forward_list_get(ForwardList *l, int i)
{
    Node *n = l->head;

    if (i < 0 || i > l->size)
    {
        printf("Indice invalido!\n");
        return NULL;
    }
    
    for (int j = 0; j < i; j++)
    {
        n = node_next(n);
    }

    return node_value(n);
}

data_type forward_list_pop_front(ForwardList *l)
{
    Node *n_clear = l->head;
    data_type d = node_value(n_clear);

    l->head = node_next(n_clear);

    node_destroy(n_clear);

    return d;
}

ForwardList *forward_list_reverse(ForwardList *l)
{
    ForwardList *l2 = forward_list_construct();
    Node *n = l->head;
    
    for (int i = 0; i < forward_list_size(l); i++)
    {
        forward_list_push_front(l2, node_value(n));
        n = node_next(n);
    }

    return l2;
}

void forward_list_clear(ForwardList *l)
{
    int size = l->size;

    for (int i = 0; i < size; i++)
    {
        forward_list_pop_front(l);
    }
}

void forward_list_remove(ForwardList *l, data_type val)
{
    Node *n = l->head, *prev = NULL,*new_n = NULL;

    while (n != NULL) 
    {
        if (node_value(n) == val) 
        {
            if (prev == NULL)
            {
                new_n = node_next(n);
                l->head = new_n;
            }
            else
            {
                new_n = node_next(n);
                node_set_next(prev, new_n);
            }
            
            node_destroy(n);
            n = new_n;
            l->size--;
        }
        else 
        {
            prev = n;
            n = node_next(n);
        }
    }
}

void forward_list_unique(ForwardList *l)
{
    Node *n = l->head, *d, *nAux;
    int flag = 0;

    while (n != NULL)
    {
        nAux = node_next(n);

        while (nAux != NULL)
        {
            if (node_value(n) == node_value(nAux))
            {
                d = nAux;
                l->size--;
                flag = 1;
            }

            nAux = node_next(nAux);
            if (flag)
                node_destroy(d);
            flag = 0;
        }
        
        n = node_next(n);
    }
}

void forward_list_cat(ForwardList *l, ForwardList *m)
{
    Node *n = m->head;

    while (n)
    {
        forward_list_push_front(l, node_value(n));
        n = node_next(n);
    }
}

void forward_list_sort(ForwardList *l)
{
    // Node *node = l->head;
    // Node *nAux;
    // data_type *change, aux;
    // int flag = 0;

    // while (node != NULL)
    // {
    //     nAux = node_next(node);

    //     while (nAux != NULL)
    //     {
    //         if (node_value(node) > node_value(nAux))
    //         {
    //             change = &(node_value(nAux));
    //             flag = 1;
    //         }

    //         nAux = node_next(nAux);
    //     }

    //     if (flag)
    //     {
    //         aux = *change;
    //         *change = node_value(node);
    //         node_value(node) = aux;
    //     }

    //     flag = 0;
    //     node = node_next(node);
    // }
}

void forward_list_destroy(ForwardList *l, void (*fptr_destroy)(void *))
{
    Node *next = l->head;
    data_type data;

    while (next != NULL)
    {
        data = node_value(next);
        l->head = node_next(next);
        node_destroy(next);

        fptr_destroy(data);
        next = l->head;
    }

    free(l);
}

// =========== ITERADOR PARA LISTAS ENCADEADAS ===========

// O(1): Cria 1 iterador de forward_list e faz uma atribuição
ForwardListIterator *forward_list_front_iterator(ForwardList *l)
{
    ForwardListIterator *it = (ForwardListIterator *)malloc(sizeof(ForwardListIterator));

    it->current = l->head;

    return it;
}

// O(1): Faz uso apenas de funções O(1) e realiza atribuições
data_type *forward_list_iterator_next(ForwardListIterator *it)
{
    data_type *data = node_value_adress(it->current);

    it->current = node_next(it->current);

    return data;
}

// O(1): Realiza apenas uma verificação
int forward_list_iterator_is_over(ForwardListIterator *it)
{
    if (it->current == NULL)
        return 1;
    
    return 0;
}

Node *forward_list_iterator_current(ForwardListIterator *it)
{
    return it->current;
}

// O(1): Da free() em um ponteiro
void forward_list_iterator_destroy(ForwardListIterator *it)
{
    free(it);
}