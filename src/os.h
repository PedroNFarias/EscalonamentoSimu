#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

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
#define QUANTUM 10

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
    int tempoDeBloqueio;
    int tempoADesbloquear;
} BLOQUEIO_t;

typedef struct{
    int posicao[MEMORIA];
    float prioridade[MEMORIA];
    int ultimaPosMemoria[MEMORIA]; //primeira posição de memória do programa de acordo com seu numero
    int tempoDeExec[MEMORIA];
    bool bloqueado[MEMORIA];
    BLOQUEIO_t bloqueioDados[MEMORIA];
    int processoEmExec;
    int ultimaPos; //ultima posição de memória alocada com instruções
    bool finalizado[MEMORIA];
} DESCRITOR_t;


int readFile (POSMEMORIA_t *vetMem, int *lastVetMem, char *arq, DESCRITOR_t *descritor);
void cargi(CPU_t *cpu, int n);
void cargm(CPU_t *cpu, int n, int *vetData);
void cargx(CPU_t *cpu, int n, int *vetData);
void armm(CPU_t *cpu, int n, int *vetData, int *lastN);
void armx(CPU_t *cpu, int n, int *vetData, int *lastN);
void soma(CPU_t *cpu, int n, int *vetData);
void desvZ(CPU_t *cpu, int n);
void neg(CPU_t *cpu);
void showCom(POSMEMORIA_t *vetMem, int lastVetMem);
void OS(CPU_t *cpu, POSMEMORIA_t *vetMem, ESTADO_t *estado, int *vetData, int *lastN, TIMER_t *timer, DESCRITOR_t *descritor);
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
void tratarInterrupcao(CPU_t *cpu, ESTADO_t *estado, DESCRITOR_t *descritor, TIMER_t *timer);
//tabela de processos
void iniciarTabelaDeProcessos(DESCRITOR_t *descritor);
void adicionarTabelaDeProcessos(DESCRITOR_t *descritor, int posicao, float prioridade);
void recalcularPrioridade(DESCRITOR_t *decritor);
int escolherProcesso(DESCRITOR_t *descritor, ESTADO_t *estado);
void bloquearProcesso(DESCRITOR_t *descritor, TIMER_t *timer, int tempo);
void acordar(DESCRITOR_t *descritor, TIMER_t *timer);

//alterar o conteúdo da memória de programa (recebe um vetor de strings)
//alterar o conteúdo da memória de dados (recebe um vetor de inteiros, que é alterado pela execução das instruções)
//obter o conteúdo da memória de dados (retorna um vetor de inteiros que é o conteúdo atual da memória – não precisa desta função caso o vetor passado pela função acima seja alterado “in loco”)
