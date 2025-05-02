#include "compactar.h"
 
#define tam 256
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
 
//recebe o nome do arquivo e um vetor para armazenar a frequência de todos os seus bytes. 
void tabela_frequencia( const char* nome_arquivo,ulli frequencia[TAM]){
 
    FILE* arquivo = fopen(nome_arquivo,"rb"); // rb = read binary (leitura binaria, em traducao livre)
 
    //Verificacao de seguranca
    if( arquivo == NULL){
        perror("Falha ao abrir o arquivo");
        return;
    }else{
 
        int byte;
 
        //a função "fgetc" checa byte a byte do arquivo, aumentando a frequência do byte encontrado no vetor "frequencia" correspondente ao byte lido.
        while( (byte = fgetc(arquivo)) != EOF){
            frequencia[(uchar)byte]++; // cada byte que ele achar na frequencia serah adicionado mais 1
        }
        fclose(arquivo);
 
    }
 
}
 
//inseri na fila de prioridade (O de menor frequencia vem no inicio)
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
 
// Preenche a fila com os bytes do arquivo original e suas respectivas frequencias
void preencher_fila( Fila* queue, ulli *frequencia)
{
    //percorre os bytes do vetor de frequencia
    for( int i = 0; i < TAM;i++)
    {
        //Verifica apenas os bytes com frequencia maior que 0.
        if( frequencia[i] > 0)
        { 
            // Alocar dinamicamente o byte e a frequência para usar void*, para evitar casting de maneira errada
            uchar* ponteiro_para_byte = (uchar*)malloc(sizeof(uchar));
            ulli* ponteiro_para_frequencia = (ulli*)malloc(sizeof(ulli));
 
            //Verificacao de seguranca
            if( ponteiro_para_byte != NULL && ponteiro_para_frequencia != NULL){
 
                *ponteiro_para_byte = (uchar)i;
                *ponteiro_para_frequencia = frequencia[i];
 
                //cria um noh para o byte "i" e sua frequencia
                No* noh = criar_noh(ponteiro_para_byte,ponteiro_para_frequencia);
 
                //Verificacao de seguranca
                if( noh != NULL){
                    //insere o no de forma ordenada na fila de prioridade (menor para maior)
                    fila_prioridade(queue,noh);
 
                }else{
                    printf("Falha na alocacao de preenchimento da queuea\n");
                    return; // parar o procedimento
                }
            }else{
                printf("Falha na alocacao dos ponteiros para byte e frenquencia, na funcao preencher\n");
                return;
            }
 
        }
    }
}
 
// retorna o endereço do noh de menor frequencia da fila
No* dequeue(Fila* queue){
 
    /*
    Temos quer ter um ponteiro auxiliar e inicializa-lo com NULL, pois vamos retornar o auxiliar, pois se nao tiver ninguem na queuea removeremos o null
    */
    No* aux = NULL;
 
    if( queue->inicio != NULL){
        aux = queue->inicio; // o auxiliar aponta para o inicio
        //O inicio da queuea eh o proximo ponteiro
        queue->inicio = aux->prox;  // O inicio vai para o proximo para nao perdermos o encadeamento da queuea
 
        //Para nao termos problemas de tentar acessar, de maneira indevida, o campo proximo fora da manipulacao da queuea
        aux->prox = NULL;
    }
 
    return aux;
}
 
