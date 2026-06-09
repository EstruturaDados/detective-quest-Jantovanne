#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <time.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash

#define TAMANHO_HASH 5

// Estrutura para representar as salas da mansão
typedef struct No
{
    char nome[20];
    char pista[100]; // Pista associada à sala
    struct No *esquerda, *direita;
} No;

// Estrutura para representar as pistas
typedef struct Pista
{
    char texto[100];
    struct Pista *esquerda, *direita;
} Pista;

// Estrutura para representar os suspeitos
typedef struct Suspeito
{
    char nome[20];
    char pistas[5][100]; // Lista de pistas associadas
    int contador; // Contador para saber quantas vezes o suspeito foi citado
    struct Suspeito *proximo; // Ponteiro para próxima suspeito em caso de colisão na hash
} Suspeito;

// Função para criar uma nova sala
void criarSala(No* sala, char nome[20]) {
    strcpy(sala->nome, nome);
    sala->pista[0] = '\0'; // Inicializa a pista como vazia
    sala->esquerda = NULL;
    sala->direita = NULL;
}

// Função para inserir salas na árvore de forma fixa
No* insereSala() {
    char nome[6][20] = {"Hall", "Biblioteca", "Cozinha", "Sótão", "Jardim", "Sala de Estar"};
    No* raiz = NULL;

    // insere os nomes na arvore de forma fixa
    for (int i = 0; i < 6; i++) {

        No* novaSala = (No*)malloc(sizeof(No));
        if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
        }
        criarSala(novaSala, nome[i]);
        if (raiz == NULL) {
            raiz = novaSala;
        } else {
            // inserção por ordem alfabética
            No* atual = raiz;
            while (1) {
                if (strcmp(novaSala->nome, atual->nome) < 0) {
                    if (atual->esquerda == NULL) {
                        atual->esquerda = novaSala;
                        break;
                    } else {
                        atual = atual->esquerda;
                    }
                } else {
                    if (atual->direita == NULL) {
                        atual->direita = novaSala;
                        break;
                    } else {
                        atual = atual->direita;
                    }
                }
            }
        }
    }

    return raiz;
}

// Função para navegar pela arvore
void imprimirArvore(No* sala, int nivel, char prefixo) {
    if (sala == NULL && nivel == 0) {
        printf("Arvore vazia.\n");
        return;
    } else if (sala == NULL) {
        return;
    }
    if (nivel == 0) {
        printf("---------------\n");
        printf("Mapa da Mansão:\n");
        printf("---------------\n");
    }
    for (int i = 0; i < nivel; i++) {
        printf("        ");
    }
    if (nivel == 0){
        printf("Raiz: %s\n", sala->nome);
    } else {
        printf("%c - %s\n", prefixo, sala->nome);
    }
    imprimirArvore(sala->esquerda, nivel + 1, 'E');
    imprimirArvore(sala->direita, nivel + 1, 'D');

}

// Função para criar pistas
Pista* criarPista(char texto[100]) {
    Pista* novaPista = (Pista*)malloc(sizeof(Pista));
    if (novaPista == NULL) {
        printf("Erro ao alocar memória para a pista.\n");
        exit(1);
    }
    strcpy(novaPista->texto, texto);
    novaPista->esquerda = NULL;
    novaPista->direita = NULL;
    return novaPista;
}

// Função para inserir pistas na árvore de busca
Pista* inserirBST(Pista* raiz, char texto[100], No* salaAtual) {
    if (raiz == NULL) {
        strcpy(salaAtual->pista, texto); // Associa a pista à sala atual
        return criarPista(texto);
    }
    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirBST(raiz->esquerda, texto, salaAtual);
    } else if (strcmp(texto, raiz->texto) > 0) {
        raiz->direita = inserirBST(raiz->direita, texto, salaAtual);
    }
    return raiz;
}

// Função para exibir as pistas em ordem alfabética
void emOrdem(Pista* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("%s\n", raiz->texto);
        emOrdem(raiz->direita);
    }
}

// Função para verificar se uma pista já existe na árvore
bool verificarPista(Pista* raiz, char texto[100]) {
    if (raiz == NULL) {
        return false;
    }
    if (strcmp(texto, raiz->texto) == 0) {
        return true;
    }
    if (strcmp(texto, raiz->texto) < 0) {
        return verificarPista(raiz->esquerda, texto);
    } else {
        return verificarPista(raiz->direita, texto);
    }
}

// Função para verificar se a árvore de pista está cheia
int contarPistas(Pista* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return 1 + contarPistas(raiz->esquerda) + contarPistas(raiz->direita);
}

