// Maurício Santana e Daniel Santana

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define TAM_STR 500
#define TAM_COORD 300

typedef struct {
    int x, y;
} Ponto;

float dist_origem(Ponto p) {
    return sqrt(p.x * p.x + p.y * p.y);
}

float dist_entre(Ponto a, Ponto b) {
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    return sqrt(dx * dx + dy * dy);
}

int extrair_pontos(const char *linha, Ponto pontos[]) {
    int x, y, n = 0, offset;
    const char *ptr = linha;

    while (*ptr) {
        if (*ptr == '(') {
            if (sscanf(ptr, " ( %d , %d ) %n", &x, &y, &offset) == 2 ||
                sscanf(ptr, "(%d,%d)%n", &x, &y, &offset) == 2 ||
                sscanf(ptr, " (%d ,%d) %n", &x, &y, &offset) == 2 ||
                sscanf(ptr, " (%d,%d) %n", &x, &y, &offset) == 2) {
                pontos[n].x = x;
                pontos[n].y = y;
                n++;
                ptr += offset;
                continue;
            }
        }
        ptr++;
    }
    return n;
}

int comparar_dist_origem(const void *a, const void *b) {
    Ponto *p1 = (Ponto *)a;
    Ponto *p2 = (Ponto *)b;
    float d1 = dist_origem(*p1);
    float d2 = dist_origem(*p2);
    if (d1 < d2) return -1;
    if (d1 > d2) return 1;
    return 0;
}

void imprimir_saida(FILE *saida, const Ponto pontos_ord[], int n, float d_total, float d_caminho) {
    fprintf(saida, "points");
    for (int i = 0; i < n; i++) {
        fprintf(saida, " (%d,%d)", pontos_ord[i].x, pontos_ord[i].y);
    }
    fprintf(saida, " distance %.2f shortcut %.2f\n", d_total, d_caminho);
}

int main() {
    FILE *fp_in = fopen("L0Q1.in", "r");
    FILE *fp_out = fopen("L0Q1.out", "w");
    if (!fp_in || !fp_out) {
        perror("Erro ao abrir arquivos");
        return EXIT_FAILURE;
    }

    char linha[TAM_STR];
    Ponto pontos[TAM_COORD];
    Ponto pontos_ord[TAM_COORD];

    while (fgets(linha, sizeof(linha), fp_in)) {
        size_t len = strlen(linha);
        if (len > 0 && linha[len - 1] == '\n') {
            linha[len - 1] = '\0';
        }
    
        int somente_espacos = 1;
        for (size_t i = 0; i < len; i++) {
            if (linha[i] != ' ' && linha[i] != '\t' && linha[i] != '\r') {
                somente_espacos = 0;
                break;
            }
        }
        if (somente_espacos || linha[0] == '\0') continue;

        int n = extrair_pontos(linha, pontos);
        if (n == 0) {
            fprintf(fp_out, "points distance 0.00 shortcut 0.00\n");
            continue;
        }

        for (int i = 0; i < n; i++) pontos_ord[i] = pontos[i];
        qsort(pontos_ord, n, sizeof(Ponto), comparar_dist_origem);

        float distancia_total = 0.0;
        for (int i = 0; i < n - 1; i++) {
            distancia_total += dist_entre(pontos[i], pontos[i + 1]);
        }

        float atalho = dist_entre(pontos[0], pontos[n - 1]);

        imprimir_saida(fp_out, pontos_ord, n, distancia_total, atalho);
    }

    fclose(fp_in);
    fclose(fp_out);
    return 0;
}