//Montando a arvore de huffman
No* gerar_arvore(Fila* queue){
    /*
    Criaremos 3 ponteiros:
        - O primeiro ponteiro "primeiro" irah receber o noh com a frequencia da queuea(sendo a queuea organizada para ser retirado o noh de menor frequencia)
        - O segundo ponteiro "segundo" irah receber o segundo noh de menor frequencia da queuea
        - O terceiro ponteiro "novo" serah o noh que juntaremos a frequencia do "primeiro" e "segundo" ponteiro 
    */
    No* primeiro;
    No* segundo;
    No* novo;
 
    //Enquanto o tamanho da queuea for maior que 1 vamos continuar executando o codigo, pois quando o tamanho da queuea for igual a 1, significa que temos apenas um noh e esse noh serah o noh raiz
    while( queue->inicio->prox != NULL){
        primeiro = dequeue(queue);// no de manor frequencia
        segundo = dequeue(queue);// no de segunda menor frequencia
 
        ulli freq_primeiro = *(ulli*)primeiro->frequencia;// pega a frequencia do primeiro noh
        ulli freq_segundo = *(ulli*)segundo->frequencia;// pega a frequencia do segundo noh
 
        // Alocar memória para a nova frequência
        ulli* nova_frequencia = (ulli*)malloc(sizeof(ulli));
 
        if( nova_frequencia != NULL){
            // Soma das frequências dos dois nós
            *nova_frequencia = freq_primeiro + freq_segundo;
            //Agora precisamos criar o noh "novo" e alocar memoria e concetar o "primeiro" e o "segundo"
            novo = criar_noh(NULL,nova_frequencia);
            //Verificacao de seguranca
            if( novo != NULL){
                novo->esq = primeiro; // O primeiro noh vai ser o filho esquerdo do noh novo
                novo->dir = segundo;// O segundo noh vai ser o filho direito do noh novo
                novo->prox = NULL; // Neste momento nao aponta para ninguem
 
                //Agora vamos inserir o noh "novo" novamente na queuea e repetir o processo ate o tamanho da queuea ser 1
                fila_prioridade(queue,novo);
 
 
            }else{
                printf("Falha na alocacao do noh novo, na funcao gerar_arvore\n");
                return NULL;
            }
 
        }else{
            printf("Falha na alocacao de nova frequencia\n");
            return NULL;
 
        }
 
    }
 
    //Quando o loop acabar, ou seja, "fila->tam" == 1, retornaremos o noh raiz da arvore que sera o inicio da fila
    return dequeue(queue);
}
 
//preenche a tabela de códigos com o caminho da árvore de Huffman para cada byte. 
void tabela_de_codigo(char *tb_codigo[], No *arvore, char *caminho)
{
    if (arvore == NULL) return;
 
    // Caso base: nó folha
    if (arvore->esq == NULL && arvore->dir == NULL) {
        //copia o caminho atual para a tabela de códigos na posição correspondente ao byte
        tb_codigo[*(unsigned char*)arvore->byte] = strdup(caminho);
        return;
    }
 
    // Caminho para esquerda: adiciona '0'
    char caminho_esq[TAM]; //criamos um novo vetor para o caminho esquerdo
 
    // Copia o caminho atual e adiciona '0' para o caminho esquerdo
    strcpy(caminho_esq, caminho);
    strcat(caminho_esq, "0");
    tabela_de_codigo(tb_codigo, arvore->esq, caminho_esq);// chamada recursiva para o filho esquerdo
 
    // Caminho para direita: adiciona '1'
    char caminho_dir[TAM];
 
    // Copia o caminho atual e adiciona '1' para o caminho direito
    strcpy(caminho_dir, caminho);
    strcat(caminho_dir, "1");
    tabela_de_codigo(tb_codigo, arvore->dir, caminho_dir);// chamada recursiva para o filho direito
}
 
//calcula a quantidade bits do arquivo original apos a compactacao.
ulli contar_bits_totais(const char* nome_arquivo, char** tabela_codigos) {
    FILE* arquivo = fopen(nome_arquivo, "rb"); // leitura binária
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }
 
    //inicializa o total de bits
    ulli total_bits = 0;
    int byte;
 
    while ((byte = fgetc(arquivo)) != EOF) {
        // Fazemos um cast para unsigned char para garantir que o valor do byte esteja entre 0 e 255,
        // evitando acessos inválidos fora do vetor tabela_codigos (que tem tamanho 256).
        uchar c = (unsigned char)byte;
 
        // Verifica se o byte lido existe na tabela de códigos
        if (tabela_codigos[byte] != NULL) 
        {
            //Se o byte existe, soma o tamanho do código correspondente ao byte lido ao total de bits
            total_bits += strlen(tabela_codigos[byte]);
        } else {
            // Caso não tenha código para esse byte,escreve no arquivo,o erro
            fprintf(stderr, "Aviso: byte %d não tem código na tabela.\n", byte);
        }
    }
 
    fclose(arquivo);
    return total_bits; // retorna o total de bits
}
 
