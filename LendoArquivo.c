#include <stdio.h>
#define TAM 600

int main(){
   FILE *arquivo;
   int num1, num2;
   
   arquivo = fopen("L0Q1.in", "r"); // abre o arquivo
   


   if (arquivo == NULL) {
      printf("Erro ao abrir arquivo.\n");
      return 1;
   }else {
         printf("Arquivo aberto com sucesso.\n");
   }

   while (fscanf(arquivo, "%d %d", &num1, &num2) == 2) {
      printf("(%d, %d)\n", num1, num2);
   }

   fclose(arquivo);
   return 0;
}