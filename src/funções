#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define MEMORIA 100

int main(){
    int acum, pc;
    int vetData[MEMORIA];
    char **vetMem;

    return 0;
}
//CARGI - coloca o valor n no acumulador (A=n)
void cargi(int *acum, int n){
    *acum = n;
}

//CARGM - coloca no acumulador o valor na posição n da memória de dados (A=M[n])
void cargm(int *acum, int n, int *vetdata){
    *acum = veetData[n];
}

//CARGX - coloca no acumulador o valor na posição que está na posição n da memória de dados (A=M[M[n]])
void cargx(int *acum, int n, int *vetdata){
    *acum = *veetData[n];
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
    *acum = *acum + vetdata[n];
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