// Função para escrever a árvore de Huffman no arquivo em pré-ordem
// Utiliza '*' para nós internos e '\' como caractere de escape
void escrever_arvore(FILE *arquivo, No *raiz) {
    // Caso base: se o nó for nulo, não faz nada
    if (raiz == NULL) return;
 
    // Verifica se o nó é uma folha (sem filhos)
    if (raiz->esq == NULL && raiz->dir == NULL) {
        // Converte o ponteiro void* para unsigned char (garante valores entre 0 e 255)
        unsigned char c = *(unsigned char*)raiz->byte;
 
        // Se o caractere da folha for '*' ou '\', precisamos escapá-lo
        // para não confundir com os símbolos especiais usados na serialização
        if (c == '*' || c == '\\') {
            fputc('\\', arquivo);  // Escreve o caractere de escape antes
        }
 
        // Escreve o byte real da folha no arquivo
        fputc(c, arquivo);
    } else {
        // Escreve '*' para indicar que este é um nó interno
        fputc('*', arquivo);
 
        // Chama recursivamente a função para os filhos esquerdo e direito
        escrever_arvore(arquivo, raiz->esq);
        escrever_arvore(arquivo, raiz->dir);
    }
}
 
//escrevemos o cabeçalho do arquivo compactado, 16 primeiros bits mais a arvore de Huffman, com a ajuda da funcao "escrever_arvore"
void escrever_cabecalho(FILE *arquivo, int bitsLixo, int tamanhoArvore, No* raiz) 
{
    // Garantir que bitsLixo caiba em 3 bits e tamanhoArvore em 13 bits
    if (bitsLixo > 7 || tamanhoArvore > 8191) {
        fprintf(stderr, "Erro: bits de lixo ou tamanho da árvore fora do limite.\n");
        exit(EXIT_FAILURE);
    }
 
    // Montar os 16 bits do cabeçalho
    unsigned short cabecalho = 0;
 
    // Posicionar bits de lixo nos 3 bits mais significativos (13ª a 15ª posição)
    cabecalho |= (bitsLixo << 13);
 
    // Colocar o tamanho da árvore nos 13 bits menos significativos
    cabecalho |= tamanhoArvore;
 
    /*
        Escrever os 2 bytes (16 bits) do cabeçalho no arquivo — big endian (MSB primeiro).
        Fazemos isso para garantir que o arquivo seja lido corretamente em diferentes sistemas, 
        visto que a ordem dos bytes pode variar entre arquiteturas (big-endian vs little-endian).
    */
    fputc((cabecalho >> 8) & 0xFF, arquivo);  // byte mais significativo
    fputc(cabecalho & 0xFF, arquivo);         // byte menos significativo
 
    //escreve a árvore no arquivo compactado
    escrever_arvore(arquivo, raiz); 
 
 
}
 
// Função para escrever os dados compactados no arquivo
void dados_compactados(const char* nome_arquivo, FILE* arquivo_compactado, char** tabela_codigos) {
    FILE* arquivo = fopen(nome_arquivo, "rb"); // leitura binária
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }
 
    int byte;
    unsigned char buffer = 0; // Buffer para armazenar os bits
    int posicao_buffer = 0;   // Posição atual no buffer
 
    while ((byte = fgetc(arquivo)) != EOF) {
        // Obtemos o código binário correspondente ao byte atual
        char* codigo = tabela_codigos[byte];
 
        // Adiciona cada bit do código ao buffer
        for (int i = 0; codigo[i] != '\0'; i++) {
            if (codigo[i] == '1') {
                buffer |= (1 << (7 - posicao_buffer)); // Define o bit correspondente a 1
            }
            posicao_buffer++;
 
            // Se o buffer estiver cheio, escreva-o no arquivo compactado
            if (posicao_buffer == 8) {
                fputc(buffer, arquivo_compactado);
                buffer = 0; // Limpa o buffer
                posicao_buffer = 0; // Reinicia a posição do buffer
            }
        }
    }
 
    // Se ainda houver bits no buffer, escreva-os no arquivo compactado
    if (posicao_buffer > 0) {
        fputc(buffer, arquivo_compactado);
    }
 
    fclose(arquivo);
}
 