// Função para criar um novo suspeito
Suspeito* criarSuspeito(char nome[20]) {
    Suspeito* novoSuspeito = (Suspeito*)malloc(sizeof(Suspeito));
    if (novoSuspeito == NULL) {
        printf("Erro ao alocar memória para o suspeito.\n");
        exit(1);
    }
    strcpy(novoSuspeito->nome, nome);
    novoSuspeito->contador = 0;
    for (int i = 0; i < 5; i++) {
        novoSuspeito->pistas[i][0] = '\0'; // Inicializa as pistas como vazias
    }
    novoSuspeito->proximo = NULL;
    return novoSuspeito;
}

// Função para calcular o índice da tabela hash
int calcularHash(char nome[20]) {
    int soma = 0;
    for (int i = 0; nome[i] != '\0'; i++) {
        soma += nome[i]; // Soma dos valores ASCII dos caracteres do nome
    }
    return soma % TAMANHO_HASH; // Retorna o índice da tabela hash
}

// Função para buscar um suspeito existente
Suspeito* buscarSuspeito(char nome[20], Suspeito* tabelaHash[TAMANHO_HASH]) {
    int indice = calcularHash(nome);
    Suspeito* atual = tabelaHash[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual; // Achou o suspeito
        }
        atual = atual->proximo;
    }
    return NULL; // Não existe na tabela ainda
}

// Função para inserir um suspeito na tabela hash
void inserirHash(char nomeSuspeito[20], char pista[100], Suspeito* tabelaHash[TAMANHO_HASH]) {
    //Procura se esse suspeito já foi criado antes
    Suspeito* suspeitoExistente = buscarSuspeito(nomeSuspeito, tabelaHash);

    if (suspeitoExistente != NULL) {
            // Se já existe, apenas adiciona a nova pista no vetor dele (se houver espaço)
            if (suspeitoExistente->contador < 5) {
                strcpy(suspeitoExistente->pistas[suspeitoExistente->contador], pista);
                suspeitoExistente->contador++;
            } 
        } else{
        int indice = calcularHash(nomeSuspeito);
        Suspeito* novoSuspeito = criarSuspeito(nomeSuspeito);
        strcpy(novoSuspeito->pistas[novoSuspeito->contador], pista); // Associa a pista ao suspeito
        novoSuspeito->contador++; // Incrementa o contador de pistas associadas

        if (tabelaHash[indice] == NULL) {
            tabelaHash[indice] = novoSuspeito; // Insere o suspeito na tabela hash
        } else {
            // Tratamento de colisão: adiciona o suspeito à lista encadeada
            Suspeito* atual = tabelaHash[indice];
            while (atual->proximo != NULL) {
                atual = atual->proximo;
            }
            atual->proximo = novoSuspeito; // Adiciona o novo suspeito ao final da lista
        }
    }
}

// Função para listar todos os suspeitos e suas respectivas pistas
void listarAssociacoes(Suspeito* tabelaHash[TAMANHO_HASH]) {
    printf("==============================================\n");
    printf("Listando suspeitos e suas pistas associadas:\n");
    printf("==============================================\n");
    for (int i = 0; i < TAMANHO_HASH; i++) {
        Suspeito* atual = tabelaHash[i];
        while (atual != NULL) {
            printf("Suspeito: %s\n", atual->nome);
            printf("Pistas associadas:");
            for (int j = 0; j < atual->contador; j++) {
                if (j == 0) {
                    printf(" %s", atual->pistas[j]);
                } else {
                printf(", %s", atual->pistas[j]);
            }
        }
        printf("\n------------------------------\n");
        atual = atual->proximo; // Move para o próximo suspeito na lista
    }
}
}

// Função para encontrar o suspeito com mais pistas associadas ou aleatoriamente em caso de empate
void encontrarSuspeitoProvavel(Suspeito* tabelaHash[TAMANHO_HASH]) {
    Suspeito* suspeitoProvavel = NULL;
    int maxPistas = 0;
    int quantEmpatados = 1; // Contador para o algoritmo de amostragem reservatório

    for (int i = 0; i < TAMANHO_HASH; i++) {
        Suspeito* atual = tabelaHash[i];
        while (atual != NULL) {
            if (atual->contador > maxPistas) {
                maxPistas = atual->contador;
                suspeitoProvavel = atual;
                quantEmpatados = 1; // Reinicia o contador de empates
            } else if (atual->contador == maxPistas && maxPistas > 0) {
                quantEmpatados++;
                // Sorteia um entre os suspeitos empatados
                if (rand() % quantEmpatados == 0) {
                    suspeitoProvavel = atual;
                }
            }
            atual = atual->proximo;
        }
    }

    if (suspeitoProvavel != NULL) {
        printf("===================================================================\n");
        printf("O suspeito mais provável é: %s com %d pistas associadas.\n", suspeitoProvavel->nome, maxPistas);
        printf("====================================================================\n");
    } else {
        printf("Nenhum suspeito foi associado a pistas.\n");
    }
}


