#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
#include "forward_list.h"

struct HashTable {
    ForwardList **buckets;
    HashFunction hash_fn;
    CmpFunction cmp_fn;
    int table_size;
    int n_elements;
};

struct HashTableIterator {
    ForwardList **buckets;
    ForwardListIterator *list_iterator;
    int table_size;
    int current_bucket;
};

HashTableItem *_hash_pair_construct(void *key, void *val)
{
    HashTableItem *p = calloc(1, sizeof(HashTableItem));
    
    p->key = key;
    p->val = val;
    
    return p;
}

void _hash_pair_destroy(void *pair)
{
    pair = (HashTableItem *)pair;

    free(pair);
}

HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn)
{
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));

    ht->cmp_fn = cmp_fn;
    ht->hash_fn = hash_fn;
    ht->n_elements = 0;
    ht->table_size = table_size;
    ht->buckets = (ForwardList **)calloc(table_size, sizeof(ForwardList *));

    return ht;
}

void *hash_table_set(HashTable *ht, void *key, void *val)
{
    int key_value = ht->hash_fn(ht, key);

    if (ht->buckets[key_value] == NULL)
        ht->buckets[key_value] = forward_list_construct();
    
    ForwardListIterator *it = forward_list_front_iterator(ht->buckets[key_value]);
    data_type *data;
    HashTableItem *cmp_pair;
    void *rtn_val;

    while (!forward_list_iterator_is_over(it))
    {
        data = forward_list_iterator_next(it);
        cmp_pair = *data;

        if (ht->cmp_fn(cmp_pair->key, key) == 0)
        {
            rtn_val = cmp_pair->val;
            cmp_pair->val = val;
            forward_list_iterator_destroy(it);
            return rtn_val;
        }
    }
    ht->n_elements++;
    
    HashTableItem *pair = _hash_pair_construct(key, val);
    forward_list_push_front(ht->buckets[key_value], pair);

    forward_list_iterator_destroy(it);

    return NULL;
}

void *hash_table_get(HashTable *ht, void *key)
{
    int key_value = ht->hash_fn(ht, key);

    if (ht->buckets[key_value] == NULL)
        return NULL;

    ForwardListIterator *it = forward_list_front_iterator(ht->buckets[key_value]);
    data_type *data;
    HashTableItem *cmp_pair;

    while (!forward_list_iterator_is_over(it))
    {
        data = forward_list_iterator_next(it);
        cmp_pair = *data;

        if (ht->cmp_fn(cmp_pair->key, key) == 0)
        {
            forward_list_iterator_destroy(it);
            return cmp_pair->val;
        }
    }

    forward_list_iterator_destroy(it);
    return NULL;
}

void *hash_table_pop(HashTable *ht, void *key)
{
    int key_value = ht->hash_fn(ht, key);

    if (ht->buckets[key_value] == NULL)
        return NULL;

    ForwardListIterator *it = forward_list_front_iterator(ht->buckets[key_value]);
    data_type *data;
    HashTableItem *cmp_pair;
    void *rtn = NULL;

    while (!forward_list_iterator_is_over(it))
    {
        data = forward_list_iterator_next(it);
        cmp_pair = *data;

        if (ht->cmp_fn(cmp_pair->key, key) == 0)
        {
            forward_list_iterator_destroy(it);
            ht->n_elements--;
            rtn = cmp_pair->val;
            forward_list_remove(ht->buckets[key_value], cmp_pair);

            if (forward_list_size(ht->buckets[key_value]) == 0)
            {
                forward_list_destroy(ht->buckets[key_value], _hash_pair_destroy);
                ht->buckets[key_value] = NULL;
            }

            _hash_pair_destroy(cmp_pair);
            return rtn;
        }
    }

    forward_list_iterator_destroy(it);
    return NULL;
}

int hash_table_size(HashTable *ht)
{
    return ht->table_size;
}

void hash_table_destroy(HashTable *ht)
{
    for (int i = 0; i < ht->table_size; i++)
    {
        if (ht->buckets[i] != NULL)
        {
            forward_list_destroy(ht->buckets[i], _hash_pair_destroy);
        }
    }

    free(ht->buckets);
    free(ht);
}


// =========== ITERADOR PARA HASH TABLES ===========

HashTableIterator *hash_table_iterator(HashTable *ht)
{
    HashTableIterator *it = (HashTableIterator *)malloc(sizeof(HashTableIterator));

    it->buckets = ht->buckets;
    it->table_size = ht->table_size;
    it->current_bucket = 0;
    it->list_iterator = NULL;

    return it;
}

HashTableItem *hash_table_iterator_next(HashTableIterator *it)
{
    if (it->list_iterator == NULL)
        it->list_iterator = forward_list_front_iterator(it->buckets[it->current_bucket]);

    if (forward_list_iterator_is_over(it->list_iterator))
    {
        it->current_bucket++;
        forward_list_iterator_destroy(it->list_iterator);
        it->list_iterator = NULL;
        return NULL;
    }

    void **pair = forward_list_iterator_next(it->list_iterator);
    HashTableItem *rtn_pair = *pair;

    return rtn_pair;
}

int hash_table_iterator_is_over(HashTableIterator *it)
{
    if (it->current_bucket >= it->table_size)
        return 1;

    while (it->buckets[it->current_bucket] == NULL)
    {
        it->current_bucket++;
        if (it->current_bucket >= it->table_size)
            return 1;
    }

    return 0;
}

void hash_table_iterator_destroy(HashTableIterator *it)
{
    free(it);
}