// Função para calcular o tamanho da árvore de Huffman 
int calcular_tamanho_arvore(No *raiz) 
{
    if (raiz == NULL)  return 0; // Árvore vazia ou posicao nula
 
    if(raiz->esq == NULL && raiz->dir == NULL)// Caso especial, Se for uma folha contendo um caractere especial
    {  
        uchar no = *(uchar*)raiz->byte; //recebe o valor do byte do noh
 
        if(no == '*' || no == '\\') // se for uma folha contendo um dos sinais especiais, o nó conta como 2.
        {
            return 2 + calcular_tamanho_arvore(raiz->esq) + calcular_tamanho_arvore(raiz->dir); 
        }
    }
 
    // casos normais, nó vale 1
    // se for uma folha,sem carcatere especial.
    // Se não for folha, continuamos a percorrer a árvore, conta 1 para o nó atual e soma os tamanhos das subárvores esquerda e direita
    return 1 +  calcular_tamanho_arvore(raiz->esq) + calcular_tamanho_arvore(raiz->dir);
}
 
//Serve para colocar a extensao .huff no arquivo
char* adicionar_extensao(const char* nome_base, const char* extensao) {
    // Calcula o tamanho necessário para o nome + a extensão + '\0'
    size_t tamanho = strlen(nome_base) + strlen(extensao) + 1;
 
    // Aloca memória para o nome completo
    char* nome_completo = (char*)malloc(tamanho);
    if (nome_completo == NULL) {
        perror("Erro ao alocar memória");
        return NULL;
    }
 
    // Cria o nome completo (nome + extensão)
    snprintf(nome_completo, tamanho, "%s%s", nome_base, extensao);
 
    return nome_completo;
}
 
