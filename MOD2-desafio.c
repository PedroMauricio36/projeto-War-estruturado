#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR 15  // Aumentado para caber "Laranja"

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// Lista de cores pré-definidas
const char* cores[] = {
    "Vermelho", "Azul", "Verde", "Amarelo",
    "Preto", "Branco", "Roxo", "Laranja"
};
#define NUM_CORES 8

// Protótipos
void cadastrarTerritorios(Territorio* mapa, int quantidade);
void exibirTerritorios(Territorio* mapa, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);
int encontrarTerritorioPorNome(Territorio* mapa, int quantidade, const char* nome);

int main() {
    srand(time(NULL));

    int quantidade;
    printf("=== SIMULADOR DE BATALHA DE TERRITÓRIOS ===\n\n");
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &quantidade);
    getchar();

    if (quantidade <= 0) {
        printf("Quantidade invalida!\n");
        return 1;
    }

    Territorio* mapa = (Territorio*) calloc(quantidade, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, quantidade);
    
    int turno = 1;
    char opcao;

    do {
        printf("\n==================================================\n");
        printf("                   TURNO %d                        \n", turno++);
        printf("==================================================\n");

        exibirTerritorios(mapa, quantidade);

        printf("\nDeseja realizar um ataque neste turno? (s/n): ");
        scanf(" %c", &opcao);
        getchar();

        if (opcao == 'n' || opcao == 'N') {
            break;
        }

        char nomeAtacante[MAX_NOME], nomeDefensor[MAX_NOME];
        int idxAtacante, idxDefensor;

        printf("\nDigite o nome do territorio ATACANTE: ");
        fgets(nomeAtacante, MAX_NOME, stdin);
        nomeAtacante[strcspn(nomeAtacante, "\n")] = '\0';

        idxAtacante = encontrarTerritorioPorNome(mapa, quantidade, nomeAtacante);
        if (idxAtacante == -1) {
            printf("Territorio atacante nao encontrado!\n");
            continue;
        }

        if (mapa[idxAtacante].tropas < 2) {
            printf("O territorio atacante precisa ter pelo menos 2 tropas para atacar!\n");
            continue;
        }

        printf("Digite o nome do territorio DEFENSOR: ");
        fgets(nomeDefensor, MAX_NOME, stdin);
        nomeDefensor[strcspn(nomeDefensor, "\n")] = '\0';

        idxDefensor = encontrarTerritorioPorNome(mapa, quantidade, nomeDefensor);
        if (idxDefensor == -1) {
            printf("Territorio defensor nao encontrado!\n");
            continue;
        }

        if (idxAtacante == idxDefensor) {
            printf("Um territorio nao pode atacar a si mesmo!\n");
            continue;
        }

        if (strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
            printf("Voce nao pode atacar um territorio da mesma cor!\n");
            continue;
        }

        atacar(&mapa[idxAtacante], &mapa[idxDefensor]);

        printf("\n=== FIM DO ATAQUE - ESTADO ATUAL ===\n");
        printf("Atacante: %s (%s) - Tropas restantes: %d\n", 
               mapa[idxAtacante].nome, mapa[idxAtacante].cor, mapa[idxAtacante].tropas);
        printf("Defensor: %s (%s) - Tropas restantes: %d\n", 
               mapa[idxDefensor].nome, mapa[idxDefensor].cor, mapa[idxDefensor].tropas);

        printf("\nPressione ENTER para continuar ao proximo turno...");
        getchar();

    } while (1);

    printf("\n=== MAPA FINAL APOS TODOS OS TURNOS ===\n");
    exibirTerritorios(mapa, quantidade);

    liberarMemoria(mapa);

    printf("\nJogo encerrado. Memoria liberada.\n");
    return 0;
}

