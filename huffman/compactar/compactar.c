#include "compactar.h"

//Tabela de frequencia

//Criar fila
Fila* criar_fila(){
    //Alocacao do noh 
    Fila* queue = (Fila*)malloc(sizeof(Fila));

    //Verificar se o noh foi alocado corretamente
    if(queue != NULL){
        queue->inicio = NULL;

        return queue;
    }else{
        printf("Falha na alocacao de memoria, na funcao criar fila ( linha 6)\n");
        return NULL;
    }
}

//Criar noh da arvore
No* criar_noh(void* byte, void* frequencia){
    //Alocacao do noh
    No* noh = (No*)malloc(sizeof(No));

    //Verificacao de seguranca
    if( noh != NULL){
        noh->byte = byte;
        noh->frequencia = frequencia;
        noh->esq = NULL;
        noh->dir = NULL;
        noh->prox = NULL;

        return noh;
    }else{
        printf("Falha na alocacao do noh, na funcao criar_noh\n");
        return NULL;
    }
}
//Criar fila de prioridade (O de menor frequencia vem no inicio)
void fila_prioridade(Fila* queue, No* no) {
    if (queue == NULL || no == NULL) {
        return; // Houve algum erro na alocação da fila ou do nó
    }

    // Caso 1: A fila está vazia, insere o nó como o primeiro elemento
    if (queue->inicio == NULL) {
        queue->inicio = no;
        no->prox = NULL; // Garante que o próximo do nó inserido aponte para NULL
    }
    // Caso 2: O nó deve ser inserido antes do primeiro elemento (frequência menor)
    else if (*(ulli*)no->frequencia <= *(ulli*)queue->inicio->frequencia) {
        no->prox = queue->inicio;  // O novo nó aponta para o atual início
        queue->inicio = no;        // Atualiza o início da fila
    }
    // Caso 3: Percorre a fila para encontrar a posição correta
    else {
        No* aux = queue->inicio;

        // Procura o local correto para inserir o nó
        while (aux->prox != NULL && *(ulli*)aux->prox->frequencia < *(ulli*)no->frequencia) {
            aux = aux->prox; // Avança para o próximo nó
        }

        // Insere o nó no local correto
        no->prox = aux->prox; // O próximo do nó inserido será o próximo do auxiliar
        aux->prox = no;       // O auxiliar aponta para o nó recém-inserido
    }

}