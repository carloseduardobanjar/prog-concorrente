#include<stdio.h>
#include<pthread.h>
#define N 5

#define nthread_consumidor 5
#define nthread_produtor 1

int contador = 0;
int tail = 0;
int buffer[N];
pthread_cond_t terVaga;
pthread_cond_t terItem;
pthread_mutex_t mutex;

void * Produz(void*arg){
    int itemASerInserido;
    for(int i=0; i<50;i++){
        itemASerInserido = i;
        pthread_mutex_lock(&mutex);
        while(contador != 0){
            pthread_cond_wait(&terVaga, &mutex);
        }
        contador = N;
        for(int i=0;i<N;i++){
            buffer[i] = itemASerInserido;
            printf("Produz %d\n", itemASerInserido);
        }
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&terItem);    
    }
    pthread_exit(NULL);
}

void * Consome(void*arg){
    int itemASerRemovido;
    long int id;
    id = (long int) arg;
    for(int i=0; i<50;i++){
        pthread_mutex_lock(&mutex);
        while(contador == 0){
            pthread_cond_wait(&terItem, &mutex);
        }
        contador--;
        itemASerRemovido = buffer[tail];
        printf("Consome %d (thread %ld)\n", itemASerRemovido, id);
        tail = (tail + 1) % N;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&terVaga);
    }
    pthread_exit(NULL);
}


int main(void){

    pthread_t tid_produz[nthread_produtor], tid_consome[nthread_consumidor];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&terVaga, NULL);
    pthread_cond_init(&terItem, NULL);

    for(int i=0;i<nthread_produtor;i++){
        pthread_create(&tid_produz[i], NULL, Produz, NULL);
    }

    for(long int i=0;i<nthread_consumidor;i++){
        pthread_create(&tid_consome[i], NULL, Consome, (void *)i);
    }

    for(int i=0;i<nthread_produtor;i++){
        pthread_join(tid_produz[i], NULL);
    }

    for(int i=0;i<nthread_consumidor;i++){
        pthread_join(tid_consome[i], NULL);
    }

    return 0;
}