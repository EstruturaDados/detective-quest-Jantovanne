#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.


typedef struct No
{
    char nome[20];
    struct No *esquerda, *direita;
} No;

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




void menu(No* sala) {

    No* salaNavegada = sala;
    int opcao;

    do
    {
        printf("------------\n");
        printf("Menu:\n");
        printf("------------\n");
        printf("1 - ir para a esquerda (e)\n");
        printf("2 - ir para a direita (d)\n");
        printf("0 - sair (s)\n");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
            if (salaNavegada->esquerda != NULL) {
                salaNavegada = salaNavegada->esquerda;
                printf("Você está na sala: %s\n", salaNavegada->nome);
            } else {
                printf("Não há sala à esquerda.\n");
            }
            break;
        case 2:
            if (salaNavegada->direita != NULL) {
                salaNavegada = salaNavegada->direita;
                printf("Você está na sala: %s\n", salaNavegada->nome);
            } else {
                printf("Não há sala à direita.\n");
            }
            break;
        default:
            printf("Opção inválida.\n");
            break;
        }
    } while (opcao != 0);
}

int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    setlocale(LC_ALL,  ".UTF-8");

    No* sala = insereSala();

    printf("=================================\n");
    printf("Bem vindo ao DETETIVE QUEST\n");
    printf("=================================\n\n");
    imprimirArvore(sala, 0, 'R');
    menu(sala);


    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

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