// Função para sortear uma pista e associar a uma sala e a um suspeito
Pista* associarPista(Pista* raizPista, Suspeito* tabelaHash[TAMANHO_HASH], No* salaAtual) {
    char pistas[5][100] = {
        "Pegada de sapato",
        "Mancha de sangue",
        "Carta rasgada",
        "Objeto quebrado",
        "Testemunha ocular"
    };
    char suspeitos[5][20] = {
        "Joao",
        "Ana",
        "Carlos",
        "Maria",
        "Pedro"
    };

    if (contarPistas(raizPista) >= 5) {
        printf("Todas as pistas já foram coletadas.\n");
        return raizPista;
    }
    int indice = rand() % 5;
    int indiceSuspeito = rand() % 5;
    // verifica se a sala já teve uma pista coletada
    if (salaAtual->pista[0] != '\0') {
        printf("Você já coletou uma pista nesta sala.\n");
        return raizPista;
    }
    if (verificarPista(raizPista, pistas[indice])) {
        return associarPista(raizPista, tabelaHash, salaAtual); // sorteia outra pista se já existir ou se a raiz for nula.
    } else{
    raizPista = inserirBST(raizPista, pistas[indice], salaAtual);
    inserirHash(suspeitos[indiceSuspeito], pistas[indice], tabelaHash);
    printf("Pista coletada: %s\n", pistas[indice]);

    }
    return raizPista;
}



void menu(No* sala, Pista* raizPista, Suspeito* tabelaHash[TAMANHO_HASH]) {

    No* salaNavegada = sala;
    int opcao;

    do
    {
        printf("------------\n");
        printf("Menu:\n");
        printf("------------\n");
        printf("1 - ir para a esquerda\n");
        printf("2 - ir para a direita\n");
        printf("3 - voltar para o Hall\n");
        printf("4 - revisar pistas coletadas\n");
        printf("5 - listar suspeitos e suas pistas associadas\n");
        printf("6 - encontrar suspeito mais provável\n");
        printf("0 - sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
            if (salaNavegada->esquerda != NULL) {
                salaNavegada = salaNavegada->esquerda;
                printf("Você está na sala: %s\n", salaNavegada->nome);
                raizPista = associarPista(raizPista, tabelaHash, salaNavegada);
                imprimirArvore(sala, 0, 'R');
            } else {
                printf("Não há sala à esquerda.\n");
            }
            break;
        case 2:
            if (salaNavegada->direita != NULL) {
                salaNavegada = salaNavegada->direita;
                printf("Você está na sala: %s\n", salaNavegada->nome);
                raizPista = associarPista(raizPista, tabelaHash, salaNavegada);
                imprimirArvore(sala, 0, 'R');
            } else {
                printf("Não há sala à direita.\n");
            }
            break;
        case 3:
            salaNavegada = sala;
            printf("Você voltou para o Hall.\n");
            imprimirArvore(sala, 0, 'R');
            break;
        case 4:
            printf("======================\n");
            printf("Pistas coletadas:\n");
            printf("======================\n");
            emOrdem(raizPista);
            printf("-----------------------\n");
            imprimirArvore(sala, 0, 'R');
            break;
        case 5:
            listarAssociacoes(tabelaHash);
            break;
        case 6:
            encontrarSuspeitoProvavel(tabelaHash);
            break;
        default:
            printf("Opção inválida.\n");
            break;
        }
    } while (opcao != 0);
}

int main() {

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    setlocale(LC_ALL,  ".UTF-8");
    srand(time(NULL));

    No* sala = insereSala();
    Pista* raizPista = NULL;
    Suspeito* tabelaHash[TAMANHO_HASH] = {NULL}; // Inicializa a tabela hash de suspeitos

    printf("=================================\n");
    printf("Bem vindo ao DETETIVE QUEST\n");
    printf("=================================\n\n");
    imprimirArvore(sala, 0, 'R');
    menu(sala, raizPista, tabelaHash);
    free(sala);
    free(raizPista);

    return 0;
}