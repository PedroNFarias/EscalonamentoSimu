#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>


typedef struct{
    char inst[10];
    int num;
} POSMEMORIA_t;

void readFile (POSMEMORIA_t *vetMem);
void cargi(int *acum, int n);
void cargm(int *acum, int n, int *vetData);
void cargx(int *acum, int n, int *vetData);
void armm(int *acum, int n, int *vetData);
void armx(int *acum, int n, int *vetData);
void soma(int *acum, int n, int *vetData);
void desvZ(int *acum, int *pc, int inst);
void neg(int *acum);
void showCom(POSMEMORIA_t *vetMem);


int main(){
    int acum, pc;
    int vetData[MEMORIA];
    POSMEMORIA_t *posMemoria;
    readFile(posMemoria);

    return 0;
}

//Função que lê um arquivo
void readFile (POSMEMORIA_t *vetMem) {
    int i = 0;
    FILE *file;
    char inst[10];
    int n;
    file = fopen("inst","r");
    if (file == NULL) {
        printf("error to load file\n");
    }else {
        while(fscanf(file,"%s %i", inst, &n)!= EOF){
            i++;
        }
        vetMem = malloc(i * sizeof(POSMEMORIA_t));
        fclose(file);
        file = fopen("inst", "r");
        i = 0;
        while(fscanf(file,"%s %i", inst, &n)!= EOF){
            printf("%s %i\n", inst, n);
            strcpy(inst,vetMem[i].inst);
            vetMem[i].num = n;
            i++;
        }
    fclose(file);
    }
}

//Mostra a lista de comandos
void showCom(POSMEMORIA_t *vetMem){
    int i = 0;
    
    while(i != 19){
        printf("\n%s, %i", );
    }
}


//CARGI - coloca o valor n no acumulador (A=n)
void cargi(int *acum, int n){
    *acum = n;
}

//CARGM - coloca no acumulador o valor na posição n da memória de dados (A=M[n])
void cargm(int *acum, int n, int *vetData){
    *acum = vetData[n];
}

//CARGX - coloca no acumulador o valor na posição que está na posição n da memória de dados (A=M[M[n]])
void cargx(int *acum, int n, int *vetData){
    *acum = vetData[n];
}

//ARMM - coloca o valor do acumulador na posição n da memória de dados (M[n]=A)
void armm(int *acum, int n, int *vetData){
    vetData[n] = *acum;
}

//ARMX - coloca o valor do acumulador posição que está na posição n da memória de dados (M[M[n]]=A)
void armx(int *acum, int n, int *vetData){

}

//SOMA - soma ao acumulador o valor no endereço n da memória de dados (A=A+M[n])
void soma(int *acum, int n, int *vetData){
    *acum = *acum + vetData[n];
}

//DESVZ - se A vale 0, coloca o valor n no PC
void desvZ(int *acum, int *pc, int inst){
    if(*acum == 0)
        *pc = inst;
}

//NEG - inverte o sinal do acumulador (Acum = -Acum)
void neg(int *acum){
  *acum = -*acum;
}

