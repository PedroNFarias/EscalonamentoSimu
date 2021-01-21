#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "os.h"

//Função principal
int main(){
    CPU_t cpu;
    int vetData[MEMORIA], lastN = 0, lastVetMem = 0;
    POSMEMORIA_t vetMem[MEMORIA];
    ESTADO_t estado;
    TIMER_t timer;
    DESCRITOR_t descritor;
    char arq1[10], arq2[10], arq3[10], arq4[10], arq5[10];
    strcpy(arq1, "inst1");
    strcpy(arq2, "inst2");
    strcpy(arq3, "inst3");
    strcpy(arq4, "inst4");
    strcpy(arq5, "inst5");

    inicializarCPU(&cpu, &estado);
    criarTimer(&timer);
    iniciarTabelaDeProcessos(&descritor);
    readFile(vetMem, &lastVetMem, arq1, &descritor);
    readFile(vetMem, &lastVetMem, arq2, &descritor);
    readFile(vetMem, &lastVetMem, arq3, &descritor);
    readFile(vetMem, &lastVetMem, arq4, &descritor);
    readFile(vetMem, &lastVetMem, arq5, &descritor);
    showCom(vetMem, lastVetMem);
    OS(&cpu,vetMem,&estado,vetData, &lastN, &timer, &descritor);

    //Retorna as instrução apontada
    POSMEMORIA_t *instTmp;
    instTmp = returnPC(&cpu, vetMem);
    //Testa se está gravando a ultima posição da memória de dados
    printf("\n Ultima mémoria de dados acessada = %i\n", lastN);
    //testa se o leitor de ES tá funcionando

    return 0;
}
