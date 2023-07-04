#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "algorithms.h"
#include "../ed/stack.h"
#include "../ed/queue.h"
#include "../ed/heap.h"

typedef struct Node {
    Celula atual;
    struct Node *prev;
    double h, g;
} Node;

Node *_node_construct(int x, int y, Node *prev)
{
    Node *n = (Node *)malloc(sizeof(Node));

    n->atual.x = x;
    n->atual.y = y;
    n->prev = prev;

    return n;
}

double _g(Node *n, Node *prev)
{
    int dx, dy;
    double g;
    if (prev != NULL)
    {
        g = prev->g;
        dx = n->atual.x - prev->atual.x;
        dy = n->atual.y - prev->atual.y;
        g += sqrt(pow(dx, 2) + pow(dy, 2));
    }
    else
        g = 0;
    
    return g;
}

double _h(Node *n, Node *fim)
{
    int dx, dy;
    double h;

    dx = fim->atual.x - n->atual.x;
    dy = fim->atual.y - n->atual.y;
    h = sqrt(pow(dx, 2) + pow(dy, 2));

    return h;
}

double _f(Node *n)
{
    return n->g + n->h;
}

Node *_node_construct_a_star(int x, int y, Node *prev, Node *fim)
{
    Node *n = (Node *)malloc(sizeof(Node));

    n->atual.x = x;
    n->atual.y = y;
    n->prev = prev;

    n->g = _g(n, prev);
    n->h = _h(n, fim);

    return n;
}

int _node_hash(HashTable *h, void *key)
{
    Node *c = (Node *)key;
    // 83 e 97 sao primos e o operador "^" é o XOR bit a bit
    return ((c->atual.x * 83) ^ (c->atual.y * 97)) % hash_table_size(h);
}

int _cmp_node_hash(void *c1, void *c2)
{
    Node *a = (Node *)c1;
    Node *b = (Node *)c2;

    if (a->atual.x == b->atual.x && a->atual.y == b->atual.y)
        return 0;
    return 1;
}

int _cmp_node(Node *c1, Node *c2)
{
    if (c1->atual.x == c2->atual.x && c1->atual.y == c2->atual.y)
        return 1;
    return 0;
}

void _node_destroy(void *node)
{
    Node *n = node;

    free(n);
}

void _node_expand_bfs(Node *n, Labirinto *l, Queue *to_expand, Queue *expanded)
{
    Celula celulas[8] = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

    int pos_x, pos_y;
    byte c;
    for (int i = 0; i < 8; i++)
    {
        pos_x = n->atual.x + celulas[i].x;
        pos_y = n->atual.y + celulas[i].y;

        if (pos_x < 0 || pos_x >= labirinto_n_colunas(l))
            continue;
        if (pos_y < 0 ||  pos_y >= labirinto_n_linhas(l))
            continue;
        
        c = labirinto_obter(l, pos_y, pos_x);
        if (c == OCUPADO || c == EXPANDIDO || (pos_x == n->atual.x && pos_y == n->atual.y) || c == FRONTEIRA)
            continue;
        
        queue_push(to_expand, _node_construct(pos_x, pos_y, n));
        labirinto_atribuir(l, pos_y, pos_x, FRONTEIRA);
    }
    labirinto_atribuir(l, n->atual.y, n->atual.x, EXPANDIDO);
    queue_push(expanded, n);
}

void _node_expand_dfs(Node *n, Labirinto *l, Stack *to_expand, Stack *expanded)
{
    Celula celulas[8] = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

    int pos_x, pos_y;
    byte c;
    for (int i = 0; i < 8; i++)
    {
        pos_x = n->atual.x + celulas[i].x;
        pos_y = n->atual.y + celulas[i].y;

        if (pos_x < 0 || pos_x >= labirinto_n_colunas(l))
            continue;
        if (pos_y < 0 ||  pos_y >= labirinto_n_linhas(l))
            continue;
        
        c = labirinto_obter(l, pos_y, pos_x);
        if (c == OCUPADO || c == EXPANDIDO || (pos_x == n->atual.x && pos_y == n->atual.y) || c == FRONTEIRA)
            continue;
        
        stack_push(to_expand, _node_construct(pos_x, pos_y, n));
        labirinto_atribuir(l, pos_y, pos_x, FRONTEIRA);
    }
    labirinto_atribuir(l, n->atual.y, n->atual.x, EXPANDIDO);
    stack_push(expanded, n);
}

