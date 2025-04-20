#include "descompactar.h"
//#include "compactar.h"


// Função para abrir um arquivo, solicitando o nome ao usuário
FILE* AbrirArquivo(char *nome) {
    FILE *arquivo;
    while (1) {
        printf("Digite o nome do arquivo: ");
        scanf("%s", nome);
        arquivo = fopen(nome, "rb"); // Abre o arquivo em modo leitura binária
        if (arquivo != NULL) {
            return arquivo; // Retorna o ponteiro do arquivo se aberto com sucesso
        } else {
            printf("ERRO: Não foi possível abrir o arquivo '%s'. Tente novamente.\n", nome);
        }
    }
}

no_t *criar_no(unsigned int caractere, no_t *esquerda, no_t *direita) 
{
    no_t *novo_no = (no_t *)malloc(sizeof(no_t));
    novo_no->caractere = caractere;
    novo_no->esquerda = esquerda;
    novo_no->direita = direita;
    return (novo_no);
}

/* apaga árvore*/
void liberar_arvore(no_t *arvore_huff) 
{
    if (arvore_huff != NULL) 
    {
        liberar_arvore(arvore_huff->esquerda);
        liberar_arvore(arvore_huff->direita);
        free(arvore_huff);
    }
}

/* verifica se o nó é uma folha ou não*/
int eh_folha_descompactar(no_t *arvore_binaria) 
{
    return ((arvore_binaria != NULL) && (arvore_binaria->esquerda == NULL) && (arvore_binaria->direita == NULL));
}

/* verifica se o bit está definido na posição ou não*/
//unsigned int caractere: valor ao qual queremos verificar o bit
//posicao: eh a posicao do bit que queremos verificar
int bit_esta_definido_descompactar(unsigned int caractere, int posicao) 
{
    //A variável mascara é criada para isolar o bit na posição desejada, com 1 << posicao deslocando um único bit para a esquerda até a posição especificada.

    /*
    Exemplo: Digamos que vamos analisar a posicao 3, entao quando fizermos 1 << posicao, ficaremos com:
                             1 (00000001)    ---->  1 << 3 -----> (00001000)

    Agora a mascara possui um 1 somente na posição desejada e '0s' nas outras posições, o que ajuda a isolar o bit específico.
    */
    unsigned int mascara = 1 << posicao;


    //Vamos comparar o valor da mascara com o valor de bit que queremos comparar

    /*
    Exemplo: posicao = 3(00000011), caracter = 13 (00001101)

    Quando fizermos a mascara, ficarah com (00001000)
    Entao quando fizermos (00001000) & ( 00001101). Vamos notar que o valor serah : 000010000

    Logo concluimos que o bit na posicao 3 estah ativado

    Caso o bit nao esteja ativado ele retorna 0, caso contrario retorna algum valor diferente de 0
    */
    return (mascara & caractere);
}


/*Esta função é responsável por ler os dados compactados de um arquivo, navegar na árvore de Huffman reconstruída com base nos bits lidos, 
e escrever os bytes descompactados em outro arquivo.*/
void descompactar(FILE *arquivo_entrada, unsigned int tamanho_lixo, int tamanho_arvore, no_t *arvore_huff, FILE *arquivo_saida) 
{
    if (tamanho_arvore == 0) 
    {
        return;
    }

    no_t *no_atual = arvore_huff;
    unsigned int byte_atual; //Armazena o valor do byte atual que está sendo lido do arquivo compactado.
    unsigned int ultimo_byte; //Armazena o último byte do arquivo para tratar os bits de lixo.
    long long int total_bytes; 
    long long int bytes; //contador de bytes lidos
    int i;  // Indice usado para percorrer os bits do byte atual, da posição mais significativa para a menos significativa.

    //fseek move o ponteiro do arquivo para o último byte, e getc lê esse byte para armazená-lo em ultimo_byte.
    fseek(arquivo_entrada, -1, SEEK_END);
    ultimo_byte = getc(arquivo_entrada);
    //ftell obtém a posição total de bytes do arquivo (total_bytes), que será usada para delimitar o loop de leitura dos dados compactados.
    total_bytes = ftell(arquivo_entrada);

    //Posiciona o ponteiro do arquivo logo após os dois primeiros bytes do cabeçalho (tamanho da arvore e lixo) e o trecho que representa a árvore de Huffman.
    //Esse ponto marca o início dos dados compactados no arquivo, pois temos no comeco 2 bytes que sao
    // para o lixo e o tamanho da arvore, entao os dados compactados comecam depois desses dois bytes.
    fseek(arquivo_entrada, (2 + tamanho_arvore), 0);


    //Aqui serah onde iremos ler e descompactar os dados
    byte_atual = getc(arquivo_entrada);

    //Ler os cada bytes dos dados compactados, um por um, ate o penultimo byte
    //Lembrete: cada bit determina para onde viajaremos na arvore( 1 - direita, 0 - esquerda)
    for (bytes = (2 + tamanho_arvore); bytes < (total_bytes - 1); bytes++) 
    {
        // Para cada byte, vamos processar cada bit individualmente, do mais significativo "posição 7" ao menos significativo "posição 0".
        for (i = 7; i >= 0; i--) 
        {
            /* se o bit estiver definido, anda para a direita na árvore */
            if (bit_esta_definido_descompactar(byte_atual, i) != 0) 
            {
                if (no_atual->direita != NULL)
                {
                    no_atual = no_atual->direita;
                }
            }
            else 
            {
                if (no_atual->esquerda != NULL)
                {
                    no_atual = no_atual->esquerda;
                }
            }//Caso seja uma folha, coloque o caracter ou byte no arquivo descompactado
            if (eh_folha_descompactar(no_atual) != 0) 
            {
                fprintf(arquivo_saida, "%c", no_atual->caractere);
                no_atual = arvore_huff; //reinicia o ponteiro para a raiz da árvore, assim podemos decodificar o próximo caractere.
            }
        }
        byte_atual = getc(arquivo_entrada);
    }

    //Agora vamos processar o ultimo byte, considerando os bytes de lixo
    //Percorremos apenas os bits úteis, de 7 até (7 - tamanho_lixo + 1), ignorando os bits lixo no final.
    for (i = 7; i >= (signed int)tamanho_lixo; i--) 
    {   
        //Observacao: o casting signed int serve para nao dar erro de comparacao, ja que originalmente "tamanho_lixo" eh um unsigned int, ou seja, um inteiro que nao assume sinal negativo. Porem, "i" assume valores tanto positivos quanto negativos, entao para nao gerar erros na comparacao entre "i" e "tamanho_lixo" fazemos este casting



        //O processo de navegacao pela arvore eh o mesmo do loop anterior
        if (bit_esta_definido_descompactar(byte_atual, i) != 0) 
        {
            if (no_atual->direita != NULL)
            {
                no_atual = no_atual->direita;
            }
        }
        else 
        {
            if (no_atual->esquerda != NULL)
            {
                no_atual = no_atual->esquerda;
            }
        }
        if (eh_folha_descompactar(no_atual) != 0) 
        {
            fprintf(arquivo_saida, "%c", no_atual->caractere);
            no_atual = arvore_huff;
        }
    }
    printf("Descompactacao concluida!\n");
    return;
}