void compactar_arquivo (const char* nome_arquivo_original)
{
    //Passos;
    /*
        Passo 1: Gerar tabela de frequências
        Passo 2: Criar fila de nós e gerar a árvore de Huffman
        Passo 3: Gerar tabela de códigos referente a cada byte
        Passo 4: Calcula o tamanho do arquivo apos a compactação e os bits lixo
        Passo 5: Criar o arquivo compactado e escrever o cabeçalho
        Passo 6: Escrever os dados compactados no arquivo
        Passo 7: Liberar a memória alocada para a tabela de códigos
        Passo 8: Fechar o arquivo compactado
    */
 
    //-------------------------------- Passo 1 ---------------------------------------------------
 
    //O vetor frequencia tem 256 posicoes, uma para cada byte possivel (0 a 255)
    ulli frequencia[TAM] = {0};
 
    //calcula a frequencia dos bytes do arquivo original e amazena no vetor "frequencia"
    tabela_frequencia(nome_arquivo_original, frequencia);
 
 
    //-------------------------------- Passo 2 ---------------------------------------------------
 
    Fila * fp = criar_fila();  //Criar uam fila de prioridade com os bytes do arquivo original e suas respectivas frequencias
    preencher_fila(fp, frequencia); //Preenche a fila de prioridade com os bytes do arquivo original e suas respectivas frequencias
 
    //gerar a árvore de Huffman
    No * raiz=  gerar_arvore(fp);
 
    //-------------------------------- Passo 3 ---------------------------------------------------
 
    //gerar tabela de códigos que armazena os códigos de Huffman para cada byte
    char* tabela_codigos[TAM] = {0};// Inicializa a tabela de códigos com NULL
    char caminho_na_arvore[TAM] = "";//inicializa o caminho na árvore com uma string vazia
 
    //preenche a tabela de códigos com o caminho da árvore de Huffman para cada byte
    tabela_de_codigo(tabela_codigos, raiz, caminho_na_arvore); 
 
 
    //-------------------------------- Passo 4 ---------------------------------------------------
 
    // calcula o quantidade de bits do arquivo original apos a compactacao
    ulli tam_aquivo = contar_bits_totais(nome_arquivo_original, tabela_codigos);
    int bit_lixo= (8 - (tam_aquivo % 8)) % 8; // Calcula o número de bits  lixo
    int tam_arvore = calcular_tamanho_arvore(raiz); // Tamanho da árvore em bits
    //ulli tam_aquivo_compactado = (tam_aquivo + bit_lixo) / 8; // Tamanho do arquivo compactado em bytes 
 
    //-------------------------------- Passo 5 ---------------------------------------------------
 
    //criando o arquivo compactado
    char arquivo[256];
    strcpy(arquivo,nome_arquivo_original);
    char* nome_arquivo_com_extensao = adicionar_extensao(arquivo,".huff"); // ajuste o tamanho conforme necessário
 
    FILE* arquivo_compactado = fopen(nome_arquivo_com_extensao,"wb");
    if( arquivo_compactado == NULL){
        perror("Falha ao abrir o arquivo");
        return;
    }
 
    //O cabeçalho: o numero de bits lixo, tamanho da árvore e a árvore de Huffman
    escrever_cabecalho(arquivo_compactado, bit_lixo, tam_arvore, raiz);
 
    // Escreve a árvore no arquivo compactado;
    //escrever_arvore(arquivo_compactado, raiz); 
 
 
    //-------------------------------- Passo 6 ---------------------------------------------------
 
    //Escreve os dados compactados no arquivo, o caminho da árvore de Huffman para cada byte
    dados_compactados(nome_arquivo_original, arquivo_compactado, tabela_codigos);
 
 
    //-------------------------------- Passo 7 ---------------------------------------------------
 
    // Libere a memória da tabela de códigos
    for (int i = 0; i < TAM; i++) {
        if (tabela_codigos[i] != NULL) {
            free(tabela_codigos[i]);
        }
    }
 
    //-------------------------------- Passo 8 ---------------------------------------------------
    fclose(arquivo_compactado); // fechar o arquivo compactado
 
 
}
 
//--------------------------------Funcoes de debug---------------------------------------------------
 
//Funcao para imprimir a tabela de frequencia
/*void imprimir_frequencia( ulli frequencia[TAM]){
    for( int i = 0; i < TAM; i++){
        if( frequencia[i] > 0){
            printf("%c: %u\n", (uchar)i,frequencia[i]);
        }
    }
}*/
//imprimir a arvore em pre-ordem
void imprimir_arvore(No* raiz) {
    if (raiz == NULL) {
        return;
    }
 
    // Se o nó for uma folha, imprime o byte e um caractere especial para identificar folha
    if (raiz->esq == NULL && raiz->dir == NULL) {
        printf("(%c)", *(uchar*)raiz->byte);
    } else {
        // Se for um nó interno, imprimimos um caractere especial
        printf("*");
    }
 
    // Recursivamente imprime a subárvore esquerda e direita (pré-ordem)
    imprimir_arvore(raiz->esq);
    imprimir_arvore(raiz->dir);
}
 
void imprimir_fila(Fila* queue) {
    No* atual = queue->inicio;  // Começa pelo primeiro nó da lista
    while (atual != NULL) {
        // Converte os ponteiros void* para os tipos corretos antes de imprimir
        printf("byte: %c freq: %llu --> ", *(uchar*)atual->byte, *(ulli*)atual->frequencia);
        atual = atual->prox;  // Avança para o próximo nó
    }
    printf("NULL\n");  // Indica o fim da lista
}
 
//----------------------------------------------------------------------------------------------------------
