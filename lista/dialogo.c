#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int started = 0;
pthread_cond_t cond;
pthread_mutex_t mutex;

void * Tarefa1(void* arg){
   char *msgs[] = {"olá, você está acessando a variável aux agora?", "certo, então vou alterá-la, tá?", "terminei a alteração da variável aux"};
    for(int i=0; i < 3; i++){
        pthread_mutex_lock(&mutex);
        if(started){
            pthread_cond_wait(&cond, &mutex);
        }
        started = 1;
        printf("%s\n", msgs[i]);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
    pthread_exit(NULL);
}

void * Tarefa2(void* arg){
    char *msgs[] = {"oi, não, não estou", "tudo bem", "perfeito, recebido!"};
    for(int i=0; i < 3; i++){
        pthread_mutex_lock(&mutex);
        if(!started){
            pthread_cond_wait(&cond, &mutex);
        }
        started = 0;
        printf("%s\n", msgs[i]);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
    pthread_exit(NULL);
}

int main(void){
    pthread_t tid1, tid2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&tid1, NULL, Tarefa1, NULL);
    pthread_create(&tid2, NULL, Tarefa2, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}