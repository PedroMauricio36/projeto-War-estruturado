#include <stdio.h>
#include <string.h>

// Definição da estrutura Territorio
// Representa um território com nome, cor do exército e quantidade de tropas
typedef struct {
    char nome[30];    // Nome do território (máximo 29 caracteres + '\0')
    char cor[10];     // Cor do exército que controla o território
    int tropas;       // Quantidade de tropas estacionadas
} Territorio;

int main() {
    // Vetor que armazena até 5 territórios
    Territorio territorios[5];

    printf("==========================================\n");
    printf("   CADASTRO DE TERRITORIOS - WAR GAME\n");
    printf("==========================================\n\n");

    // Laço para cadastrar os 5 territórios
    for (int i = 0; i < 5; i++) {
        printf("--- Territorio %d de 5 ---\n", i + 1);

        // Limpa o buffer do teclado (importante após usar scanf com strings)
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        printf("Digite o nome do territorio: ");
        fgets(territorios[i].nome, 30, stdin);
        // Remove o '\n' que fgets inclui
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Digite a cor do exercito (ex: Vermelho, Azul): ");
        fgets(territorios[i].cor, 10, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);

        printf("\nTerritorio cadastrado com Sucesso!\n\n");
    }

    // Exibição de todos os territórios cadastrados
    printf("==========================================\n");
    printf("     TERRITORIOS CADASTRADOS\n");
    printf("==========================================\n");

    for (int i = 0; i < 5; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("  Nome   : %s\n", territorios[i].nome);
        printf("  Cor    : %s\n", territorios[i].cor);
        printf("  Tropas : %d\n", territorios[i].tropas);
        printf("------------------------\n");
    }

    printf("Fim do programa. Todos os 5 territorios foram cadastrados e exibidos.\n\n");

    return 0;
}