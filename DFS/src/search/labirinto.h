
#ifndef _LABIRINTO_H_
#define _LABIRINTO_H_

typedef enum
{
    LIVRE = 0,
    OCUPADO = 1,
    FRONTEIRA = 2,
    EXPANDIDO = 3,
    CAMINHO = 4,
    INICIO = 5,
    FIM = 6
} TipoCelula;

typedef struct Labirinto Labirinto;

Labirinto *labirinto_carregar(char *arquivo);
int labirinto_n_linhas(Labirinto *l);
int labirinto_n_colunas(Labirinto *l);
void labirinto_atribuir(Labirinto *l, int linha, int coluna, TipoCelula valor);
unsigned char labirinto_obter(Labirinto *l, int linha, int coluna);
void labirinto_print(Labirinto *l);
void labirinto_destruir(Labirinto *l);

#endif
