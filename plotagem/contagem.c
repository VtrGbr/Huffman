#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct heap{
    int *valores;
    int tamanho;
    int capacidade;
    int comparacoes;
}Heap;

typedef struct no{
    int valor;
    struct no* prox;
}No;

typedef struct filaPri{
    No* cabeca;
    int comparacoes;
}FilaPri;

/*
Criacoes da fila de prioridade com heap e da fila de prioridade sem heap
*/

Heap* criar_heap(int capacidade){
    //Alocacao de memoria
    Heap* heap = (Heap*)malloc(sizeof(Heap));

    //Verificacao de seguranca
    if( heap != NULL){
        heap->valores = (int*)calloc(capacidade,sizeof(int)); // Alocando memoria para o vetor de valores
        //Verificacao de seguranca
        if( heap->valores != NULL){
            heap->tamanho = 0;
            heap->capacidade = capacidade;
            heap->comparacoes = 0;

            return heap;
        }else{
            printf("Falha no vetor de valores\n");
            return NULL;
        }
    }else{
        printf("Falha na alocacao da heap\n");
        return NULL;
    }
}


FilaPri* criar_fila(){
    FilaPri* fila = ( FilaPri*)malloc(sizeof(FilaPri));

    //Verificacao de seguranca
    if( fila != NULL){
        fila->cabeca = NULL;
        fila->comparacoes = 0;

        return fila;
    }else{
        printf("Falha na alocacao de memoria da fila de prioridade sem heap\n");
        return NULL;
    }
}

/*
Funcoes para insercao e remocao dos valores numa heap e numa fila sem heap

*/

//Funcao para pegar o pai
int pegar_pai(Heap *heap,int i){
    return i /2;
}

void swap(int* valor1, int* valor2) {
    int aux = *valor1;
    *valor1 = *valor2;
    *valor2 = aux;
}

/*
A função inserir_heap recebe um ponteiro para uma estrutura Heap e um valor int a ser inserido. A estrutura Heap contém um vetor de inteiros (valores), o tamanho atual (tamanho), a capacidade máxima (capacidade) e um contador de comparações (comparacoes).
*/

void inserir_heap(Heap* heap, int valor){
    //Chegou na capacidade total
    //Esta verificação nos assegura que a heap não ultrapasse sua capacidade máxima. Se o número de elementos (tamanho) for igual à capacidade (capacidade), a função retorna sem fazer nada.
    if( heap->tamanho == heap->capacidade){
        return; // Pare o processo
    }else{

        int i = heap->tamanho++;
        heap->valores[i] = valor; //o valor a ser inserido é colocado no último índice da heap "(i)". Nesse ponto, o valor ainda não está na posição correta para manter a estrutura de heap.

        //o loop continua enquanto i for maior que 0, ou seja, até que o valor esteja no topo da heap "índice 0" ou em uma posição válida.
        while (i > 0)
        {
            //a função "pegar_pai" calcula o índice do nó pai de i na heap. Essa função geralmente usa i / 2 para encontrar o pai do índice i.
            int pai = pegar_pai(heap,i); 
            heap->comparacoes++; // contador de comparacoes para o grafico

            //Se o valor atual "heap->valores[i]" for maior ou igual ao valor do nó pai "heap->valores[pai]", o loop para. Isso acontece porque a heap está organizada corretamente quando os valores dos filhos são maiores ou iguais aos do pai em uma heap.
            if( heap->valores[i] >= heap->valores[pai]){
                break;
            }

            //caso o valor atual seja menor que o valor do pai, os dois valores são trocados para manter a propriedade da heap.
            swap(&heap->valores[i],&heap->valores[pai]);

            //i é atualizado para o índice do pai, e o processo de ajuste é repetido até que a estrutura da heap seja restaurada.
            i = pai;
           
        }
    }

}


//a função inserir_fila, insere um valor em uma fila de prioridade de forma que a lista esteja sempre ordenada em ordem crescente.
void inserir_fila(FilaPri* fila, int valor){

    //Alocacao de memoria para um noh, onde serao inseridos o "valor" e para onde o "prox" apontarah
    No* queue = (No*)malloc(sizeof(No));

    //Verificacao de seguranca
    if( queue != NULL){
        queue->valor = valor;
        queue->prox = NULL;
    }else{
        printf("Falha na alocacao da fila, na insercao");
        return;
    }

    /*
        Insercao no inicio da fila:
        1º Caso a fila esteja vazia: o novo noh serah inserido no inicio da fila;
        2º O valor inserido eh menor que o valor ja existente, entao o novo noh deve ser inserido antes dele. (Ja que essa fila eh em ordem crescente)
    */
    if( fila->cabeca == NULL || fila->cabeca->valor >= valor){
        queue->prox = fila->cabeca; // o ponteiro prox do novo noh aponta para o noh que atualmente eh o primeiro na fila.
        fila->cabeca  = queue; // atualiza o inicio da lista "fila->cabeca" para que queue seja o novo primeiro noh.
        return; // A funcao para, pois o noh ja foi inserido
    }

    //Aqui a fila nao estah mais vazia ou o valor a ser inserido nao eh menor que o valor ja existente. Entao vamos ter que percorrer a fila para saber onde devemos inserir

    No* aux = fila->cabeca; //Criamos um auxiliar (que comeca no primeiro noh), para nao perdermos a referencia da fila

    /*O loop so continuarah se:
        1º Nao chegamos no fim da fila (aux->prox != NULL)
        2º Enquanto o valor do próximo noh for menor que o valor a ser inserido. Isso garante que aux irah parar no ultimo noh antes do ponto onde o novo noh deve ser inserido.
    */
    while( aux->prox != NULL && aux->prox->valor < valor){
        fila->comparacoes++; //Adicionamos na comparacao para que possamos usar na plotagem do grafico
        aux = aux->prox;
    }

    //Aqui ja estamos no lugar certo para inserir o noh
    fila->comparacoes++;
    queue->prox = aux->prox;  //o ponteiro prox do novo noh "queue" aponta para o noh que estah logo apos a posicao onde o novo noh serah inserido.
    
    aux->prox = queue; //o noh anterior a posição correta "aux" agora aponta para o novo noh "queue", completando a insercao do noh na posicao correta

    return; //Finaliza a operacao
}




int main() {
    Heap* heap = criar_heap(1000);
    FilaPri* fila = criar_fila();
    int n = 1000;
    FILE* resultados;

    resultados = fopen("valores.txt", "w");

    if (resultados == NULL) {
        perror("Falha ao abrir o arquivo");
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int valor = rand() % 10000;

        inserir_heap(heap, valor);
        inserir_fila(fila, valor);

        fprintf(resultados, "%d %d %d\n", i + 1, heap->comparacoes, fila->comparacoes);
    }


    fclose(resultados);

    free(heap->valores);
    free(heap);
    free(fila);

    return 0;
}