#include<stdio.h>
#include<math.h>
#include<pthread.h>
#include <stdlib.h> 


int k_global = 3;
int nthreads;

double calculaPi(int n){
    double pi;
    pi = 0;
    for(int k=0; k < n;k++){
        pi += ((1.0/pow(16,k)) * ((4.0/(8*k+1)) - (2.0/(8*k+4)) - (1.0/(8*k+5)) - (1.0/(8*k+6))));
    }
    return pi;
}

void * Tarefa(void * arg){
    float pi_local = 0;
    float * ret;
    for(int i=(long int) arg; i<k_global; i+= nthreads){
        pi_local += ((1.0/pow(16,i)) * ((4.0/(8*i+1)) - (2.0/(8*i+4)) - (1.0/(8*i+5)) - (1.0/(8*i+6))));
    }
    ret = malloc(sizeof(float));
    *ret = pi_local;
    pthread_exit((void*)ret);
}

int main(int argc, char *argv[]){
    float * pi_local;
    float pi_global = 0.0;
    // pthread_t * tid;
    if(argc<2){
        printf("Execute com %s <numero de threads>\n", argv[0]);
    }
    nthreads = atoi(argv[1]);
    pthread_t tid[nthreads];

    for(long int i=0; i<nthreads;i++){
        if(pthread_create(&tid[i], NULL, Tarefa, (void*)i)){
            printf("Ops, algo deu errado!\n");
        }
    }
     for(int i=0; i<nthreads;i++){
        if(pthread_join(tid[i], (void*) & pi_local)){
            printf("Ops, algo deu errado!");
        }
        pi_global += *pi_local;
    }

    // printf("Valor calculado para Pi = %f\n", calculaPi(100));
     printf("Valor calculado para Pi = %f\n", pi_global);
    return 0;
}