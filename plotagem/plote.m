% Carrega os dados do arquivo
dados = load('valores.txt');

% Separa as colunas
n = dados(:,1);        % número de inserções
heap = dados(:,2);     % comparações na heap
lista = dados(:,3);    % comparações na lista encadeada

% Plotagem dos dados
figure;
plot(n, heap, 'b', 'LineWidth', 1.5); hold on;
plot(n, lista, 'r', 'LineWidth', 1.5);
legend('Heap', 'Lista Encadeada');
xlabel('Número de Inserções');
ylabel('Número de Comparações');
title('Comparações: Heap vs Lista Encadeada');
grid on;
