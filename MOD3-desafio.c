#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR 15
#define MAX_MISSAO 150

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// Vetor de missões pré-definidas (pelo menos 5)
const char* missoes[] = {
    "Conquistar todos os territorios da cor Azul",
    "Controlar pelo menos 12 territorios",
    "Eliminar completamente o jogador Vermelho",
    "Conquistar todos os territorios da cor Verde e Amarelo",
    "Controlar pelo menos 8 territorios com 3 ou mais tropas cada",
    "Destruir todas as tropas do jogador Preto"
};
#define TOTAL_MISSOES 6

// Protótipos
void cadastrarTerritorios(Territorio* mapa, int quantidade);
void exibirMapa(const Territorio* mapa, int quantidade);
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes);
void exibirMissao(const char* missao); // passagem por valor
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa, char* missaoJogador);
int encontrarTerritorioPorNome(const Territorio* mapa, int quantidade, const char* nome);
int contarTerritoriosPorCor(const Territorio* mapa, int tamanho, const char* cor);
int contarTerritoriosComMinimoTropas(const Territorio* mapa, int tamanho, int minimo);

int main() {
    srand(time(NULL));

    int quantidade;
    printf("=== SIMULADOR ESTRATEGICO COM MISSOES SECRETAS ===\n\n");
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &quantidade);
    getchar(); // limpa \n

    if (quantidade <= 0) {
        printf("Quantidade invalida!\n");
        return 1;
    }

    // Alocação dinâmica do mapa
    Territorio* mapa = (Territorio*) calloc(quantidade, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para o mapa!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, quantidade);

    // Alocação dinâmica da missão do jogador (apenas 1 jogador no modo simples)
    char* missaoJogador = (char*) malloc(MAX_MISSAO * sizeof(char));
    if (missaoJogador == NULL) {
        printf("Erro ao alocar memoria para a missao!\n");
        free(mapa);
        return 1;
    }

    // Sorteio e atribuição da missão secreta
    atribuirMissao(missaoJogador, missoes, TOTAL_MISSOES);
    printf("\n=== MISSAO SECRETA ATRIBUIDA ===\n");
    exibirMissao(missaoJogador); // exibida apenas uma vez
    printf("\nBoa sorte! Sua missao e secreta e so sera verificada ao final de cada turno.\n");

    int turno = 1;
    char opcao;

    do {
        printf("\n==================================================\n");
        printf("                   TURNO %d                        \n", turno++);
        printf("==================================================\n");

        exibirMapa(mapa, quantidade);

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
            printf("Precisa de pelo menos 2 tropas para atacar!\n");
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
            printf("Nao pode atacar a si mesmo!\n");
            continue;
        }

        if (strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
            printf("Nao pode atacar territorio da mesma cor!\n");
            continue;
        }

        atacar(&mapa[idxAtacante], &mapa[idxDefensor]);

        // Verificação silenciosa da missão ao final do turno
        if (verificarMissao(missaoJogador, mapa, quantidade)) {
            printf("\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("!!! PARABENS! VOCE CUMPRIU SUA MISSAO SECRETA !!!\n");
            printf("!!! MISSAO: %s                          !!!\n", missaoJogador);
            printf("!!! VOCE E O VENCEDOR!!!\n");
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
            break;
        } else {
            printf("\nMissao ainda nao cumprida. Continue lutando!\n");
        }

        printf("\nPressione ENTER para o proximo turno...");
        getchar();

    } while (1);

    printf("\n=== MAPA FINAL ===\n");
    exibirMapa(mapa, quantidade);

    liberarMemoria(mapa, missaoJogador);

    printf("\nJogo encerrado. Memoria liberada com seguranca.\n");
    return 0;
}

