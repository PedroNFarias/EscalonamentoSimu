#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "os.h"

//Função que lê um arquivo
int readFile (POSMEMORIA_t *vetMem, int *lastVetMem, char *arq, DESCRITOR_t *descritor){
    int i = *lastVetMem;
    FILE *file;
    char inst[10];
    int n;
    file = fopen(arq,"r");
    if (file == NULL) {
        printf("error to load file\n");
    }else {
        while(fscanf(file,"%s %i", inst, &n)!= EOF){
            if(!strcmp("DESVZ", inst)){
                vetMem[i].num = n + *lastVetMem;
            }else{
                vetMem[i].num = n;
            }
            strcpy(vetMem[i].inst,inst);
            i++;
        }
    fclose(file);
    }
    adicionarTabelaDeProcessos(descritor, i, 0.5);
    *lastVetMem = i;
}

//Mostra a lista de comandos,
void showCom(POSMEMORIA_t *vetMem, int lastVetMen){
    int i = 0;
    printf("Lendo comandos\n");
    while(i != lastVetMen){
        printf("\n%s, %i", vetMem[i].inst, vetMem[i].num);
        i++;
    }
}

//Escolhe o comando a executar e executa até ocorrer uma interrupção
void OS(CPU_t *cpu, POSMEMORIA_t *vetMem, ESTADO_t *estado, int *vetData, int *lastN, TIMER_t *timer, DESCRITOR_t *descritor){

    while(estado->estado != DESLIGADO){
        printf("\nPC = %i", cpu->pc);
        if(estado->estado == NORMAL){
            lerCom(cpu, vetMem, estado, vetData, lastN);
            cpu->pc++;
        }else{
            readInterruption(estado);
            tratarInterrupcao(cpu, estado, descritor, timer);
        }
        incrementaTimer(timer);
    }

}

