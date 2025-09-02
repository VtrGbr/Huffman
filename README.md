Claro! Aqui está uma versão melhorada do seu README, utilizando Markdown para uma melhor formatação, clareza e profissionalismo.

Projeto de Estrutura de Dados: Compactador de Arquivos
Este repositório contém um projeto desenvolvido para a disciplina de Estrutura de Dados. O projeto principal consiste na implementação do algoritmo de Huffman para compactação e descompactação de arquivos. Além disso, inclui códigos auxiliares para um seminário e para a plotagem de dados de desempenho.

Pré-requisitos
Antes de começar, garanta que você tenha os seguintes softwares instalados:

Um compilador C/C++ (como GCC ou G++).

Acesso ao MATLAB (seja a versão Online ou o software para desktop).

Estrutura do Projeto
O projeto está organizado nas seguintes pastas:

/huffman: Contém o código-fonte do compactador e descompactador de arquivos.

/seminario: Código de apoio utilizado na apresentação do seminário.

/plotagem: Scripts para gerar dados e plotar gráficos de desempenho.

1. Compactador e Descompactador de Arquivos (Huffman)
Siga os passos abaixo para compilar e executar a aplicação principal.

Compilação
Navegue até a pasta huffman:

```bash
    cd huffman
```

Compile os arquivos-fonte usando o GCC. O comando abaixo irá gerar um executável chamado projeto_huffman.

```bash
    gcc compactar/compactar.c descompactar/descompactar.c main.c -o projeto_huffman
``` 
Nota: Se você utiliza o compilador G++, basta substituir gcc por g++ no comando acima.

Execução
Execute o programa recém-compilado:

```bash
    ./projeto_huffman
```
O programa é interativo. Siga as instruções no terminal para escolher entre compactar ou descompactar e forneça o caminho do arquivo desejado quando solicitado. É recomendado que o arquivo de entrada esteja na mesma pasta (huffman) para facilitar.

2. Código do Seminário
Este código refere-se a uma demonstração sobre microprocessadores.

Navegue até a pasta seminario:

```bash

    cd seminario
```
Compile o código-fonte:

```Bash

gcc microprocessador.c -o simulador_microprocessador
```
Execute o programa:

```bash

./simulador_microprocessador
```
3. Geração e Plotagem de Gráficos com MATLAB
Este processo é dividido em duas etapas: primeiro, gerar um arquivo de dados com um programa em C e, em seguida, usar o MATLAB para plotar esses dados.

Parte 1: Gerando os Dados (C)
Navegue até a pasta plotagem:

```Bash

    cd plotagem
```
Compile o programa contagem.c, que será responsável por gerar os dados:

```bash

gcc contagem.c -o gerador_valores
```
Execute o programa para criar o arquivo de dados:

```bash

./gerador_valores
```
Este comando executará o programa em C, que por sua vez irá gerar (ou sobrescrever) um arquivo chamado valores.txt no mesmo diretório.

Parte 2: Plotando os Dados (MATLAB)
Abra o MATLAB (Online ou a versão para desktop).

Importante: Certifique-se de que o arquivo valores.txt (gerado na etapa anterior) esteja no diretório de trabalho atual do MATLAB.

Na versão desktop, você pode navegar até a pasta plotagem.

Na versão online, você precisará fazer o upload do arquivo valores.txt.

Abra o arquivo plote.m em um editor de texto e copie todo o seu conteúdo.

Cole o código copiado na janela de comando do MATLAB e pressione Enter, ou cole-o em um novo Script (.m) e clique em Run.

O MATLAB executará o script e exibirá o gráfico gerado a partir dos dados do arquivo valores.txt.