#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define MEMORIA 100
#define NORMAL 0
#define INSTTRUCTILEGAL 1
#define VIOLATION 2
#define READ 3
#define WRITE 4
#define STOP 5
//A estrutura da memória
typedef struct{
    char inst[10];
    int num;
} POSMEMORIA_t;

//A estrtura da CPU
typedef struct{
    int acum, pc;
} CPU_t;

typedef struct{
    int estado;
} ESTADO_t;

void readFile (POSMEMORIA_t *vetMem);
void cargi(int *acum, int n);
void cargm(int *acum, int n, int *vetData);
void cargx(int *acum, int n, int *vetData);
void armm(int *acum, int n, int *vetData);
void armx(int *acum, int n, int *vetData);
void soma(int *acum, int n, int *vetData);
void desvZ(int *acum, int *pc, int n);
void neg(int *acum);
void showCom(POSMEMORIA_t *vetMem);
void execCom(CPU_t *cpu, POSMEMORIA_t *vetMem, int *vetData);
int comInv(POSMEMORIA_t *vetMem);

//Função principal
int main(){
    CPU_t cpu;
    int vetData[MEMORIA];
    POSMEMORIA_t vetMem[MEMORIA];
    readFile(vetMem);
    showCom(vetMem);
    execCom(&cpu,vetMem,vetData);

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
        i = 0;
        while(fscanf(file,"%s %i", inst, &n)!= EOF){
            strcpy(vetMem[i].inst,inst);
            vetMem[i].num = n;
            i++;
        }
    fclose(file);
    }
}

//Mostra a lista de comandos
void showCom(POSMEMORIA_t *vetMem){
    int i = 0;
    printf("Lendo comandos\n");
    while(i != 19){
        printf("\n%s, %i", vetMem[i].inst, vetMem[i].num);
        i++;
    }
}
//Escolhe o comando a executar
void execCom(CPU_t *cpu, POSMEMORIA_t *vetMem, int *vetData){
    int i = 0;
    int cont = 0;
    while(cont == 0){
        if(!strcmp("CARGI",vetMem[i].inst)){
            cargi(&cpu->acum, vetMem[i].num);
        }else if(!strcmp("CARGM",vetMem[i].inst)){
            cargm(&cpu->acum, vetMem[i].num, vetData);
        }else if(!strcmp("CARGX",vetMem[i].inst)){
            cargx(&cpu->acum, vetMem[i].num, vetData);
        }else if(!strcmp("ARMM", vetMem[i].inst)){
            armm(&cpu->acum, vetMem[i].num, vetData);
        }else if(!strcmp("ARMX", vetMem[i].inst)){
            armx(&cpu->acum, vetMem[i].num, vetData);
        }else if(!strcmp("SOMA", vetMem[i].inst)){
            soma(&cpu->acum, vetMem[i].num, vetData);
        }else if(!strcmp("NEG", vetMem[i].inst)){
            neg(&cpu->acum);
        }else if(!strcmp("DESVZ", vetMem[i].inst)){
            desvZ(&cpu->acum, &cpu->pc, vetMem[i].num);
        }else{
            cont++;
        }
        i++;
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
void desvZ(int *acum, int *pc, int n){
    if(*acum == 0)
        *pc = n;
}

//NEG - inverte o sinal do acumulador (Acum = -Acum)
void neg(int *acum){
  *acum = -*acum;
}

