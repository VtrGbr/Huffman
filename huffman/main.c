#include "compactar/compactar.h"
#include "descompactar/descompactar.h"

/*
segunda opção de include
#include "compactar.h"
#include "descompactar.h"
*/

int main() {
    char nome_arquivo_original[100], nome_arquivo_compactado[100];
    int opcao;
    
    do
    {
        printf("\nDigite a opcao que voce quer:\n 1 - compactar;\n 2 - descompactar;\n 3 - sair\n");
        scanf("%d",&opcao);
        getchar();

        if( opcao == 1){
            // Lendo os nomes dos arquivos
            printf("Digite o nome do arquivo original: ");
            fgets(nome_arquivo_original, sizeof(nome_arquivo_original), stdin);
            nome_arquivo_original[strcspn(nome_arquivo_original, "\n")] = 0; // Remover nova linha
            compactar_arquivo(nome_arquivo_original);

        }else if( opcao == 2){
           
           descompactarPrincipal();

        }else if (opcao < 1 || opcao > 3){
            printf("opcao invalida\n");
        }

    } while (opcao != 3);
    
    return 0;
}