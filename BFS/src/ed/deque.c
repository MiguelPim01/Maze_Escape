#include <stdio.h>
#include <stdlib.h>

#include "deque.h"

#define TAM_DEQUE 10
#define TAM_ARRAY 10

struct Deque {
    void ***blocos;
    int blocoIni;
    int blocoFin;
    int inicio;
    int fim;
    int alocado;
};

Deque *deque_construct()
{
    Deque *d = (Deque *)malloc(sizeof(Deque));

    d->blocos = (void ***)calloc(TAM_DEQUE, sizeof(void **));
    
    int x = TAM_DEQUE/2;

    d->blocos[x] = (void **)calloc(TAM_ARRAY, sizeof(void *));

    d->blocoIni = d->blocoFin = x;
    d->inicio = d->fim = 0;
    d->alocado = TAM_DEQUE;

    return d;
}

void deque_push_front(Deque *d, void *p)
{
    if (d->inicio == 0)
    {
        if (d->blocoIni == 0)
        {
            int meio_alocado = (d->blocoFin + d->blocoIni + 1)/2, meio_deque = d->alocado/2;

            if (d->blocoFin == d->alocado-1)
            {
                void *** novos_blocos = (void ***)calloc(d->alocado*2, sizeof(void **));
                int meio_novos_blocos = d->alocado;

                for (int i = (meio_novos_blocos - meio_deque), j = 0; j <= d->blocoFin; j++, i++)
                    novos_blocos[i] = d->blocos[j];
                
                d->blocoIni = meio_novos_blocos - meio_deque;
                d->blocoFin = meio_novos_blocos + meio_deque - 1;

                free(d->blocos);
                d->blocos = novos_blocos;
                d->alocado *= 2;
            }
            else
            {
                int deslocamento = meio_deque - meio_alocado;

                for (int i = d->blocoFin; i >= d->blocoIni; i--)
                {
                    d->blocos[i+deslocamento] = d->blocos[i];

                    if (i < d->blocoIni + deslocamento)
                        d->blocos[i] = NULL;
                }
                d->blocoIni += deslocamento;
                d->blocoFin += deslocamento;
            }
        }

        d->blocoIni--;
        d->blocos[d->blocoIni] = (void **)calloc(TAM_ARRAY, sizeof(void *));
        d->inicio = TAM_ARRAY-1;
        d->blocos[d->blocoIni][d->inicio] = p;
    }
    else
    {
        d->inicio--;
        d->blocos[d->blocoIni][d->inicio] = p;
    }
}

void deque_push_back(Deque *d, void *p)
{
    if (d->fim == TAM_ARRAY)
    {
        if (d->blocoFin == d->alocado-1)
        {
            int meio_alocado = (d->blocoFin + d->blocoIni + 1)/2, meio_deque = d->alocado/2;

            if (d->blocoIni == 0)
            {
                void *** novos_blocos = (void ***)calloc(d->alocado*2, sizeof(void **));
                int meio_novos_blocos = d->alocado;

                for (int i = (meio_novos_blocos - meio_deque), j = 0; j <= d->blocoFin; j++, i++)
                    novos_blocos[i] = d->blocos[j];
                
                d->blocoIni = meio_novos_blocos - meio_deque;
                d->blocoFin = meio_novos_blocos + meio_deque - 1;

                free(d->blocos);
                d->blocos = novos_blocos;
                d->alocado *= 2;
            }
            else
            {
                int deslocamento = meio_alocado - meio_deque + 1;

                for (int i = d->blocoIni; i <= d->blocoFin; i++)
                {
                    d->blocos[i-deslocamento] = d->blocos[i];

                    if (i > d->blocoFin - deslocamento)
                        d->blocos[i] = NULL;
                }
                d->blocoIni -= deslocamento;
                d->blocoFin -= deslocamento;
            }
        }

        d->blocoFin++;
        d->fim = 0;
        d->blocos[d->blocoFin] = (void **)calloc(TAM_ARRAY, sizeof(void *));
        d->blocos[d->blocoFin][d->fim] = p;
        d->fim++;
    }
    else
    {
        d->blocos[d->blocoFin][d->fim] = p;

        d->fim++;
    }
}

void *deque_pop_front(Deque *d)
{
    if (d->inicio == d->fim && d->blocoFin == d->blocoIni)
        return NULL;

    void *rtn = d->blocos[d->blocoIni][d->inicio];

    if (d->inicio == TAM_ARRAY-1)
    {
        free(d->blocos[d->blocoIni]);
        d->inicio = 0;
        d->blocoIni++;
    }
    else
        d->inicio++;
    
    return rtn;
}

void *deque_pop_back(Deque *d)
{
    if (d->inicio == d->fim && d->blocoFin == d->blocoIni)
        return NULL; 

    if (d->fim == 0)
    {
        free(d->blocos[d->blocoFin]);
        d->blocoFin--;
        d->fim = TAM_ARRAY-1;
    }
    else
        d->fim--;
    
    void *rtn = d->blocos[d->blocoFin][d->fim];

    return rtn;
}

void *deque_get(Deque *d, int i)
{
    int bloco = d->blocoIni + (int)((i + d->inicio) / TAM_ARRAY);
    int indice = (i + d->inicio) % TAM_ARRAY;

    if (bloco > d->blocoFin || (bloco == d->blocoFin && indice > d->fim) || i < 0)
        return NULL;

    return d->blocos[bloco][indice];
}

int deque_size(Deque *d)
{
    return (d->blocoFin - d->blocoIni)*TAM_ARRAY + (d->fim - d->inicio);
}

void deque_print(Deque *d, void (*fptr_print)(void *))
{
    for (int i = d->blocoIni; i <= d->blocoFin; i++)
    {
        for (int pos = 0; pos < TAM_ARRAY; pos++)
        {
            if (i == d->blocoFin && pos == d->fim)
                break;

            fptr_print(d->blocos[i][pos]);
        }
        printf("\n");
    }
}

void deque_destroy(Deque *d, void (*fptr_destroy)(void *))
{
    int j = 0, cond = TAM_ARRAY;
    for (int i = d->blocoIni; i <= d->blocoFin; i++)
    {
        if (i == d->blocoIni && i == d->blocoFin)
        {
            j = d->inicio;
            cond = d->fim;
        }
        else if (i == d->blocoIni)
        {
            j = d->inicio;
            cond = TAM_ARRAY;
        }
        else if (i == d->blocoFin)
        {
            j = 0;
            cond = d->fim;
        }
        else
        {
            j = 0;
            cond = TAM_ARRAY;
        }
        
        for (; j < cond; j++)
            fptr_destroy(d->blocos[i][j]);
        
        free(d->blocos[i]);
    }
    
    free(d->blocos);
    free(d);
}