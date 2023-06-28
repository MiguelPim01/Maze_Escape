
#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

typedef struct HashTable HashTable;

// criacao da tabela hash
HashTable *hash_table_construct();

// funcao para insercao/atualizacao de pares chave-valor em O(1).
// Eh responsabilidade do usuario gerar a chave como um inteiro.
// internamente a tabela hash realiza o hash modular para colocar o elemento na posicao correta
// lista encadeada sao usadas para tratar colisoes
void hash_table_set(HashTable *h, int key, void *val);

// retorna o valor associado com a chave key ou NULL se ela nao existir
void *hash_table_get(HashTable *h, int key);

// remove o par chave-valor e retorna o valor ou NULL se nao existir tal chave
void *hash_table_pop(HashTable *h, int key);

// numero de elementos
int hash_table_size(HashTable *h);

// libera o espaco alocado para a tabela hash
void hash_table_destroy(HashTable *h);

#endif