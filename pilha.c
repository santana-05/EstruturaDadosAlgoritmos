#include <stdio.h>
#include <stdlib.h>
#define TAM 10

int pilha [TAM];
int topo = 0;

void coloca(int x){
    pilha[topo] = x;
    topo++;
}

int retira(){
    topo--;
    return pilha[topo];
}

int main(int argc, char **argv){
    int n;
    coloca(5);
    coloca(14);
    n = retira();
    
    for(int i = 0; i < topo; i++){
        printf("%d \n", pilha[i]);
    }

    printf("%d \n", n);
    
    return 0;
}