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

void inicializarVetor(int vetor[], int tamanho) {
    srand(time(NULL));
    
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = rand();
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
    int *vetor_alterado, *vetor_original, mthreads, nvetor, qtd_por_thread;
  
    t_Args *args;

    if(argc < 3) {
        printf("ERRO: informe M e N");
        return 1;
    }

    mthreads = atoi(argv[1]);
    nvetor = atoi(argv[2]);
    vetor_original = (int *) malloc(nvetor * sizeof(int));
    inicializarVetor(vetor_original, nvetor);

    vetor_alterado = vetor_original;

    qtd_por_thread = nvetor / mthreads;

    pthread_t tid_sistema[mthreads];

    for(int i=0; i<mthreads; i++) {
        args = malloc(sizeof(t_Args));
        if (args == NULL) {
            printf("ERRO: malloc()\n"); 
            return 1;
        }
        if(i == mthreads - 1){
            args->qtd_elementos = nvetor - ((mthreads-1) * qtd_por_thread); 
        }
        else {
            args->qtd_elementos = qtd_por_thread;
        }
        args->ptrInicial = vetor_original + (qtd_por_thread * i); 
        
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
        printf("[CORRETO] A diferença entre todos os elementos nas mesmas posições é 1.\n");
    } else {
        printf("[ERRADO] A diferença entre os elementos nas mesmas posições não é 1.\n");
    }
}
