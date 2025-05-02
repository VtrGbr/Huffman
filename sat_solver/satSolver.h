#ifndef satSolver_h
#define satSolver_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//definição dos maximos (mexer se o pc tankar)
#define MAX_LIT 500
#define MAX_CLAU 500

//definir termos
#define SAT 1
#define UNSAT 0
#define UNDEFINED 2

//struct de uma clausula (sozinha)
typedef struct Clausula{
    int literais[MAX_LIT]; //literais da clausula (parcelas com ou do problema)
    int tamanho; //tamanho da clausula
} Clausula;

//estrutura do conjunto de clausulas (todas juntas formam a CNF - forma normal conjuntiva)
//em outras palavras, o conjunto de todos os problemas
typedef struct CNF{
    Clausula clausulas[MAX_CLAU]; //conjunto das clausulas do problema
    int num_clausulas; //numero de clausulas que formam o problema completo (dado no scanneamento)
    int num_literais; //numero de literais que formam o problema (dado no scanneamento)
} CNF;

//estrutura da árvore
typedef struct Arvr{
    int variavel; //variável que é cuidada nesse nó da ávore
    int valor; //valor atribuido 1 (verdadeiro) ou -1 (falso)
    int atribuicoes[MAX_LIT]; //vai preenchendo com o passar da árvore, se por algum acaso der contradição, faz o que chamamos de BACKTRACKING
    struct Arvr* esquerda; //ponteiro pro prox nó da árvore
    struct Arvr* direita; //ponteiro pro prox nó da árvore
} Arvr;


//Funcao responsavel por ler o arquivo cnf e montar a estrura CNF
void ler_arquivo_cnf(const char* nome_arquivo, CNF* problema);

int resolver_cnf(CNF* problema, int atribuicoes[]);

int resolver_sat(Arvr *no, CNF *problema, int solucao[]);

#endif