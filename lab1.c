#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
   int * ptrInicial;
   int qtd_elementos;
} t_Args;

void *SomarUm (void *arg) {
    t_Args *args = (t_Args *) arg;

    for(int i = 0; i < args->qtd_elementos; i++){
        *(args->ptrInicial + i) = *(args->ptrInicial + i) + 1;
    }

    free(arg);

    pthread_exit(NULL);
}

void inicializarVetor(int vetor1[], int vetor2[], int tamanho) {
    int num_rand;
    srand(time(NULL));

    for (int i = 0; i < tamanho; i++) {
        num_rand = rand() % 101;
        vetor1[i] = num_rand;
        vetor2[i] = num_rand;
    }
}

int verificarDiferenca(int *vetor1, int *vetor2, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (abs(vetor1[i] - vetor2[i]) != 1) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int *vetor_alterado, *vetor_original, mthreads, nvetor, qtd_por_thread, qtd_proc = 0, resto;

    t_Args *args;

    if(argc < 3) {
        printf("ERRO: informe M e N\n");
        return 1;
    }

    mthreads = atoi(argv[1]);
    nvetor = atoi(argv[2]);
    vetor_original = (int *) malloc(nvetor * sizeof(int));
    vetor_alterado = (int *) malloc(nvetor * sizeof(int));

    if (vetor_original == NULL || vetor_alterado == NULL) {
        printf("ERRO: malloc()\n");
        return 1;
    }

    inicializarVetor(vetor_original, vetor_alterado, nvetor);

    if(mthreads != 0){
        qtd_por_thread = nvetor / mthreads;
        resto = nvetor % mthreads;
    } else{
        printf("ERRO: informe M diferente de 0.\n");
        return 1;
    }

    pthread_t tid_sistema[mthreads];

    for(int i=0; i<mthreads; i++) {
        args = malloc(sizeof(t_Args));
        if (args == NULL) {
            printf("ERRO: malloc()\n");
            return 1;
        }
        args->ptrInicial = vetor_alterado + qtd_proc;

        if(resto > 0){
            args->qtd_elementos = qtd_por_thread + 1;
            qtd_proc += qtd_por_thread + 1;
            resto--;
        }
        else {
            args->qtd_elementos = qtd_por_thread;
            qtd_proc += qtd_por_thread;
        }

        if (pthread_create(&tid_sistema[i], NULL, SomarUm, (void*) args)) {
            printf("ERRO: pthread_create()\n");
            return 2;
        }
    }

    for (int i=0; i<mthreads; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("ERRO: pthread_join() da thread %d\n", i);
        }
    }

    if (verificarDiferenca(vetor_original, vetor_alterado, nvetor)) {
        printf("[ERRADO] A diferença entre os elementos nas mesmas posições não é 1.\n");
    } else {
        printf("[CORRETO] A diferença entre todos os elementos nas mesmas posições é 1.\n");
    }
}
