#ifndef descompactar_h
#define descompactar_h

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

typedef struct no_arvore no_t;

struct no_arvore 
{
    unsigned int caractere;
    no_t *esquerda; 
    no_t *direita;
};

/* 
Esta função cria uma árvore com o caractere
e as árvores esquerda e direita, respectivamente,
e retorna o ponteiro para a árvore criada.
*/
no_t *criar_no(unsigned int caractere, no_t *esquerda, no_t *direita);

/*
Esta função libera a memória alocada para evitar
vazamento de memória.
*/
void liberar_arvore(no_t *arvore_huff);

/*
Esta função identifica se um nó é uma folha e
retorna 1 para positivo e 0 para negativo.
*/
int eh_folha_descompactar(no_t *arvore_binaria);

/*
Esta função identifica se um bit de uma posição
dada de um byte está definido e retorna 1 para
positivo ou 0 para negativo.
*/
int bit_esta_definido_descompactar(unsigned int caractere, int posicao); 

/*
Esta função lê o arquivo byte a byte a partir
do segundo byte. Então, escreve os caracteres
correspondentes de acordo com a árvore de
Huffman recebida no arquivo de saída.
*/
void descompactar(FILE *arquivo_entrada, unsigned int tamanho_lixo, int tamanho_arvore, no_t *arvore_huff, FILE *arquivo_saida);

/*
Esta função obtém a informação do tamanho do lixo
do primeiro byte do arquivo e a retorna.
*/
unsigned int obter_tamanho_lixo(FILE *arquivo_entrada); 

/*
Esta função obtém a informação do tamanho da árvore
dos dois primeiros bytes do arquivo e a retorna.
*/
unsigned int obter_tamanho_arvore(FILE *arquivo_entrada);

/*
Esta função obtém o array da árvore de acordo com
o tamanho da árvore e o retorna.
*/
unsigned int *obter_array_arvore(FILE *arquivo_entrada, unsigned int tamanho_arvore);

/*
Esta função constrói uma árvore de Huffman
recursivamente.
*/
no_t *construir_arvore(unsigned int **array_arvore);

/*
Esta função retorna a árvore de Huffman construída
a partir dos dados do arquivo.
*/
no_t *obter_arvore(FILE *arquivo_entrada, unsigned int tamanho_arvore);

/*
Esta função remove a extensão Huffman do arquivo
e retorna o ponteiro do arquivo de saída.
*/
FILE* remover_huff(char nome[]);

/*
Função principal para executar o algoritmo de descompactação.
*/
void descompactarPrincipal();

#endif