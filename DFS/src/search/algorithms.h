#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include "labirinto.h"

typedef struct
{
    int x;
    int y;
} Celula;

typedef struct
{
    Celula *caminho;
    double custo_caminho;
    int tamanho_caminho;
    int nos_expandidos;
    int sucesso;
} ResultData;

ResultData a_star(Labirinto *l, Celula inicio, Celula fim);
ResultData breadth_first_search(Labirinto *l, Celula inicio, Celula fim);
ResultData depth_first_search(Labirinto *l, Celula inicio, Celula fim);

// algoritmo bobo de teste que tenta ir em linha reta do inicio ao fim
// e retorna impossível se encontrar um obstáculo
ResultData dummy_search(Labirinto *l, Celula inicio, Celula fim);

#endif