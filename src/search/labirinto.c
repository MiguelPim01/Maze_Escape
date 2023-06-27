
#include <stdio.h>
#include <stdlib.h>

#include "labirinto.h"

struct Labirinto
{
    int n_linhas;
    int n_colunas;
    unsigned char **celulas;
};

Labirinto *labirinto_carregar(char *arquivo)
{
    FILE *file = fopen(arquivo, "rb");

    if (file == NULL)
        exit(printf("Arquivo %s nao encontrado.\n", arquivo));

    int n_linhas, n_colunas;
    fread(&n_linhas, sizeof(int), 1, file);
    fread(&n_colunas, sizeof(int), 1, file);

    Labirinto *lab = (Labirinto *)malloc(sizeof(Labirinto));
    lab->celulas = (unsigned char **)malloc(n_linhas * sizeof(unsigned char *));
    lab->n_linhas = n_linhas;
    lab->n_colunas = n_colunas;

    for (int i = 0; i < n_linhas; i++)
    {
        lab->celulas[i] = (unsigned char *)malloc(n_colunas * sizeof(unsigned char));
        fread(lab->celulas[i], sizeof(unsigned char), n_colunas, file);
    }

    fclose(file);
    return lab;
}

int labirinto_n_linhas(Labirinto *l)
{
    return l->n_linhas;
}

int labirinto_n_colunas(Labirinto *l)
{
    return l->n_colunas;
}

void labirinto_atribuir(Labirinto *l, int linha, int coluna, TipoCelula valor)
{
    if (linha < 0 || linha >= l->n_linhas || coluna < 0 || coluna >= l->n_colunas)
        exit(printf("Posição (%d, %d) inválida no labirinto com tamanho (%d, %d).\n", linha, coluna, l->n_linhas, l->n_colunas));

    l->celulas[linha][coluna] = valor;
}

unsigned char labirinto_obter(Labirinto *l, int linha, int coluna)
{
    if (linha < 0 || linha >= l->n_linhas || coluna < 0 || coluna >= l->n_colunas)
        exit(printf("Posição (%d, %d) inválida no labirinto com tamanho (%d, %d).\n", linha, coluna, l->n_linhas, l->n_colunas));

    return l->celulas[linha][coluna];
}

void labirinto_destruir(Labirinto *l)
{
    for (int i = 0; i < l->n_linhas; i++)
        free(l->celulas[i]);

    free(l->celulas);
    free(l);
}

void _labirinto_print_celula(unsigned char val)
{
    switch (val)
    {
    case LIVRE:
        printf(". ");
        break;
    case OCUPADO:
        printf("# ");
        break;
    case FRONTEIRA:
        printf("F ");
        break;
    case EXPANDIDO:
        printf("X ");
        break;
    case CAMINHO:
        printf("O ");
        break;
    case INICIO:
        printf("I ");
        break;
    case FIM:
        printf("[]");
        break;
    default:
        printf("Tipo invalido de celula.\n");
    }
}

void labirinto_print(Labirinto *l)
{
    for (int i = 0; i < l->n_linhas; i++)
    {
        printf("| ");
        for (int j = 0; j < l->n_colunas; j++)
            _labirinto_print_celula(labirinto_obter(l, i, j));
        printf("|\n");
    }
}