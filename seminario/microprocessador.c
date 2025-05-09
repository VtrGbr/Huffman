#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de Evento
typedef struct Evento {
    char descricao[100];
    int prioridade;
    struct Evento* prox;
} Evento;

// Fila de eventos
typedef struct {
    Evento* inicio;
} Fila;

// Funções de gerenciamento da fila
Fila* criar_fila() {
    Fila* fila = (Fila*)malloc(sizeof(Fila));
    if (!fila) {
        perror("Erro ao criar fila");
        exit(1);
    }
    fila->inicio = NULL;
    return fila;
}

Evento* criar_evento(const char* descricao, int prioridade) {
    Evento* evento = (Evento*)malloc(sizeof(Evento));
    if (!evento) {
        perror("Erro ao criar evento");
        exit(1);
    }
    strcpy(evento->descricao, descricao);
    evento->prioridade = prioridade;
    evento->prox = NULL;
    return evento;
}

void adicionar_evento(Fila* fila, Evento* evento) {
    if (fila->inicio == NULL || evento->prioridade < fila->inicio->prioridade) {
        evento->prox = fila->inicio;
        fila->inicio = evento;
    } else {
        Evento* aux = fila->inicio;
        while (aux->prox && aux->prox->prioridade <= evento->prioridade) {
            aux = aux->prox;
        }
        evento->prox = aux->prox;
        aux->prox = evento;
    }
}

Evento* remover_evento(Fila* fila) {
    if (!fila || !fila->inicio) {
        printf("Nenhum evento para tratar!\n");
        return NULL;
    }
    Evento* removido = fila->inicio;
    fila->inicio = removido->prox;
    removido->prox = NULL;
    return removido;
}

void imprimir_eventos(Fila* fila) {
    Evento* atual = fila->inicio;
    if (!atual) {
        printf("Nenhum evento pendente.\n");
    } else {
        printf("\nFila de eventos pendentes:\n");
        while (atual) {
            printf("Evento: %s | Prioridade: %d\n", atual->descricao, atual->prioridade);
            atual = atual->prox;
        }
    }
    printf("\n");
}

void liberar_fila(Fila* fila) {
    Evento* atual = fila->inicio;
    while (atual) {
        Evento* temp = atual;
        atual = atual->prox;
        free(temp);
    }
    free(fila);
}

// ---------- Simulando as Interrupções (ISRs) ----------
void ISR_botao_emergencia(Fila* fila) {
    printf("[ISR] Botão de Emergência acionado!\n");
    adicionar_evento(fila, criar_evento("Botão de Emergência", 1));
}

void ISR_temperatura_critica(Fila* fila) {
    printf("[ISR] Temperatura Crítica detectada!\n");
    adicionar_evento(fila, criar_evento("Temperatura Crítica", 2));
}

void ISR_queda_energia(Fila* fila) {
    printf("[ISR] Queda de Energia detectada!\n");
    adicionar_evento(fila, criar_evento("Queda de Energia", 3));
}

void ISR_manutencao_preventiva(Fila* fila) {
    printf("[ISR] Manutenção Preventiva necessária.\n");
    adicionar_evento(fila, criar_evento("Problemas de Manutenção", 4));
}

void ISR_LedApagando(Fila* fila) {
    printf("[ISR] Sistema funcionando normalmente.\n");
    adicionar_evento(fila, criar_evento("Led apagando", 5));
}

// ---------- Simulação principal com preempção ----------
void menu() {
    printf("\nSimulador de Sistema de Segurança Industrial (com Preempção):\n");
    printf("1 - Botão de Emergência\n");
    printf("2 - Temperatura Crítica\n");
    printf("3 - Queda de Energia\n");
    printf("4 - Problema de Manutenção Preventiva\n");
    printf("5 - Led apagando\n");
    printf("6 - Tratar eventos\n");
    printf("7 - Mostrar fila de eventos\n");
    printf("0 - Finalizar\n");
}

void simular_ISR_durante_tratamento(Fila* fila) {
    int opcao;
    printf("\n*** Nova Interrupção Durante Tratamento! ***\n");
    printf("Escolha qual evento disparar:\n");
    printf("1 - Botão de Emergência\n");
    printf("2 - Temperatura Crítica\n");
    printf("3 - Queda de Energia\n");
    printf("4 - Problema de Manutenção\n");
    printf("5 - Led apagando\n");
    printf("0 - Nenhum\n");

    printf("Opção: ");
    scanf("%d", &opcao);
    getchar(); // limpar '\n'

    //Adiciona a opcao desejada na fila
    switch (opcao) {
        case 1: ISR_botao_emergencia(fila); break;
        case 2: ISR_temperatura_critica(fila); break;
        case 3: ISR_queda_energia(fila); break;
        case 4: ISR_manutencao_preventiva(fila); break;
        case 5: ISR_LedApagando(fila); break;
        case 0: default: printf("Nenhum novo evento gerado.\n"); break;
    }
}

void tratar_eventos(Fila* fila) {
    Evento* atual = NULL;

    //Enquanto a fila tiver elementos
    while (fila->inicio != NULL) {
        atual = remover_evento(fila); // Pega o elemento de maior prioridade

        printf("\n>>> Iniciando tratamento: %s (Prioridade %d)\n", atual->descricao, atual->prioridade);

        for (int i = 0; i < 3; i++) {
            printf("... Trabalhando no evento: %s [Passo %d/3]\n", atual->descricao, i+1);

            // Perguntar se o usuário quer simular uma nova interrupção
            //So me pergunta uma vez para fins praticos de demonstracao
            if( i == 0){
                
                printf("Deseja simular uma nova interrupção? (s/n): ");
                char resposta;
                scanf(" %c", &resposta);
                getchar(); // limpar '\n'
    
                if (resposta == 's' || resposta == 'S') {
                    simular_ISR_durante_tratamento(fila);
    
                    // Checa se há um evento de prioridade maior
                    if (fila->inicio && fila->inicio->prioridade < atual->prioridade) {
                        printf("\n!!! Preempção detectada !!!\n");
                        printf("Novo evento mais urgente: %s (Prioridade %d)\n", fila->inicio->descricao, fila->inicio->prioridade);
                        
                        // Guarda o evento atual de volta na fila
                        adicionar_evento(fila, atual);
                        break; // sai do for para tratar o mais urgente
                    }
                }
            }
        }

        // Se o evento foi totalmente tratado (não preemptado)
        if (fila->inicio == NULL || atual->prioridade <= fila->inicio->prioridade) {
            printf(">>> Evento tratado: %s\n", atual->descricao);
            free(atual);
        }
    }

    printf("\nTodos os eventos foram tratados.\n");
}


int main() {
    Fila* fila_eventos = criar_fila();
    int opcao;

    menu();
    do {
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // limpar o '\n'

        switch (opcao) {
            case 1:
                ISR_botao_emergencia(fila_eventos);
                break;
            case 2:
                ISR_temperatura_critica(fila_eventos);
                break;
            case 3:
                ISR_queda_energia(fila_eventos);
                break;
            case 4:
                ISR_manutencao_preventiva(fila_eventos);
                break;
            case 5:
                ISR_LedApagando(fila_eventos);
                break;
            case 6:
                tratar_eventos(fila_eventos);
                break;
            case 7:
                imprimir_eventos(fila_eventos);
                break;
            case 0:
                printf("Finalizando o sistema...\n");
                break;
            default:
                printf("Opção inválida!\n");
                break;
        }
    } while (opcao != 0);

    liberar_fila(fila_eventos);
    return 0;
}
