#ifndef compactar_h
#define compactar_h
 
#define TAM 256
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
 
//Estruturas
typedef unsigned char uchar;
typedef unsigned long long int ulli;
typedef unsigned short ushor;
 
typedef struct no{
    void* byte;
    void* frequencia;
    struct no* dir;
    struct no* esq;
    struct no* prox;
}No;
 
typedef struct fila{
    No* inicio;
}Fila;
 
 
//Funcao para criar a fila
Fila* criar_fila();
 
//Cria o noh da arvore
//void* byte: aqui serah colocar o byte do noh
//void* frequencia: aqui terah a frequencia do byte
No* criar_noh(void* byte, void* frequencia);
 
 
//Inserir na fila de prioridade ( o inicio da fila sempre vai ficar com o valor de menor frequencia)
/*
Caso 1: A fila estah vazia. Caso isso ocorra o noh a ser inserido ficarah no inicio
Caso 2: A fila nao estah mais vazia,vamos ter que verificar se o elemento que queremos inserir tem frequencia menor que o 1º noh da lista
        Neste caso o noh vai ficar na frente do noh do inicio
Caso 3: O noh a ser inserido vai ficar no meio ou no fim
*/
void fila_prioridade(Fila* queue, No* no);
 
//recebe o nome do arquivo e um vetor para armazenar a frequência de todos os seus bytes. 
void tabela_frequencia( const char* nome_arquivo,ulli frequencia[TAM]);
 
//Preenche a fila de prioridade com os bytes do arquivo original e suas respectivas frequencias
void preencher_fila( Fila* queue, ulli *frequencia);
 
// retorna o endereço do noh de menor frequencia da fila
No* dequeue(Fila* queue);
 
 
/* 
    Montando a arvore de huffman
    A cada iteração, remove os dois primeiros nós da fila (os de menor frequência), cria um novo nó com a soma das frequências e insere o novo nó na fila.
    O processo continua até que reste apenas um nó na fila, que será a raiz da árvore de Huffman.
    A árvore resultante é uma estrutura binária onde cada nó contém um byte e sua frequência, e os filhos esquerdo e direito representam os caminhos para os bits 0 e 1, respectivamente.
*/
No* gerar_arvore(Fila* queue);
 
//Preenche a tabela de códigos com o caminho da árvore de Huffman para cada byte.
//char* tb_codigo[]: vetor de strings que armazena os códigos de Huffman para cada byte
//No* arvore: ponteiro para a raiz da árvore de Huffman
//char* caminho: string que representa o caminho atual na árvore de Huffman (0 para esquerda, 1 para direita)
void tabela_de_codigo(char *tb_codigo[], No *arvore, char *caminho);
 
/*
    Essa função percorre todo o arquivo, verifica o código de Huffman correspondente a cada byte usando uma 
    tabela, e soma o número total de bits que seriam usados para comprimir o conteúdo, retornando esse valor.
*/
ulli contar_bits_totais(const char* nome_arquivo, char** tabela_codigos);
/*
    Essa função escreve a árvore de Huffman no arquivo compactado. 
    A árvore é representada de forma serializada, onde cada nó é escrito como um caractere especial ('*') para nós internos e o byte correspondente para folhas.
    A função percorre a árvore em pré-ordem, escrevendo os bytes e os nós internos no arquivo.
*/
void escrever_arvore(FILE *arquivo, No *raiz);
 
/*
    Essa função escreve o cabeçalho do arquivo compactado, que contém informações sobre o número de bits de lixo e o tamanho da árvore de Huffman e a árvore em si.
    O tamanho da árvore é o número de nós na árvore de Huffman, que é o número total de bytes únicos no arquivo original.
    O número de bits de lixo é o número de bits que não podem ser usados para armazenar dados úteis no arquivo compactado.
    A arvore é escrita com a função "escrever_arvore".
*/
void escrever_cabecalho(FILE *arquivo, int bitsLixo, int tamanhoArvore, No* raiz);
 
/*
    Essa função lê o arquivo original byte a byte, obtém o código binário correspondente a cada byte usando a tabela de códigos, 
    e escreve os dados compactados no arquivo compactado. Os bits são armazenados em um buffer e escritos no arquivo quando o buffer está cheio.
    A função também lida com o caso em que ainda há bits restantes no buffer após ler todo o arquivo.
*/
void dados_compactados(const char* nome_arquivo, FILE* arquivo_compactado, char** tabela_codigos);
 
/*
    Essa função calcula o tamanho da árvore de Huffman em bits. 
    A função percorre a árvore recursivamente, contando o número de nós e considerando casos especiais para folhas que contêm caracteres especiais ('*' ou '\\').
    O tamanho total da árvore é retornado como um inteiro.
*/
int calcular_tamanho_arvore(No *raiz);
 
//colocamos a extesao .huff no arquivo que queremos compactar
char* adicionar_extensao(const char* nome_base, const char* extensao) ;
 
/*
    Função principal para compactar o arquivo.
    Ela executa os seguintes passos:
    Passo 1: Gerar tabela de frequências
    Passo 2: Criar fila de nós e gerar a árvore de Huffman
    Passo 3: Gerar tabela de códigos referente a cada byte
    Passo 4: Calcula o tamanho do arquivo apos a compactação e os bits lixo
    Passo 5: Criar o arquivo compactado e escrever o cabeçalho
    Passo 6: Escrever os dados compactados no arquivo
    Passo 7: Liberar a memória alocada para a tabela de códigos
    Passo 8: Fechar o arquivo compactado
*/
void compactar_arquivo (const char* nome_arquivo_original);
 
//void imprimir_frequencia( ulli frequencia[TAM]);
void imprimir_arvore(No* raiz);
 
//imprimir a fila 
void imprimir_fila(Fila* queue);
 
#endif
