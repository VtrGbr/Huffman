/*
COMO USAR??

Basta colocar o arquivo cnf na mesma pasta que o sat solver compilado e executar


ATENÇÃO: o formato precisa ser exatamente igual ao especificado pelo prof

*/
#include "satSolver.h"




void ler_arquivo_cnf(const char* nome_arquivo, CNF* problema){

    //primeiro passo - abrir o arquivo
    FILE* arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("erro na abertura do arquivo cnf (verificar )");
        exit(1);
    }

    char linha[256];
    int idx_clausula = 0;

    //segundo passo - ler o arquivo
    while (fgets(linha, sizeof(linha), arquivo)) {//enquanto houver linhas no arquivo, o while se mantem

        if (linha[0] == 'c') 
            continue; //pula os comentários (linhas que começam com c)

        if (linha[0] == 'p') {
            sscanf(linha, "p cnf %d %d", &problema->num_literais, &problema->num_clausulas); //le o cabeçalho do problema
            //sscanf é uma função que lê a string e não uma entrada do usuário, como o scanf
            //Ex: p cnf 3 4 -> 3 literais e 4 clausulas
            continue; //pula pra próxima linha depois de ler o cabeçalho
        }

        Clausula c = {0}; //cria uma cláusula nova e zera tudo dentro dela

        char *token = strtok(linha, " "); //começa a separa o linha em tokens (separando os literais)
        while (token != NULL) {
            int literal = atoi(token); //converte o token para literal (int)

            if (literal == 0)
                break; //se for 0 para a esse while

            c.literais[c.tamanho++] = literal; //adiciona o literal na cláusula e aumenta o tamanho dela
            token = strtok(NULL, " "); //vai para o próximo literal
        }

        problema->clausulas[idx_clausula++] = c; //guarda uma clausula nova na estrutura do CNF
    }

    fclose(arquivo);
}

int resolver_cnf(CNF* problema, int atribuicoes[]) {
    int todas_satisfeitas = 1; //variavel que vai guardar se todas as clausulas foram satisfeitas ou não

    for (int i = 0; i < problema->num_clausulas; i++) {//passa por todas as clausulas do problema
        //verifica se a clausula foi satisfeita ou não

        Clausula *cl = &problema->clausulas[i];
        int clausula_satisfeita = 0;
        int indefinida = 0;
        
        for (int j = 0; j < cl->tamanho; j++) {//vai passar por cada literal da clausula
            int literal = cl->literais[j]; //pega o literal da clausula
            int variavel = abs(literal); //pega o valor absoluto do literal (se for negativo, pega o positivo)
            int valor = atribuicoes[variavel]; //pega o valor atribuido a variavel (se for 1 ou -1)
            
            if (valor == 0) {//se for 0 não foi atribuido ainda
                indefinida = 1; //ainda não atribuída
            } else if ((literal > 0 && valor == 1) || (literal < 0 && valor == -1)) { //se o literal for positivo e o valor for 1 ou se o literal for negativo e o valor for -1
                clausula_satisfeita = 1; //a clausula foi satisfeita
                break; //pelo menos um literal é verdadeiro
            }
        }
        
        if (!clausula_satisfeita && !indefinida) {
            return UNSAT; //todas as clausulas não foram satisfeitas e não tem mais nada indefinido (ou seja, essa expressão é unsat)
        }
        if (!clausula_satisfeita && indefinida) {
            todas_satisfeitas = 0; //ainda indefinida
        }
    }
    
    return todas_satisfeitas ? SAT : UNDEFINED;

}

int resolver_sat(Arvr *no, CNF *problema, int solucao[]){

    int estado =  resolver_cnf(problema, no->atribuicoes); //estado recebe sat, unsat ou indefinido (0, 1 ou 2)

        if (estado == SAT) { //se o estado for sat, a solução foi encontrada
        memcpy(solucao, no->atribuicoes, sizeof(int) * (problema->num_literais + 1)); //copia as atribuições para a solução
        return SAT; //retorna sat
        }

        if (estado == UNSAT) {//se o estado for unsat, volta pra árvore (backtracking)
            return UNSAT; //insatisfatível
        }

        //aqui começa o uso de árvore (se o estado for indefinido)

        //proximo passo - procurar a próxima variável a ser atribuida

        int proxima_variavel = -1; //variável que vai ser atribuida (se for -1 vai terminar com a variável sendo indefinida (ou seja, unsat))
        for (int i = 1; i <= problema->num_literais; i++) {
            if (no->atribuicoes[i] == 0) { ////tenta procurar a proxima variável que não foi atribuida ainda
                proxima_variavel = i;//
                break;
            }
        }

        if (proxima_variavel == -1) {
            return UNSAT; //não tem mais variaveis pra atribuir (ou seja, todas foram atribuídas)
        }

        //tenta atribuir verdadeiro, fazendo um novo nó e atribuindo
        Arvr *esq = malloc(sizeof(Arvr));
        memcpy(esq->atribuicoes, no->atribuicoes, sizeof(int) * MAX_LIT);//copia as atribuições do nó atual para o novo nó
        esq->atribuicoes[proxima_variavel] = 1;
        esq->variavel = proxima_variavel;
        esq->valor = 1;
        esq->esquerda = NULL;
        esq->direita = NULL;
    
        if (resolver_sat(esq, problema, solucao)) {//aqui vem a recursão pra resolver o problema com árvore
        free(esq);
        return SAT;
        }

        //tenra atribuir falso, fazendo um novo nó e atribuindo
        Arvr *dir = malloc(sizeof(Arvr));
        memcpy(dir->atribuicoes, no->atribuicoes, sizeof(int) * MAX_LIT);//copia as atribuições do nó atual para o novo nó
        dir->atribuicoes[proxima_variavel] = -1;
        dir->variavel = proxima_variavel;
        dir->valor = -1;
        dir->esquerda = NULL;
        dir->direita = NULL;
    
        if (resolver_sat(dir, problema, solucao)) {
        free(dir);
        return SAT;
        }
    
        //se nenhuma atribuição funcionar, volta pra árvore (backtracking)
        free(esq);
        free(dir);
        return UNSAT;

    }




int main(){
    CNF problema = {0}; //começa a estruturação do problema
    char arquivo[50]; // nome do arquivo

    printf("Digite o nome do arquivo (com a extensão cnf, por favor)!\n");
    fgets(arquivo,sizeof(arquivo),stdin);
    arquivo[strcspn(arquivo, "\n")] = 0; // Tira o \n da string
    
    ler_arquivo_cnf(arquivo, &problema); //lê o arquivo cnf
    
    //cria a raiz
    Arvr raiz = {0};
    raiz.esquerda = NULL;
    raiz.direita = NULL;
    
    //começa as atribuições
    for (int i = 0; i <= problema.num_literais; i++) {
        raiz.atribuicoes[i] = 0;
    }
    
    //guarda a solução
    int solucao[MAX_LIT] = {0};
    
    //tenta resolver o problema
    if (resolver_sat(&raiz, &problema, solucao)) {
        printf("\n===== SATISFATIVEL! =====\n\n");
        printf("Solucao encontrada:\n");
        printf("Literal - Valor\n");
        printf("-----------------\n");
        
        for (int i = 1; i <= problema.num_literais; i++) {
            printf("%5d   - %3d\n", i, solucao[i] == 1 ? 1 : 0);
        }
    } else {
        printf("\n===== INSATISFATIVEL! =====\n");
    }
    
    return 0;
}
