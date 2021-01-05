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
#define PERIODICA 6
#define APERIODICA 7
#define DORMINDO 8
#define DESLIGADO 9
#define INTERRUPCAOFALSE 10
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

typedef struct{
    int timer;
    int vetInterrupcaoDate[MEMORIA];
    int vetInterrupcaoMotivo[MEMORIA];
    int ultimaInterrupcao;
 } TIMER_t;

 typedef struct{
     int posicao;
     float prioridade;
 } DESCRITOR_t;

int readFile (POSMEMORIA_t *vetMem, int *lastVetMem, char *arq);
void cargi(CPU_t *cpu, int n);
void cargm(CPU_t *cpu, int n, int *vetData);
void cargx(CPU_t *cpu, int n, int *vetData);
void armm(CPU_t *cpu, int n, int *vetData, int *lastN);
void armx(CPU_t *cpu, int n, int *vetData, int *lastN);
void soma(CPU_t *cpu, int n, int *vetData);
void desvZ(CPU_t *cpu, int n);
void neg(CPU_t *cpu);
void showCom(POSMEMORIA_t *vetMem, int lastVetMem);
void OS(CPU_t *cpu, POSMEMORIA_t *vetMem, ESTADO_t *estado, int *vetData, int *lastN, TIMER_t *timer);
void lerCom(CPU_t *cpu, POSMEMORIA_t *vetMem, ESTADO_t *estado, int *vetData, int *lastN);
int comInv(POSMEMORIA_t *vetMem);
void inicializarCPU(CPU_t *cpu, ESTADO_t *estado);
void estadoNormal(ESTADO_t *estado);
POSMEMORIA_t *returnPC(CPU_t *cpu, POSMEMORIA_t *vetMen);
void alterarRegistradores(CPU_t *cpu, int valor, int reg);
void retornaRegistadores(CPU_t *cpu);
int retornaAcum(CPU_t *cpu);
void readInterruption(ESTADO_t *estado);
void changeMemData(int *vetData, int *tempData, int tamVet, int *lastN);
////////////////////////Segunda parte
void leituraES(CPU_t *cpu);
void gravacaoES(CPU_t *cpu);
///////////timer
void criarTimer(TIMER_t *timer);
void incrementaTimer(TIMER_t *timer);
int retornaTimer(TIMER_t *timer);
void inicializarVetInterrupcao(TIMER_t *timer);
void adicionarInterrupcao(TIMER_t *timer, int date, int motivo);
int gerarInterrupcao(TIMER_t *timer);
//interrupções
void tratarinterrupcao(CPU_t *cpu, ESTADO_t *estado);

//alterar o conteúdo da memória de programa (recebe um vetor de strings)
//alterar o conteúdo da memória de dados (recebe um vetor de inteiros, que é alterado pela execução das instruções)
//obter o conteúdo da memória de dados (retorna um vetor de inteiros que é o conteúdo atual da memória – não precisa desta função caso o vetor passado pela função acima seja alterado “in loco”)

//Função principal
int main(){
    CPU_t cpu;
    int vetData[MEMORIA], lastN = 0, lastVetMem = 0;
    POSMEMORIA_t vetMem[MEMORIA];
    ESTADO_t estado;
    TIMER_t timer;
    char arq1[10], arq2[10], arq3[10];
    strcpy(arq1, "inst1");
    strcpy(arq2, "inst2");
    strcpy(arq3, "inst3");

    inicializarCPU(&cpu, &estado);
    criarTimer(&timer);
    readFile(vetMem, &lastVetMem, arq1);
    readFile(vetMem, &lastVetMem, arq2);
    showCom(vetMem, lastVetMem);
    OS(&cpu,vetMem,&estado,vetData, &lastN, &timer);

    //Retorna as instrução apontada
    POSMEMORIA_t *instTmp;
    instTmp = returnPC(&cpu, vetMem);
    //Testa se está gravando a ultima posição da memória de dados
    printf("\n Ultima mémoria de dados acessada = %i\n", lastN);
    //testa se o leitor de ES tá funcionando

    return 0;
}

//Função que lê um arquivo
int readFile (POSMEMORIA_t *vetMem, int *lastVetMem, char *arq) {
    int i = *lastVetMem;
    FILE *file;
    char inst[10];
    int n;
    file = fopen(arq,"r");
    if (file == NULL) {
        printf("error to load file\n");
    }else {
        while(fscanf(file,"%s %i", inst, &n)!= EOF){
            strcpy(vetMem[i].inst,inst);
            vetMem[i].num = n;
            i++;
        }
    fclose(file);
    }
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
void OS(CPU_t *cpu, POSMEMORIA_t *vetMem, ESTADO_t *estado, int *vetData, int *lastN, TIMER_t *timer){

    while(estado->estado != DESLIGADO){
        if(estado->estado == NORMAL){
            lerCom(cpu, vetMem, estado, vetData, lastN);
            cpu->pc++;
        }else{
            readInterruption(estado);
            //tratarInterrupcao(estado);
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
        }else if(!strcmp("LER", vetMem[cpu->pc].inst)){
            estado->estado = READ;
            return;
        }else if(!strcmp("GRAVAR", vetMem[cpu->pc].inst)){
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
void tratarinterrupcao(CPU_t *cpu, ESTADO_t *estado){
    if(estado->estado == WRITE){
        gravacaoES(cpu);
        //bloquerProcesso();
        //escolherProcesso();
    }else if(estado->estado == READ){
        leituraES(cpu);
        //bloquerProcesso();
        //escolherProcesso();
    }else if(estado->estado == STOP){
        //escolherProcesso();
        //bloquerProcesso();
    }else if(estado->estado == MEMORYVIOLATION){
        //escolherProcesso();
    }else if(estado->estado == DORMINDO){
        sleep(estado);
        //setarAcordar();
    }
}
//Descritor de processos