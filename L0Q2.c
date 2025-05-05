// Maurício Santana e Daniel Santana

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_BUFFER 1024
#define MAX_ELEMENTS 128

typedef struct {
    double coordenada_x;
    double coordenada_y;
    double distancia_origem;
    char representacao[64];
} Ponto;

int eh_inteiro(const char *str) {
    char *fim;
    strtol(str, &fim, 10);
    return *fim == '\0';
}

int eh_decimal(const char *str) {
    char *fim;
    strtod(str, &fim);
    return *fim == '\0' && strchr(str, '.');
}

int eh_ponto(const char *str, double *x, double *y, char *original) {
    if (str[0] != '(' || str[strlen(str) - 1] != ')') return 0;
    
    char copia[64];
    strncpy(copia, str + 1, strlen(str) - 2);
    copia[strlen(str) - 2] = '\0';
    
    char *virgula = strchr(copia, ',');
    if (!virgula) return 0;
    
    *virgula = '\0';
    char *x_str = copia;
    char *y_str = virgula + 1;
    
    char *fim_x, *fim_y;
    *x = strtod(x_str, &fim_x);
    *y = strtod(y_str, &fim_y);
    
    if (*fim_x || *fim_y) return 0;
    
    strcpy(original, str);
    return 1;
}

int comparar_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int comparar_inteiros(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int comparar_decimais(const void *a, const void *b) {
    double diff = *(double *)a - *(double *)b;
    return (diff > 0) - (diff < 0);
}

int comparar_pontos(const void *a, const void *b) {
    double dist_a = ((Ponto *)a)->distancia_origem;
    double dist_b = ((Ponto *)b)->distancia_origem;
    return (dist_a > dist_b) - (dist_a < dist_b);
}

void processar_linha(const char *linha, FILE *saida) {
    char elementos[MAX_ELEMENTS][64];
    char *ponteiros_elementos[MAX_ELEMENTS];
    int total_elementos = 0;
    
    
    char buffer[MAX_BUFFER];
    strcpy(buffer, linha);
    char *token = strtok(buffer, " ");
    
    while (token && total_elementos < MAX_ELEMENTS) {
        strcpy(elementos[total_elementos], token);
        ponteiros_elementos[total_elementos] = elementos[total_elementos];
        total_elementos++;
        token = strtok(NULL, " ");
    }
    
    
    char *strings[MAX_ELEMENTS];
    int inteiros[MAX_ELEMENTS], total_inteiros = 0;
    double decimais[MAX_ELEMENTS];
    int total_strings = 0, total_decimais = 0;
    Ponto pontos[MAX_ELEMENTS];
    int total_pontos = 0;

    for (int i = 0; i < total_elementos; i++) {
        double x, y;
        char original[64];
        
        if (eh_ponto(ponteiros_elementos[i], &x, &y, original)) {
            pontos[total_pontos].coordenada_x = x;
            pontos[total_pontos].coordenada_y = y;
            pontos[total_pontos].distancia_origem = sqrt(x*x + y*y);
            strcpy(pontos[total_pontos].representacao, original);
            total_pontos++;
        } 
        else if (eh_inteiro(ponteiros_elementos[i])) {
            inteiros[total_inteiros++] = atoi(ponteiros_elementos[i]);
        } 
        else if (eh_decimal(ponteiros_elementos[i])) {
            decimais[total_decimais++] = atof(ponteiros_elementos[i]);
        } 
        else {
            strings[total_strings++] = ponteiros_elementos[i];
        }
    }

    
    qsort(strings, total_strings, sizeof(char *), comparar_strings);
    qsort(inteiros, total_inteiros, sizeof(int), comparar_inteiros);
    qsort(decimais, total_decimais, sizeof(double), comparar_decimais);
    qsort(pontos, total_pontos, sizeof(Ponto), comparar_pontos);
    
    
    fprintf(saida, "str:");
    for (int i = 0; i < total_strings; i++) {
        fprintf(saida, "%s%s", (i > 0 ? " " : ""), strings[i]);
    }

    fprintf(saida, " int:");
    for (int i = 0; i < total_inteiros; i++) {
        fprintf(saida, "%s%d", (i > 0 ? " " : ""), inteiros[i]);
    }

    fprintf(saida, " float:");
    for (int i = 0; i < total_decimais; i++) {
        fprintf(saida, "%s%.15g", (i > 0 ? " " : ""), decimais[i]);
    }

    fprintf(saida, " p:");
    for (int i = 0; i < total_pontos; i++) {
        fprintf(saida, "%s%s", (i > 0 ? " " : ""), pontos[i].representacao);
    }

    fprintf(saida, "\n");
}

int main() {
    FILE *entrada = fopen("L0Q2.in", "r");
    FILE *saida = fopen("L0Q2.out", "w");
    
    if (!entrada || !saida) {
        perror("Erro ao abrir arquivos");
        return EXIT_FAILURE;
    }

    char linha[MAX_BUFFER];
    while (fgets(linha, sizeof(linha), entrada)) {
        linha[strcspn(linha, "\n")] = '\0';
        processar_linha(linha, saida);
    }

    fclose(entrada);
    fclose(saida);
    return EXIT_SUCCESS;
}