// Cadastro com cor automática
void cadastrarTerritorios(Territorio* mapa, int quantidade) {
    const char* cores[] = {"Vermelho", "Azul", "Verde", "Amarelo", "Preto", "Branco", "Roxo", "Laranja"};
    int numCores = 8;

    printf("\n=== CADASTRO DE TERRITORIOS ===\n");
    for (int i = 0; i < quantidade; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("  Nome: ");
        fgets(mapa[i].nome, MAX_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        strcpy(mapa[i].cor, cores[i % numCores]);
        printf("  Cor atribuida: %s\n", mapa[i].cor);

        do {
            printf("  Quantidade de tropas: ");
            scanf("%d", &mapa[i].tropas);
            getchar();
            if (mapa[i].tropas < 1) printf("Minimo 1 tropa!\n");
        } while (mapa[i].tropas < 1);
    }
}

void exibirMapa(const Territorio* mapa, int quantidade) {
    printf("\nLista de Territorios:\n");
    printf("%-25s %-12s %s\n", "Nome", "Cor", "Tropas");
    printf("------------------------------------------------\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%-25s %-12s %d\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Sorteia e copia a missão (passagem por referência no destino)
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Exibe missão (passagem por valor)
void exibirMissao(const char* missao) {
    printf("Sua missao secreta e:\n");
    printf(">>> %s <<<\n", missao);
}

// Verifica se a missão foi cumprida (lógica simples por palavras-chave)
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho) {
    if (strstr(missao, "todos os territorios da cor Azul")) {
        return contarTerritoriosPorCor(mapa, tamanho, "Azul") == 0; // jogador conquistou todos → não sobrou nenhum Azul
    }
    if (strstr(missao, "pelo menos 12 territorios")) {
        int total = 0;
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas > 0) total++; // conta territórios ocupados (qualquer cor, mas no contexto é do jogador)
        }
        return total >= 12;
    }
    if (strstr(missao, "Eliminar completamente o jogador Vermelho")) {
        return contarTerritoriosPorCor(mapa, tamanho, "Vermelho") == 0;
    }
    if (strstr(missao, "Verde e Amarelo")) {
        return contarTerritoriosPorCor(mapa, tamanho, "Verde") == 0 &&
               contarTerritoriosPorCor(mapa, tamanho, "Amarelo") == 0;
    }
    if (strstr(missao, "8 territorios com 3 ou mais tropas")) {
        return contarTerritoriosComMinimoTropas(mapa, tamanho, 3) >= 8;
    }
    if (strstr(missao, "todas as tropas do jogador Preto")) {
        return contarTerritoriosPorCor(mapa, tamanho, "Preto") == 0;
    }
    return 0; // missão não reconhecida ou não cumprida
}

void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n=== BATALHA ===\n");
    printf("Atacante (%s): %d tropas vs Defensor (%s): %d tropas\n",
           atacante->cor, atacante->tropas, defensor->cor, defensor->tropas);

    int dadoAtq = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtq, dadoDef);

    if (dadoAtq > dadoDef) {
        printf(">>> Atacante vence!\n");
        // Conquista: transfere cor e metade das tropas
        strcpy(defensor->cor, atacante->cor);
        int transferidas = (atacante->tropas - 1) / 2;
        if (transferidas < 1) transferidas = 1;
        defensor->tropas = transferidas;
        atacante->tropas -= transferidas;
        printf("Conquistado! %d tropas transferidas.\n", transferidas);
    } else {
        printf(">>> Defensor resiste! Atacante perde 1 tropa.\n");
        atacante->tropas--;
        if (atacante->tropas < 1) atacante->tropas = 1;
    }
}

void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    free(mapa);
    free(missaoJogador);
    mapa = NULL;
    missaoJogador = NULL;
}

int encontrarTerritorioPorNome(const Territorio* mapa, int quantidade, const char* nome) {
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(mapa[i].nome, nome) == 0) return i;
    }
    return -1;
}

int contarTerritoriosPorCor(const Territorio* mapa, int tamanho, const char* cor) {
    int count = 0;
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, cor) == 0 && mapa[i].tropas > 0) count++;
    }
    return count;
}

int contarTerritoriosComMinimoTropas(const Territorio* mapa, int tamanho, int minimo) {
    int count = 0;
    for (int i = 0; i < tamanho; i++) {
        if (mapa[i].tropas >= minimo) count++;
    }
    return count;
}