// FUNÇÃO ATUALIZADA: agora atribui cor automaticamente
void cadastrarTerritorios(Territorio* mapa, int quantidade) {
    printf("\n=== CADASTRO DE TERRITORIOS ===\n");
    for (int i = 0; i < quantidade; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("  Nome: ");
        fgets(mapa[i].nome, MAX_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        // Atribui cor automaticamente
        strcpy(mapa[i].cor, cores[i % NUM_CORES]);
        printf("  Cor atribuida automaticamente: %s\n", mapa[i].cor);

        do {
            printf("  Quantidade de tropas: ");
            scanf("%d", &mapa[i].tropas);
            getchar(); // limpa o \n
            if (mapa[i].tropas < 1) {
                printf("Um territorio deve ter pelo menos 1 tropa!\n");
            }
        } while (mapa[i].tropas < 1);
    }
}

void exibirTerritorios(Territorio* mapa, int quantidade) {
    printf("\nLista de Territorios:\n");
    printf("%-25s %-12s %s\n", "Nome", "Cor", "Tropas");
    printf("------------------------------------------------\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%-25s %-12s %d\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n=== INICIO DO ATAQUE ===\n");
    printf("Atacante: %s (%s) com %d tropas\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("Defensor: %s (%s) com %d tropas\n\n", defensor->nome, defensor->cor, defensor->tropas);

    int rodada = 1;
    char continuar;

    while (atacante->tropas >= 2 && defensor->tropas > 0) {
        int dadosAtaque = (atacante->tropas >= 3) ? 2 : 1;
        int dadosDefesa = (defensor->tropas >= 2) ? 2 : 1;

        int dadoAtaque[2] = {0, 0};
        int dadoDefesa[2] = {0, 0};

        printf("Rodada %d:\n", rodada++);
        printf("  Atacante rola %d dado(s): ", dadosAtaque);
        for (int i = 0; i < dadosAtaque; i++) {
            dadoAtaque[i] = (rand() % 6) + 1;
            printf("%d ", dadoAtaque[i]);
        }

        printf("\n  Defensor rola %d dado(s): ", dadosDefesa);
        for (int i = 0; i < dadosDefesa; i++) {
            dadoDefesa[i] = (rand() % 6) + 1;
            printf("%d ", dadoDefesa[i]);
        }
        printf("\n");

        // Ordena os dados em ordem decrescente
        if (dadosAtaque == 2 && dadoAtaque[0] < dadoAtaque[1]) {
            int temp = dadoAtaque[0];
            dadoAtaque[0] = dadoAtaque[1];
            dadoAtaque[1] = temp;
        }
        if (dadosDefesa == 2 && dadoDefesa[0] < dadoDefesa[1]) {
            int temp = dadoDefesa[0];
            dadoDefesa[0] = dadoDefesa[1];
            dadoDefesa[1] = temp;
        }

        // Comparação com desempate pelo segundo dado
        int vencedor = 0;

        if (dadoAtaque[0] > dadoDefesa[0]) {
            vencedor = 1;
        } else if (dadoAtaque[0] < dadoDefesa[0]) {
            vencedor = -1;
        } else {
            int segundoA = (dadosAtaque == 2) ? dadoAtaque[1] : 0;
            int segundoD = (dadosDefesa == 2) ? dadoDefesa[1] : 0;

            if (segundoA > segundoD) {
                vencedor = 1;
            } else {
                vencedor = -1;
            }
        }

        if (vencedor == 1) {
            int dano = 10 * dadoAtaque[0];
            printf("  >>> Atacante vence a rodada! (dados ordenados: %d,%d vs %d,%d) Causa %d de dano.\n",
                   dadoAtaque[0], dadoAtaque[1], dadoDefesa[0], dadoDefesa[1], dano);
            defensor->tropas -= dano;
            if (defensor->tropas < 0) defensor->tropas = 0;
        } else {
            int dano = 5 * dadoDefesa[0];
            printf("  >>> Defensor vence a rodada! (dados ordenados: %d,%d vs %d,%d) Causa %d de dano.\n",
                   dadoAtaque[0], dadoAtaque[1], dadoDefesa[0], dadoDefesa[1], dano);
            atacante->tropas -= dano;
            if (atacante->tropas < 1) atacante->tropas = 1;
        }

        printf("\n  === SITUAÇÃO ATUAL ===\n");
        printf("  Tropas do Atacante (%s): %d\n", atacante->nome, atacante->tropas);
        printf("  Tropas do Defensor (%s): %d\n\n", defensor->nome, defensor->tropas);

        if (atacante->tropas < 2 || defensor->tropas <= 0) {
            break;
        }

        printf("Deseja continuar o ataque? (s/n): ");
        scanf(" %c", &continuar);
        getchar();

        if (continuar == 'n' || continuar == 'N') {
            printf(">>> Ataque interrompido pelo jogador.\n");
            break;
        }
    }

    if (defensor->tropas <= 0) {
        printf("\n>>> CONQUISTA REALIZADA! %s agora pertence ao exercito %s!\n", 
               defensor->nome, atacante->cor);
        strcpy(defensor->cor, atacante->cor);

        int tropasTransferidas = (atacante->tropas - 1) / 2;
        if (tropasTransferidas < 1) tropasTransferidas = 1;

        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;

        printf("Transferidas %d tropas para o territorio conquistado.\n", tropasTransferidas);
    } else {
        printf("\n>>> Ataque finalizado. O territorio %s se manteve sob controle %s.\n",
               defensor->nome, defensor->cor);
    }
}

int encontrarTerritorioPorNome(Territorio* mapa, int quantidade, const char* nome) {
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(mapa[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
    mapa = NULL;
}