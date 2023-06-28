#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "algorithms.h"
#include "../ed/stack.h"
#include "../ed/queue.h"

typedef struct Node {
    Celula atual;
    struct Node *prev;
} Node;

Node *_node_construct(int x, int y, Node *prev)
{
    Node *n = (Node *)malloc(sizeof(Node));

    n->atual.x = x;
    n->atual.y = y;
    n->prev = prev;

    return n;
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
    // TODO!
    return _default_result();
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
