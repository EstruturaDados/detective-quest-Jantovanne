#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <time.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash


// Estrutura para representar as salas da mansão
typedef struct No
{
    char nome[20];
    struct No *esquerda, *direita;
} No;

// Estrutura para representar as pistas
typedef struct Pista
{
    char texto[100];
    struct Pista *esquerda, *direita;
} Pista;

// Função para criar uma nova sala
void criarSala(No* sala, char nome[20]) {
    strcpy(sala->nome, nome);
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
Pista* inserirBST(Pista* raiz, char texto[100]) {
    if (raiz == NULL) {
        return criarPista(texto);
    }
    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirBST(raiz->esquerda, texto);
    } else if (strcmp(texto, raiz->texto) > 0) {
        raiz->direita = inserirBST(raiz->direita, texto);
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

// Função para sortear uma pista e associar a uma sala
Pista* associarPista(Pista* raizPista) {
    char pistas[5][100] = {
        "Pegada de sapato",
        "Mancha de sangue",
        "Carta rasgada",
        "Objeto quebrado",
        "Testemunha ocular"
    };
    if (contarPistas(raizPista) >= 5) {
        printf("Todas as pistas já foram coletadas.\n");
        return raizPista;
    }
    int indice = rand() % 5;
if (verificarPista(raizPista, pistas[indice])) {
        associarPista(raizPista); // sorteia outra pista se já existir ou se a raiz for nula.
    } else{
    raizPista = inserirBST(raizPista, pistas[indice]);
    printf("Pista coletada: %s\n", pistas[indice]);
    }
    return raizPista;
}



void menu(No* sala, Pista* raizPista) {

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
        printf("0 - sair\n");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
            if (salaNavegada->esquerda != NULL) {
                salaNavegada = salaNavegada->esquerda;
                printf("Você está na sala: %s\n", salaNavegada->nome);
                raizPista = associarPista(raizPista);
                imprimirArvore(sala, 0, 'R');
            } else {
                printf("Não há sala à esquerda.\n");
            }
            break;
        case 2:
            if (salaNavegada->direita != NULL) {
                salaNavegada = salaNavegada->direita;
                printf("Você está na sala: %s\n", salaNavegada->nome);
                raizPista = associarPista(raizPista);
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
        default:
            printf("Opção inválida.\n");
            break;
        }
    } while (opcao != 0);
}

int main() {

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    setlocale(LC_ALL,  ".UTF-8");
    srand(time(NULL));

    No* sala = insereSala();
    Pista* raizPista = NULL;

    printf("=================================\n");
    printf("Bem vindo ao DETETIVE QUEST\n");
    printf("=================================\n\n");
    imprimirArvore(sala, 0, 'R');
    menu(sala, raizPista);

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    return 0;
}

