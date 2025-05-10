#!/bin/bash

# Caminho da pasta Downloads do Windows (ajuste se necessário)
USER="Vgabr"
CAMINHO_WINDOWS="/mnt/c/Users/$USER/Downloads"

# Caminho de destino no WSL
DESTINO_WSL="/home/vitor/huffman/huffman/testes"

# Garante que a pasta de destino existe
mkdir -p "$DESTINO_WSL"

# Encontra arquivos modificados nos últimos 5 minutos e copia para o destino
find "$CAMINHO_WINDOWS" -type f -mmin -5 -exec cp -v {} "$DESTINO_WSL" \;