void lerCom(CPU_t *cpu, POSMEMORIA_t *vetMem, ESTADO_t *estado, int *vetData, int *lastN){
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
        armm(cpu, vetMem[cpu->pc].num, vetData, lastN);
    }else if(!strcmp("ARMX", vetMem[cpu->pc].inst)){
        armx(cpu, vetMem[cpu->pc].num, vetData, lastN);
    }else if(!strcmp("SOMA", vetMem[cpu->pc].inst)){
        soma(cpu, vetMem[cpu->pc].num, vetData);
    }else if(!strcmp("NEG", vetMem[cpu->pc].inst)){
        neg(cpu);
    }else if(!strcmp("DESVZ", vetMem[cpu->pc].inst)){
        desvZ(cpu, vetMem[cpu->pc].num);
    }else{
        if(!strcmp("PARA", vetMem[cpu->pc].inst)){
            estado->estado = STOP;
            return;
        }else if(!strcmp("LE", vetMem[cpu->pc].inst)){
            estado->estado = READ;
            return;
        }else if(!strcmp("GRAVA", vetMem[cpu->pc].inst)){
            estado->estado = WRITE;
            return;
        }else{
            estado->estado = INSTTRUCTILEGAL;
            return;
        }
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
void armm(CPU_t *cpu, int n, int *vetData, int *lastN){
    vetData[n] = cpu->acum;
    if(*lastN < n)
        *lastN = n;
}

//ARMX - coloca o valor do acumulador posição que está na posição n da memória de dados (M[M[n]]=A)
void armx(CPU_t *cpu, int n, int *vetData, int *lastN){
    int tmp;
    tmp = vetData[n];
    vetData[tmp] = cpu->acum;
    if(*lastN < tmp)
        *lastN = tmp;
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

//DORMINDO - coloca o estado em dormencia
void sleep(ESTADO_t *estado){
    estado->estado = DORMINDO;
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

//Retorna os valores dos registradores
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

//alterar o conteúdo da memória de dados (recebe um vetor de inteiros, que é alterado pela execução das instruções)
void changeMemData(int *vetData, int *tempData, int tamVet, int *lastN){
    int i;
    i = *lastN;

    while(i < *lastN + tamVet){
        vetData[i] = tempData[i - *lastN];
        i++;
    }
}

//alterar o conteúdo da memória de programa (recebe um vetor de strings)

//retorna acumulador
int retornaAcum(CPU_t *cpu){
    return cpu->acum;
}

//SEGUNDA PARTE
//Ler
void leituraES(CPU_t *cpu){
    FILE *file;
    int n;
    file = fopen("ES","r");
    if (file == NULL) {
        printf("\nerror to load file\n");
    }else {
        while(fscanf(file,"%i", &n)!= EOF){
            cpu->acum = n;
        }
    fclose(file);
    }
}

//Gravar
void gravacaoES(CPU_t *cpu){
    FILE *file;

    file = fopen("ES", "w");
    if (file == NULL){
        printf("\nerror to load file\n");
    }else{
        fprintf(file, "%i", cpu->acum);
    }
}

//Timer
void criarTimer(TIMER_t *timer){
    timer->timer = 0;
    inicializarVetInterrupcao(timer);
}

void incrementaTimer(TIMER_t *timer){
    timer->timer++;
}

int retornaTimer(TIMER_t *timer){
    return timer->timer;
}

void inicializarVetInterrupcao(TIMER_t *timer){
    timer->ultimaInterrupcao = 0;
}

void timerAdicionarInterrupcao(TIMER_t *timer, int date, int motivo){
    timer->vetInterrupcaoDate[timer->ultimaInterrupcao] = date;
    timer->vetInterrupcaoMotivo[timer->ultimaInterrupcao] = motivo;
    timer->ultimaInterrupcao++;
}

int gerarInterrupcao(TIMER_t *timer){
    int i = 0;

    while(i <= timer->ultimaInterrupcao){
        if(timer->timer == timer->vetInterrupcaoDate[i]){
            return timer->vetInterrupcaoMotivo[i];
        }
        i++;
    }
    return INTERRUPCAOFALSE;
}

//Interrupções
void tratarInterrupcao(CPU_t *cpu, ESTADO_t *estado, DESCRITOR_t *descritor, TIMER_t *timer){
    if(estado->estado == WRITE){
        gravacaoES(cpu);
        bloquearProcesso(descritor, timer, WRITE);
        cpu->pc = escolherProcesso(descritor, estado);
    }else if(estado->estado == READ){
        leituraES(cpu);
        bloquearProcesso(descritor, timer, READ);
        cpu->pc = escolherProcesso(descritor, estado);
    }else if(estado->estado == STOP){
        bloquearProcesso(descritor, timer, STOP);
        cpu->pc = escolherProcesso(descritor, estado);
    }else if(estado->estado == MEMORYVIOLATION){
        cpu->pc = escolherProcesso(descritor, estado);
    }else if(estado->estado == DORMINDO){
        sleep(estado);
        acordar(descritor, timer);
    }
    recalcularPrioridade(descritor);
}

//Descritor de processos
void iniciarTabelaDeProcessos(DESCRITOR_t *descritor){
    descritor->ultimaPos = 0;
    descritor->processoEmExec = 0;
}

void adicionarTabelaDeProcessos(DESCRITOR_t *descritor, int posicao, float prioridade){
    descritor->posicao[descritor->ultimaPos] = posicao;
    descritor->prioridade[descritor->ultimaPos] = prioridade;
    descritor->ultimaPosMemoria[descritor->ultimaPos] = posicao;
    descritor->tempoDeExec[descritor->ultimaPos] = 0;
    descritor->bloqueado[descritor->ultimaPos] = false;
    descritor->finalizado[descritor->ultimaPos] = false;
    descritor->ultimaPos++;
}

int escolherProcesso(DESCRITOR_t *descritor, ESTADO_t *estado){
    int i = 0, j = 0, escolhido = MEMORYVIOLATION;
    printf("Escolhendo processo");

    while(escolhido == MEMORYVIOLATION){
        if(descritor->finalizado[j] == false){
            escolhido = j;
        }
        j++;
        if(j > descritor->ultimaPos){
            return NULL;
        }
    }
    while(i < descritor->ultimaPos){
        if(descritor->prioridade[escolhido] > descritor->prioridade[i] && descritor->finalizado[i] != false)
            escolhido = descritor->posicao[i];
        i++;
    }

    estado->estado = NORMAL;
    descritor->processoEmExec = escolhido;
    return descritor->ultimaPosMemoria[escolhido];
}

void recalcularPrioridade(DESCRITOR_t *descritor){
    int i = 0, aux;

    while(i < descritor->ultimaPos){
        if(descritor->tempoDeExec[i] == 0){
            descritor->prioridade[i] = descritor->prioridade[i] - 0.1;
        }else{
            aux = descritor->tempoDeExec[i]/QUANTUM;
            descritor->prioridade[i] = descritor->prioridade[i] + aux;
            if(descritor->prioridade[i] > 1){
                descritor->prioridade[i] = 1;
            }
        }

        if(descritor->prioridade[i] < 0){
            descritor->prioridade[i] = 0;
        }else if(descritor->prioridade[i] > 1){
            descritor->prioridade[i] = 1;
        }
        i++;
    }
}

void bloquearProcesso(DESCRITOR_t *descritor, TIMER_t * timer, int tempo){
    descritor->bloqueado[descritor->processoEmExec] = true;
    descritor->bloqueioDados[descritor->processoEmExec].tempoDeBloqueio = timer->timer;
    descritor->bloqueioDados[descritor->processoEmExec].tempoADesbloquear = timer->timer + 5;
}

void acordar(DESCRITOR_t *descritor, TIMER_t *timer){
    int i = 0;

    while(i < descritor->ultimaPos){
        if(descritor->bloqueioDados[i].tempoADesbloquear < timer->timer){
            descritor->bloqueado[i] = false;
        }
        i++;
    }
}
