#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define MEMORIA 100
#define NORMAL 0
#define INSTTRUCTILEGAL 1
#define MEMORYVIOLATION 2
#define READ 3
#define WRITE 4
#define STOP 5
#define PC 0
#define ACUM 1

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
void cargi(CPU_t *cpu, int n);
void cargm(CPU_t *cpu, int n, int *vetData);
void cargx(CPU_t *cpu, int n, int *vetData);
void armm(CPU_t *cpu, int n, int *vetData);
void armx(CPU_t *cpu, int n, int *vetData);
void soma(CPU_t *cpu, int n, int *vetData);
void desvZ(CPU_t *cpu, int n);
void neg(CPU_t *cpu);
void showCom(POSMEMORIA_t *vetMem);
void execCom(CPU_t *cpu, POSMEMORIA_t *vetMem, ESTADO_t *estado, int *vetData);
int comInv(POSMEMORIA_t *vetMem);
void inicializarCPU(CPU_t *cpu, ESTADO_t *estado);
void estadoNormal(ESTADO_t *estado);
POSMEMORIA_t *returnPC(CPU_t *cpu, POSMEMORIA_t *vetMen);
void alterarRegistradores(CPU_t *cpu, int valor, int reg);
void retornaRegistadores(CPU_t *cpu);
void readInterruption(ESTADO_t *estado);

//alterar o conteúdo da memória de programa (recebe um vetor de strings)
//alterar o conteúdo da memória de dados (recebe um vetor de inteiros, que é alterado pela execução das instruções)
//obter o conteúdo da memória de dados (retorna um vetor de inteiros que é o conteúdo atual da memória – não precisa desta função caso o vetor passado pela função acima seja alterado “in loco”)

//Função principal
int main(){
    CPU_t cpu;
    int vetData[MEMORIA];
    POSMEMORIA_t vetMem[MEMORIA];
    ESTADO_t estado;

    inicializarCPU(&cpu, &estado);
    readFile(vetMem);
    showCom(vetMem);
    execCom(&cpu,vetMem,&estado,vetData);
    readInterruption(&estado);

    POSMEMORIA_t *instTmp;
    instTmp = returnPC(&cpu, vetMem);

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
void execCom(CPU_t *cpu, POSMEMORIA_t *vetMem, ESTADO_t *estado, int *vetData){
    int cont = 0;
    while(estado == NORMAL){
        if(cpu->pc > MEMORIA){
            estado->estado = MEMORYVIOLATION;
            return;
        }
        if(!strcmp("CARGI",vetMem[cpu->pc].inst)){
            cargi(cpu, vetMem[cpu->pc].num);
        }else if(!strcmp("CARGM",vetMem[cpu->pc].inst)){
            cargm(cpu, vetMem[cpu->pc].num, vetData);
        }else if(!strcmp("CARGX",vetMem[cpu->pc].inst)){
            cargx(cpu, vetMem[cpu->pc].num, vetData);
        }else if(!strcmp("ARMM", vetMem[cpu->pc].inst)){
            armm(cpu, vetMem[cpu->pc].num, vetData);
        }else if(!strcmp("ARMX", vetMem[cpu->pc].inst)){
            armx(cpu, vetMem[cpu->pc].num, vetData);
        }else if(!strcmp("SOMA", vetMem[cpu->pc].inst)){
            soma(cpu, vetMem[cpu->pc].num, vetData);
        }else if(!strcmp("NEG", vetMem[cpu->pc].inst)){
            neg(cpu);
        }else if(!strcmp("DESVZ", vetMem[cpu->pc].inst)){
            desvZ(cpu, vetMem[cpu->pc].num);
        }else{
            if(!strcmp("PARA", vetMem[cpu->pc].inst)){
                estado->estado = NORMAL;
                return;
            }else{
                estado->estado = INSTTRUCTILEGAL;
                return;
            }
        }
        cpu->pc++;
    }
}

//inicializa a CPU com os valores básicos
void inicializarCPU(CPU_t *cpu, ESTADO_t *estado){
    cpu->acum = 0;
    cpu->pc = 0;
    estado->estado = NORMAL;
}

//Altera o estado da CPU para normal
void estadoNormal(ESTADO_t *estado){
    estado->estado = NORMAL;
}

//CARGI - coloca o valor n no acumulador (A=n)
void cargi(CPU_t *cpu, int n){
    cpu->acum = n;
}

//CARGM - coloca no acumulador o valor na posição n da memória de dados (A=M[n])
void cargm(CPU_t *cpu, int n, int *vetData){
    cpu->acum = vetData[n];
}

//CARGX - coloca no acumulador o valor na posição que está na posição n da memória de dados (A=M[M[n]])
void cargx(CPU_t *cpu, int n, int *vetData){
    cpu->acum = vetData[n];
}

//ARMM - coloca o valor do acumulador na posição n da memória de dados (M[n]=A)
void armm(CPU_t *cpu, int n, int *vetData){
    vetData[n] = cpu->acum;
}

//ARMX - coloca o valor do acumulador posição que está na posição n da memória de dados (M[M[n]]=A)
void armx(CPU_t *cpu, int n, int *vetData){

}

//SOMA - soma ao acumulador o valor no endereço n da memória de dados (A=A+M[n])
void soma(CPU_t *cpu, int n, int *vetData){
    cpu->acum = cpu->acum + vetData[n];
}

//DESVZ - se A vale 0, coloca o valor n no PC
void desvZ(CPU_t *cpu, int n){
    if(cpu->acum == 0)
        cpu->pc = n;
}

//NEG - inverte o sinal do acumulador (Acum = -Acum)
void neg(CPU_t *cpu){
  cpu->acum = -cpu->acum;
}

//Retorna Instrução em PC
POSMEMORIA_t *returnPC(CPU_t *cpu, POSMEMORIA_t *vetMen){
    if(cpu->pc > MEMORIA)
        return NULL;
    return &vetMen[cpu->pc];
}

//Alterar registradores da CPU
void alterarRegistradores(CPU_t *cpu, int valor, int reg){
    if(reg == PC)
        cpu->pc = valor;
    else if(reg == ACUM)
        cpu->acum = valor;
}

void retornaRegistradores(CPU_t *cpu){
    printf("\nPC = %i, Acumulador = %i", cpu->pc, cpu->acum);
}

void readInterruption(ESTADO_t *estado){
    printf("\n");
    if(estado->estado == NORMAL)
        printf("Estado normal");
    else if(estado->estado == INSTTRUCTILEGAL)
        printf("Instrução ilegal");
    else if(estado->estado == MEMORYVIOLATION)
        printf("Violação de memória");
    else if(estado->estado == READ)
        printf("Modo leitura");
    else if(estado->estado == WRITE)
        printf("Modo escrita");
    else if(estado->estado == STOP)
        printf("Modo parada");
}
