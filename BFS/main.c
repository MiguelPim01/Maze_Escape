#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/search/labirinto.h"
#include "src/search/algorithms.h"

void print_result(ResultData *result)
{
    if (!result->sucesso)
    {
        printf("IMPOSSIVEL\n");
        return;
    }

    for (int i = 0; i < result->tamanho_caminho; i++)
        printf("%d %d\n", result->caminho[i].y, result->caminho[i].x);

    printf("%.2lf\n", result->custo_caminho);
    printf("%d\n", result->tamanho_caminho);
    printf("%d\n", result->nos_expandidos);
}

void mostra_caminho(Labirinto *l, ResultData *result, Celula inicio, Celula fim)
{
    if (result->sucesso)
    {
        for (int i = 0; i < result->tamanho_caminho; i++)
            labirinto_atribuir(l, result->caminho[i].y, result->caminho[i].x, CAMINHO);
    }

    labirinto_atribuir(l, inicio.y, inicio.x, INICIO);
    labirinto_atribuir(l, fim.y, fim.x, FIM);
    labirinto_print(l);
}

int main()
{
    char arquivo_labirinto[100];
    char algoritmo[100];
    Celula inicio, fim;
    ResultData result;
    Labirinto *lab;

    scanf("%s", arquivo_labirinto);
    scanf("%d %d", &inicio.y, &inicio.x);
    scanf("%d %d", &fim.y, &fim.x);
    scanf("\n%s", algoritmo);

    lab = labirinto_carregar(arquivo_labirinto);

    if (!strcmp(algoritmo, "BFS"))
        result = breadth_first_search(lab, inicio, fim);
    else if (!strcmp(algoritmo, "DFS"))
        result = depth_first_search(lab, inicio, fim);
    else if (!strcmp(algoritmo, "A*"))
        result = a_star(lab, inicio, fim);
    else if (!strcmp(algoritmo, "DUMMY"))
        result = dummy_search(lab, inicio, fim);
    else
    {
        printf("Algoritmo desconhecido: %s\n", algoritmo);
        exit(1);
    }

    print_result(&result);

    // descomente para visualizar informacoes de debug no labirinto
    // mostra_caminho(lab, &result, inicio, fim);

    labirinto_destruir(lab);
    if (result.caminho != NULL)
        free(result.caminho);

    return 0;
}