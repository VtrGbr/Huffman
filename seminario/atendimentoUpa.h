#ifndef atendimentoUpa_h
#define atendimentoUpa_h 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct paciente {
    char nome[100];
    int urgencia;
    int cartao_sus;
    struct paciente* prox;
} Paciente;

typedef struct fila {
    Paciente* inicio;
} Fila;

//Funcao responsavel por criar um noh para fila e retorna-lo
Fila* criar_fila();

//Essa funcao transforma a string da pulseira em numero para podermos adicionar na fila
int numero_urgencia(char* urgencia);

//Nessa função ela transforma o numero da urgencia em string para podermos imprimir no "telao"
char* nome_urgencia(int urgencia);

//Criamos um noh para o paciente, para evitar estar colocando valores demais em uma funcao
//Recebemos os dados do paciente, nome, o numero de urgencia e o numero do cartao do sus
Paciente* criar_paciente(char *nome, int urgencia, int cartao_sus);

//Adicionamos de maneira prioritaria na fila, de maneira crescente
void adicionar(Fila* fila, Paciente* paciente);

//Removemos um paciente para que ele seja atendido
Paciente* remover(Fila* fila);

//Imprimir um paciente
void imprimir(Fila* fila);

//Uma tela simples de menu
void menu();

//Funcao recebe uma fila e libera o espaco dela
void liberar_fila(Fila* fila);

#endif