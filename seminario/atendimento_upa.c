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

// Criar uma fila
Fila* criar_fila() {
    Fila* fila = (Fila*)malloc(sizeof(Fila));
    if (fila == NULL) {
        perror("Erro ao alocar memoria para a fila");
        exit(1);
    }
    fila->inicio = NULL;
    return fila;
}

//Essa funcao transforma a string da pulseira em numero para podermos adicionar na fila
int numero_urgencia(char* urgencia) {
    if (strcmp(urgencia, "vermelho") == 0) return 1;
    else if (strcmp(urgencia, "laranja") == 0) return 2;
    else if (strcmp(urgencia, "amarelo") == 0) return 3;
    else if (strcmp(urgencia, "verde") == 0) return 4;
    else if (strcmp(urgencia, "azul") == 0) return 5;
    return 10; // caso seja digitado algo errado, não quero que ele seja o primeiro a estar na fila
}

//Nessa função ela transforma o numero da urgencia em string para podermos imprimir no "telao"
char* nome_urgencia(int urgencia) {
    switch(urgencia){
        case 1: return "emergencia";
        case 2: return "muito urgente";
        case 3: return "urgente";
        case 4: return "pouco urgente";
        case 5: return "nao urgente";
        default: return "desconhecida";
    }
}

//Criamos um noh para o paciente, para evitar estar colocando valores demais em uma funcao
Paciente* criar_paciente(char *nome, int urgencia, int cartao_sus) {
    Paciente* pacient = (Paciente*)malloc(sizeof(Paciente));
    if (pacient == NULL) {
        perror("Erro na criacao do paciente");
        exit(1);
    }
    pacient->cartao_sus = cartao_sus;
    pacient->urgencia = urgencia;
    strcpy(pacient->nome, nome);
    pacient->prox = NULL;
    return pacient;
}

//Adicionamos de maneira prioritaria na fila, de maneira crescente
void adicionar(Fila* fila, Paciente* paciente) {
    if (fila->inicio == NULL || paciente->urgencia < fila->inicio->urgencia) {
        paciente->prox = fila->inicio;
        fila->inicio = paciente;
    } else {
        Paciente* aux = fila->inicio;
        while (aux->prox != NULL && aux->prox->urgencia <= paciente->urgencia) {
            aux = aux->prox;
        }
        paciente->prox = aux->prox;
        aux->prox = paciente;
    }
}

//Removemos um paciente para que ele seja atendido
Paciente* remover(Fila* fila) {
    if (fila == NULL || fila->inicio == NULL) {
        printf("Fila vazia!\n");
        return NULL;
    }
    Paciente* removido = fila->inicio;
    fila->inicio = removido->prox;
    removido->prox = NULL;
    return removido;
}

//Imprimir um paciente
void imprimir(Fila* fila) {
    Paciente* atual = fila->inicio;
    if(atual == NULL){ // Caso a fila esteja vazia
        printf("Nao ha pacientes a serem atendidos!\n");
    }else{
        while (atual != NULL) {
            printf("Nome: %s | Urgência: %s\n", atual->nome, nome_urgencia(atual->urgencia));
            atual = atual->prox;
        }
        printf("\n");
    }
}
void menu(){
    printf("Digite as opcoes de atendimento:\n-adicionar: Adicionar pessoa na fila de atendimento;\n-atender: atenderemos um paciente;\n-imprimir: Imprimir as pessoas da fila\n-finalizar: Finalizar o programa;\n");
}


int main() {
    Fila* upa = criar_fila();
    Paciente* atendido;
    int cartao, urgencia;
    char opcao[20], nome[100], situacao_atendimento[50];
    
    do {
        menu();
        printf("\n");
        fgets(opcao, sizeof(opcao), stdin);
        opcao[strcspn(opcao, "\n")] = '\0';

        if (strcmp(opcao, "adicionar") == 0) {
            printf("Nome: ");
            scanf("%s", nome);
            printf("Cartão do SUS: ");
            scanf("%d", &cartao);
            printf("Cor da pulseira (ex: vermelho): ");
            scanf("%s", situacao_atendimento);
            getchar(); // Limpa o \n restante

            urgencia = numero_urgencia(situacao_atendimento);
            Paciente* paciente = criar_paciente(nome, urgencia, cartao);
            adicionar(upa, paciente);

        } else if (strcmp(opcao, "atender") == 0) {
            atendido = remover(upa);
            if (atendido != NULL) {
                printf("Paciente chamado: %s\nGrau de urgência: %s\n", atendido->nome, nome_urgencia(atendido->urgencia));
                free(atendido);
            }

        } else if (strcmp(opcao, "imprimir") == 0) {
            imprimir(upa);
        }
        printf("\n");
    } while (strcmp(opcao, "finalizar") != 0);

    return 0;
}