void _node_expand_a_star(Node *n, Node *fim, Labirinto *l, Heap *to_expand, Stack *expanded)
{
    Celula celulas[8] = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

    int pos_x, pos_y;
    byte c;
    Node *clear = NULL;
    for (int i = 0; i < 8; i++)
    {
        pos_x = n->atual.x + celulas[i].x;
        pos_y = n->atual.y + celulas[i].y;

        if (pos_x < 0 || pos_x >= labirinto_n_colunas(l))
            continue;
        if (pos_y < 0 ||  pos_y >= labirinto_n_linhas(l))
            continue;
        
        c = labirinto_obter(l, pos_y, pos_x);
        if (c == OCUPADO || c == EXPANDIDO || (pos_x == n->atual.x && pos_y == n->atual.y))
            continue;
        
        clear = _node_construct_a_star(pos_x, pos_y, n, fim);
        clear = heap_push(to_expand, clear, _f(clear));
        if (clear)
            _node_destroy(clear);
        
        labirinto_atribuir(l, pos_y, pos_x, FRONTEIRA);
    }
    labirinto_atribuir(l, n->atual.y, n->atual.x, EXPANDIDO);
    stack_push(expanded, n);
}

void _path_reverse(Celula *path, int tamanho_caminho)
{
    int pos_x, pos_y;
    for (int i = 0; i < tamanho_caminho/2; i++)
    {
        pos_x = path[i].x;
        pos_y = path[i].y;
        path[i].x = path[tamanho_caminho-i-1].x;
        path[i].y = path[tamanho_caminho-i-1].y;
        path[tamanho_caminho-i-1].x = pos_x;
        path[tamanho_caminho-i-1].y = pos_y;
    }
}

ResultData _default_result()
{
    ResultData result;

    result.caminho = NULL;
    result.custo_caminho = 0;
    result.nos_expandidos = 0;
    result.tamanho_caminho = 0;
    result.sucesso = 0;

    return result;
}

ResultData a_star(Labirinto *l, Celula inicio, Celula fim)
{
    ResultData result = _default_result();

    HashTable *hash = hash_table_construct(87, _node_hash, _cmp_node_hash);
    Heap *fronteira = heap_construct(hash);
    Stack *expandidos = stack_construct();
    Node *pos_atual = NULL, *pos_fin = _node_construct(fim.x, fim.y, NULL), *path_node = NULL, *pos_ini = _node_construct_a_star(inicio.x, inicio.y, NULL, pos_fin);

    heap_push(fronteira, pos_ini, _f(pos_ini));

    while (heap_empty(fronteira) == 0)
    {
        pos_atual = heap_pop(fronteira);

        if (_cmp_node(pos_atual, pos_fin))
        {
            result.sucesso = 1;
            stack_push(expandidos, pos_atual);
            _node_destroy(pos_fin);
            pos_fin = pos_atual;
            result.nos_expandidos++;
            break;
        }

        _node_expand_a_star(pos_atual, pos_fin, l, fronteira, expandidos);
        result.nos_expandidos++;
    }

    result.caminho = (Celula *)malloc(sizeof(Celula)*stack_size(expandidos));
    int dx, dy;

    if (result.sucesso)
    {
        path_node = pos_fin;
        while (path_node->prev != NULL)
        {
            result.caminho[result.tamanho_caminho].x = path_node->atual.x;
            result.caminho[result.tamanho_caminho].y = path_node->atual.y;
            result.tamanho_caminho++;
            path_node = path_node->prev;

            dx = path_node->atual.x - result.caminho[result.tamanho_caminho-1].x;
            dy = path_node->atual.y - result.caminho[result.tamanho_caminho-1].y;
            result.custo_caminho += sqrt(pow(dx, 2) + pow(dy, 2));
        }
        result.caminho[result.tamanho_caminho].x = path_node->atual.x;
        result.caminho[result.tamanho_caminho].y = path_node->atual.y;
        result.tamanho_caminho++;

        _path_reverse(result.caminho, result.tamanho_caminho);
    }
    else
        _node_destroy(pos_fin);
    
    HashTableIterator *it = hash_table_iterator(hash);

    while (!hash_table_iterator_is_over(it))
    {
        HashTableItem *item = hash_table_iterator_next(it);
        if (item == NULL)
            continue;

        int *pos = (int *)item->val;
        free(pos);
    }

    hash_table_iterator_destroy(it);
    hash_table_destroy(hash);
    heap_destroy(fronteira, _node_destroy);
    stack_destroy(expandidos, _node_destroy);

    return result;
}