unsigned int obter_tamanho_lixo(FILE *arquivo_entrada) 
{
    unsigned int tamanho_lixo = 0;
    unsigned char primeiro_byte; //Serve para armazenar o primeiro byte lido do arquivo, que contem o valor de "tamanho_lixo" nos três bits mais significativos.
    
    fseek(arquivo_entrada, 0, SEEK_SET); //move o ponteiro para o inicio do arquivo
    
    primeiro_byte = getc(arquivo_entrada);
    /*
    Como os 3 bits de lixo estao nos 3 bits mais significativos do byte, entao vamos deslocalos para os 3 bits menos significativos do byte
    */
    tamanho_lixo = (unsigned int)(primeiro_byte >> 5); // Obtém o tamanho do lixo
    return (tamanho_lixo);

    /*
    Vamos usar um exemplo de aplicacao:
    Digamos que o primeiro byte do arquivo (primeiro_byte) seja 11010000:

    Deslocamento dos Bits:
    11010000 >> 5 desloca cinco posições para a direita, resultando em 00000011.
    Valor de tamanho_lixo:
    tamanho_lixo terá o valor 3, o que significa que o último byte do arquivo contém três bits de lixo que devem ser ignorados durante a descompactação.
    */
}

unsigned int obter_tamanho_arvore(FILE *arquivo_entrada) 
{
    unsigned int tamanho_arvore;
    unsigned char primeiro_byte;
    unsigned char segundo_byte;

    //Esse comando move o "cursor" do arquivo para o início (0) do arquivo (SEEK_SET), para garantir que a leitura comece do início.
    fseek(arquivo_entrada, 0, SEEK_SET);

    primeiro_byte = getc(arquivo_entrada); 
    segundo_byte = getc(arquivo_entrada);

    primeiro_byte = primeiro_byte << 3; //"descarte" dos 3 bits mais significativos, ou seja, os bits do lixo de memoria
    primeiro_byte = primeiro_byte >> 3; // Agora voltamos com os 5 bits para as suas posicoes menos significativas, porem, agora os 3 bits mais significativos estao zerados. Agora o primeiro byte possui somente os valores do tamanho da arvore
    //Vamos posicionar os 5 bits menos significativos com os cinco bits mais significativos do segundo byte. Assim teremos o tamanho da arvore
    tamanho_arvore = ((primeiro_byte << 8) | segundo_byte);
    
    return (tamanho_arvore);

    /*Exemplo de aplicacao:
    Digamos que os dois primeiros bytes do arquivo sejam:

    Primeiro byte : 11010101 
    Segundo byte : 00101100 
    Isolamento dos Bits de tamanho_arvore:

    primeiro_byte = 11010101
    primeiro_byte << 3 resulta em 10101000 .
    primeiro_byte >> 3 resulta em 00010101.
    Agora, primeiro_byte tem o valor 00010101, representando os cinco bits de tamanho_arvore.

    Combinação para Obter tamanho_arvore:

    primeiro_byte << 8 desloca 00010101 para 0001010100000000.
    segundo_byte é 00101100.
    Fazendo o OR bit a bit: 0001010100000000 | 00101100 resulta em 0001010100101100, que é 5420 em decimal.
    
    
    */
}

