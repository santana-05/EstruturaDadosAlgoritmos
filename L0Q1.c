#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINHA 500
#define MAX_PONTOS 300

typedef struct {
    int x;
    int y;
} Coordenada;

float calcularDistanciaOrigem(Coordenada pt) {
    return sqrt(pt.x * pt.x + pt.y * pt.y);
}

float calcularDistancia(Coordenada a, Coordenada b) {
    int deltaX = a.x - b.x;
    int deltaY = a.y - b.y;
    return sqrt(deltaX * deltaX + deltaY * deltaY);
}

int parsearPontos(const char *texto, Coordenada vetor[]) {
    int qtd = 0;
    int x, y, deslocamento;
    const char *atual = texto;

    while (*atual) {
        if (*atual == '(' ) {
            if (sscanf(atual, "(%d,%d)%n", &x, &y, &deslocamento) == 2 ||
                sscanf(atual, " (%d,%d)%n", &x, &y, &deslocamento) == 2 ||
                sscanf(atual, "( %d , %d )%n", &x, &y, &deslocamento) == 2 ||
                sscanf(atual, " ( %d , %d ) %n", &x, &y, &deslocamento) == 2) {
                vetor[qtd].x = x;
                vetor[qtd].y = y;
                qtd++;
                atual += deslocamento;
                continue;
            }
        }
        atual++;
    }

    return qtd;
}

int compararPorOrigem(const void *p1, const void *p2) {
    Coordenada *a = (Coordenada *)p1;
    Coordenada *b = (Coordenada *)p2;
    float da = calcularDistanciaOrigem(*a);
    float db = calcularDistanciaOrigem(*b);
    return (da > db) - (da < db);
}

void escreverResultado(FILE *arq, Coordenada ordenados[], int total, float percurso, float atalho) {
    fprintf(arq, "points");
    for (int i = 0; i < total; i++) {
        fprintf(arq, " (%d,%d)", ordenados[i].x, ordenados[i].y);
    }
    fprintf(arq, " distance %.2f shortcut %.2f\n", percurso, atalho);
}

int main() {
    FILE *entrada = fopen("L0Q1.in", "r");
    FILE *saida = fopen("L0Q1.out", "w");

    if (!entrada || !saida) {
        perror("Falha ao abrir arquivos");
        return 1;
    }

    char buffer[MAX_LINHA];
    Coordenada lista[MAX_PONTOS];
    Coordenada ordenados[MAX_PONTOS];

    while (fgets(buffer, sizeof(buffer), entrada)) {
        size_t tam = strlen(buffer);
        if (tam > 0 && buffer[tam - 1] == '\n') {
            buffer[tam - 1] = '\0';
        }

        int vazia = 1;
        for (size_t i = 0; i < tam; i++) {
            if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\r') {
                vazia = 0;
                break;
            }
        }
        if (vazia || buffer[0] == '\0') continue;

        int totalPontos = parsearPontos(buffer, lista);
        if (totalPontos == 0) {
            fprintf(saida, "points distance 0.00 shortcut 0.00\n");
            continue;
        }

        memcpy(ordenados, lista, totalPontos * sizeof(Coordenada));
        qsort(ordenados, totalPontos, sizeof(Coordenada), compararPorOrigem);

        float distanciaPercorrida = 0;
        for (int i = 0; i < totalPontos - 1; i++) {
            distanciaPercorrida += calcularDistancia(lista[i], lista[i + 1]);
        }

        float atalho = calcularDistancia(lista[0], lista[totalPontos - 1]);

        escreverResultado(saida, ordenados, totalPontos, distanciaPercorrida, atalho);
    }

    fclose(entrada);
    fclose(saida);

    return 0;
}