ResultData breadth_first_search(Labirinto *l, Celula inicio, Celula fim)
{
    ResultData result = _default_result();

    Queue *fronteira = queue_construct();
    queue_push(fronteira, _node_construct(inicio.x, inicio.y, NULL));
    Queue *expandidos = queue_construct();
    Node *pos_atual = NULL, *pos_fin = _node_construct(fim.x, fim.y, NULL), *path_node = NULL;

    while (queue_empty(fronteira) == 0)
    {
        pos_atual = queue_pop(fronteira);

        if (_cmp_node(pos_atual, pos_fin))
        {
            result.sucesso = 1;
            queue_push(expandidos, pos_atual);
            _node_destroy(pos_fin);
            pos_fin = pos_atual;
            result.nos_expandidos++;
            break;
        }

        _node_expand_bfs(pos_atual, l, fronteira, expandidos);
        result.nos_expandidos++;
    }

    result.caminho = (Celula *)malloc(sizeof(Celula)*queue_size(expandidos));
    int dx, dy;

    if (result.sucesso)
    {
        path_node = pos_fin;
        while (path_node->prev != NULL)
        {
            result.caminho[result.tamanho_caminho].x = path_node->atual.x;
            result.caminho[result.tamanho_caminho].y = path_node->atual.y;
            result.tamanho_caminho++;
            path_node = path_node->prev;

            dx = path_node->atual.x - result.caminho[result.tamanho_caminho-1].x;
            dy = path_node->atual.y - result.caminho[result.tamanho_caminho-1].y;
            result.custo_caminho += sqrt(pow(dx, 2) + pow(dy, 2));
        }
        result.caminho[result.tamanho_caminho].x = path_node->atual.x;
        result.caminho[result.tamanho_caminho].y = path_node->atual.y;
        result.tamanho_caminho++;

        _path_reverse(result.caminho, result.tamanho_caminho);
    }
    else
        _node_destroy(pos_fin);

    queue_destroy(expandidos, _node_destroy);
    queue_destroy(fronteira, _node_destroy);

    return result;
}

ResultData depth_first_search(Labirinto *l, Celula inicio, Celula fim)
{
    ResultData result = _default_result();

    Stack *fronteira = stack_construct();
    stack_push(fronteira, _node_construct(inicio.x, inicio.y, NULL));
    Stack *expandidos = stack_construct();
    Node *pos_atual = NULL, *pos_fin = _node_construct(fim.x, fim.y, NULL), *path_node = NULL;

    while (stack_empty(fronteira) == 0)
    {
        pos_atual = stack_pop(fronteira);

        if (_cmp_node(pos_atual, pos_fin))
        {
            result.sucesso = 1;
            stack_push(expandidos, pos_atual);
            _node_destroy(pos_fin);
            pos_fin = pos_atual;
            result.nos_expandidos++;
            break;
        }

        _node_expand_dfs(pos_atual, l, fronteira, expandidos);
        result.nos_expandidos++;
    }

    result.caminho = (Celula *)malloc(sizeof(Celula)*stack_size(expandidos));
    int dx, dy;

    if (result.sucesso)
    {
        path_node = pos_fin;
        while (path_node->prev != NULL)
        {
            result.caminho[result.tamanho_caminho].x = path_node->atual.x;
            result.caminho[result.tamanho_caminho].y = path_node->atual.y;
            result.tamanho_caminho++;
            path_node = path_node->prev;

            dx = path_node->atual.x - result.caminho[result.tamanho_caminho-1].x;
            dy = path_node->atual.y - result.caminho[result.tamanho_caminho-1].y;
            result.custo_caminho += sqrt(pow(dx, 2) + pow(dy, 2));
        }
        result.caminho[result.tamanho_caminho].x = path_node->atual.x;
        result.caminho[result.tamanho_caminho].y = path_node->atual.y;
        result.tamanho_caminho++;

        _path_reverse(result.caminho, result.tamanho_caminho);
    }
    else
        _node_destroy(pos_fin);

    stack_destroy(expandidos, _node_destroy);
    stack_destroy(fronteira, _node_destroy);

    return result;
}

ResultData dummy_search(Labirinto *l, Celula inicio, Celula fim)
{
    int max_path_length = 0;
    float dx, dy;

    ResultData result = _default_result();

    max_path_length = abs(fim.x - inicio.x) + abs(fim.y - inicio.y);
    result.caminho = (Celula *)malloc(sizeof(Celula) * max_path_length);
    result.sucesso = 1;

    Celula atual = inicio;
    result.caminho[result.tamanho_caminho++] = atual;
    result.nos_expandidos++;

    while ((atual.x != fim.x) || (atual.y != fim.y))
    {
        dx = fim.x - atual.x;
        dy = fim.y - atual.y;

        if (dx != 0)
            dx /= fabs(dx);

        if (dy != 0)
            dy /= fabs(dy);

        atual.x += (int)dx;
        atual.y += (int)dy;

        if (labirinto_obter(l, atual.y, atual.x) == OCUPADO || (atual.x > labirinto_n_colunas(l) - 1) || (atual.y > labirinto_n_linhas(l) - 1) || (atual.x < 0) || (atual.y < 0))
        {
            result.sucesso = 0;
            free(result.caminho);
            result.caminho = NULL;
            return result;
        }

        result.caminho[result.tamanho_caminho++] = atual;
        result.nos_expandidos++;
        result.custo_caminho += sqrt(pow(dx, 2) + pow(dy, 2));
    }

    return result;
}