unsigned int *obter_array_arvore(FILE *arquivo_entrada, unsigned int tamanho_arvore) 
{
    unsigned int i;
    unsigned int *array_arvore = (unsigned int *)malloc(sizeof(unsigned int) * tamanho_arvore);
    
    fseek(arquivo_entrada, 2, 0);

    for (i = 0; i < tamanho_arvore; i++) 
    {
        array_arvore[i] = getc(arquivo_entrada);
    }
    return (array_arvore); 
}

//Esse é um ponteiro para ponteiro porque a função vai avançando dentro do array à medida que processa os nós.
no_t *construir_arvore(unsigned int **array_arvore) 
{
    //Achamos um no interno, e terah filhos na esquerda e na direita
    if (**array_arvore == '*') 
    {
        no_t *esquerda;
        (*array_arvore)++;  //nos movemos para o proximo caracter, pois queremos "sair" do  *, ja que ele ja foi processado
        //"Viajamos" para a esquerda
        esquerda = construir_arvore(array_arvore); // "Viajamos" para a esquerda, pois estamos percorrendo em pre-ordem, entao o filho da esquerda
        //vem primeiro
        (*array_arvore)++; //Avança de novo no array após processar a subárvore esquerda, indo agora para o lado direito.
        
        //Cria um nó com o caractere '*' (nó interno), com os dois filhos — o esquerdo já construído e o direito retornado pela chamada recursiva.
        return (criar_no('*', esquerda, construir_arvore(array_arvore)));
    }
    //Este noh eh uma folha
    else if (**array_arvore == '\\') //é um caractere de escape, usado para distinguir caracteres especiais como '*' ou '\\' usados como dados reais (folhas).
    {
        *array_arvore = (*array_arvore + 1); //Incrementamos para o proximo valor que serah interpretado literalmente
                                            // Ou seja,Avança para o próximo valor, que serah o caractere real da folha, como '*' ou '\\'.
        return (criar_no(**array_arvore, NULL, NULL));
    }

    //Caso nao seja "*" ou "//" entao representamos este valor diretamente, pois ele eh um noh folha e contem o byte ou caracter desejado
    return (criar_no(**array_arvore, NULL, NULL)); 
}

//Reconstroi a estrutura de nos que representa a arvore na memoria, tendo em vista que ela pega a arvore do arquivo que esta de forma seriada (impressa em pre-ordem)
no_t *obter_arvore(FILE *arquivo_entrada, unsigned int tamanho_arvore) 
{
    if (tamanho_arvore == 0) //	Verifica se ha arvore a reconstruir
    {
        return NULL;
    }

    unsigned int *array_arvore = obter_array_arvore(arquivo_entrada, tamanho_arvore); //	Lê os dados serializados da arvore do arquivo
    /*Esse ponteiro é guardado para que possamos depois liberar corretamente a memória alocada com free, 
    pois a função construir_arvore irá modificar array_arvore (passado por referência como ponteiro para ponteiro).*/
    unsigned int *array_arvore_auxiliar = array_arvore; 

    no_t *raiz_arvore = construir_arvore(&array_arvore); //Constrói a arvore recursivamente a partir do array

    free(array_arvore_auxiliar);

    return (raiz_arvore); //A função retorna a raiz da arvore de Huffman que foi reconstruída a partir da serialização.
}

FILE* remover_huff(char nome[])
{
    int n = strlen(nome);
    nome[n - 5] = '\0'; // Remove a extensão do arquivo
    return fopen(nome, "wb"); // Abre o arquivo de saída em modo escrita binária
}

void descompactarPrincipal()
{
    char nome[100];
    FILE *arquivo_entrada;
    FILE *arquivo_saida;
    
    while (1) { // Loop até que o arquivo seja encontrado
        arquivo_entrada = AbrirArquivo(nome); // Chama a função AbrirArquivo
        
        if (arquivo_entrada == NULL) {
            printf("ERRO: Não existe arquivo com o nome digitado. Por favor, tente novamente.\n");
        } else {
            printf("Arquivo encontrado.\n");
            break; // Sai do loop se o arquivo for encontrado
        }
    }

    // Continua com a execução da função se o arquivo foi encontrado
    arquivo_saida = remover_huff(nome);
    unsigned int tamanho_lixo = obter_tamanho_lixo(arquivo_entrada);
    unsigned int tamanho_arvore = obter_tamanho_arvore(arquivo_entrada);
    no_t *arvore_huff = obter_arvore(arquivo_entrada, tamanho_arvore);
    descompactar(arquivo_entrada, tamanho_lixo, tamanho_arvore, arvore_huff, arquivo_saida);
    liberar_arvore(arvore_huff);
    fclose(arquivo_entrada);
    fclose(arquivo_saida);
}