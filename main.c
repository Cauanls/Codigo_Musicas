#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    char artista[50];
    char musica[50];
    struct No* prox;
    struct No* ant;
} No;

No* criarNo(char* artista, char* musica) {
    No* novoNo = (No*)malloc(sizeof(No));
    strcpy(novoNo->artista, artista);
    strcpy(novoNo->musica, musica);
    novoNo->prox = NULL;
    novoNo->ant = NULL;
    return novoNo;
}

void inserirNoFinal(No** cabeca, char* artista, char* musica) {
    No* novoNo = criarNo(artista, musica);
    if (*cabeca == NULL) {
        *cabeca = novoNo;
        (*cabeca)->prox = *cabeca;
        (*cabeca)->ant = *cabeca;
    } else {
        No* ultimo = (*cabeca)->ant;
        ultimo->prox = novoNo;
        novoNo->ant = ultimo;
        novoNo->prox = *cabeca;
        (*cabeca)->ant = novoNo;
    }
}

void exibirPlaylist(No* cabeca) {
    if (cabeca == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    No* atual = cabeca;
    do {
        printf("Artista: %s - Música: %s\n", atual->artista, atual->musica);
        atual = atual->prox;
    } while (atual != cabeca);
}

void exibirPlaylistPorNome(No* cabeca) {
    if (cabeca == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    No* atual = cabeca;
    No* listaOrdenada = NULL;

    do {
        No* novoNo = criarNo(atual->artista, atual->musica);

        if (listaOrdenada == NULL) {
            listaOrdenada = novoNo;
        } else {
            No* temp = listaOrdenada;
            No* ant = NULL;
            while (temp != NULL && strcmp(novoNo->musica, temp->musica) > 0) {
                ant = temp;
                temp = temp->prox;
            }

            if (ant == NULL) {
                novoNo->prox = listaOrdenada;
                listaOrdenada->ant = novoNo;
                listaOrdenada = novoNo;
            } else {
                novoNo->prox = ant->prox;
                if (ant->prox != NULL) {
                    ant->prox->ant = novoNo;
                }
                ant->prox = novoNo;
                novoNo->ant = ant;
            }
        }

        atual = atual->prox;
    } while (atual != cabeca);

    exibirPlaylist(listaOrdenada);
}

void removerMusica(No** cabeca, char* musica) {
    if (*cabeca == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    No* atual = *cabeca;
    No* paraExcluir = NULL;
    int encontrada = 0;

    do {
        if (strcmp(atual->musica, musica) == 0) {
            paraExcluir = atual;
            encontrada = 1;
            break;
        }
        atual = atual->prox;
    } while (atual != *cabeca);

    if (encontrada) {
        if (paraExcluir == *cabeca) {
            *cabeca = paraExcluir->prox;
        }

        No* ant = paraExcluir->ant;
        No* prox = paraExcluir->prox;

        ant->prox = prox;
        prox->ant = ant;

        free(paraExcluir);
        printf("Música removida com sucesso.\n");
    } else {
        printf("Música não encontrada na playlist.\n");
    }
}

void buscarMusica(No* cabeca, char* musica) {
    if (cabeca == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    No* atual = cabeca;
    int encontrada = 0;
    do {
        if (strcmp(atual->musica, musica) == 0) {
            printf("Música encontrada:\nArtista: %s - Música: %s\n", atual->artista, atual->musica);
            encontrada = 1;
            break;
        }
        atual = atual->prox;
    } while (atual != cabeca);
    if (!encontrada) {
        printf("Música não encontrada na playlist.\n");
    }
}

int main() {
    No* playlist = NULL;
    FILE* arquivo = fopen("musicas.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo.");
        return 1;
    }

    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo)) {
        char artista[50], musica[50];
        if (sscanf(linha, "%49[^;];%49[^\n]", artista, musica) == 2) {
            inserirNoFinal(&playlist, artista, musica);
        }
    }
    fclose(arquivo);

    int escolha;
    char nomeMusica[50];
    No* musicaAtual = playlist;

    do {
        printf("\nEscolha uma opção:\n");
        printf("1. Exibir playlist pela ordem de cadastro\n");
        printf("2. Exibir playlist ordenada pelo nome das músicas\n");
        printf("3. Inserir nova música\n");
        printf("4. Remover uma música\n");
        printf("5. Buscar por uma música\n");
        printf("6. Avançar para próxima música\n");
        printf("7. Retornar à música anterior\n");
        printf("0. Sair\n");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                exibirPlaylist(playlist);
                break;
            case 2:
                exibirPlaylistPorNome(playlist);
                break;
            case 3:
                printf("Digite o nome do artista: ");
                char novoArtista[50], novaMusica[50];
                scanf("%s", novoArtista);
                printf("Digite o nome da música: ");
                scanf("%s", novaMusica);
                inserirNoFinal(&playlist, novoArtista, novaMusica);
                FILE* arquivo = fopen("musicas.txt", "a");
                fprintf(arquivo, "%s;%s\n", novoArtista, novaMusica);
                fclose(arquivo);
                break;
            case 4:
                printf("Digite o nome da música a ser removida: ");
                scanf("%s", nomeMusica);
                removerMusica(&playlist, nomeMusica);
                FILE* musicas = fopen("musicas.txt", "w");
                No* atual = playlist;
                do {
                    fprintf(arquivo, "%s;%s\n", atual->artista, atual->musica);
                    atual = atual->prox;
                } while (atual != playlist);
                fclose(arquivo);
                break;
            case 5:
                printf("Digite o nome da música a ser buscada: ");
                scanf("%s", nomeMusica);
                buscarMusica(playlist, nomeMusica);
                break;
            case 6:
                if (musicaAtual != NULL) {
                    musicaAtual = musicaAtual->prox;
                    printf("Avançando para a próxima música:\nArtista: %s - Música: %s\n", musicaAtual->artista, musicaAtual->musica);
                } else {
                    printf("A playlist está vazia ou você já está na última música.\n");
                }
                break;
            case 7:
                if (musicaAtual != NULL) {
                    musicaAtual = musicaAtual->ant;
                    printf("Retornando à música anterior:\nArtista: %s - Música: %s\n", musicaAtual->artista, musicaAtual->musica);
                } else {
                    printf("A playlist está vazia ou você já está na primeira música.\n");
                }
                break;
            case 0:
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (escolha != 0);

    No* atual = playlist;
    while (atual != NULL) {
        No* temp = atual;
        atual = atual->prox;
        free(temp);
    }

    return 0;
}
