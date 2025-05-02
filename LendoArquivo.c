#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(){
   FILE *arquivo;
   int nums[1000]; //armazena todos os números lidos
   int count = 0;
   char c;
   
   arquivo = fopen("L0Q1.in", "r"); // abre o arquivo

   if (arquivo == NULL) {
      printf("Erro ao abrir arquivo.\n");
      return 1;
   }else {
         printf("Arquivo aberto com sucesso.\n");
   }

   // Lê todos os números do arquivo
   while ((c = fgetc(arquivo)) != EOF) {
      if (isdigit(c) || c == '-') {
          ungetc(c, arquivo);
          if (fscanf(arquivo, "%d", &nums[count]) == 1) {
              count++;
          }
      }
  }

  // Imprime os pares (x, y)
  for (int i = 0; i < count; i += 2) {
      if (i + 1 < count) { // Verifica se há um par completo
          printf("(%d, %d)\n", nums[i], nums[i + 1]);
      }
  }

   fclose(arquivo);
   return 0;
}