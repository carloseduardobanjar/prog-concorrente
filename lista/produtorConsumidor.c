#include<stdio.h>
#include<pthread.h>
#define N 5

#define nthread_consumidor 5
#define nthread_produtor 1

int contador = 0;
int head = 0;
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
        while(contador == N){
            pthread_cond_wait(&terVaga, &mutex);
        }
        contador++;
        buffer[head] = itemASerInserido;
        printf("Produz %d\n", itemASerInserido);
        head = (head + 1) % N;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&terItem);    
    }
    pthread_exit(NULL);
}

void * Consome(void*arg){
    int itemASerRemovido;
    for(int i=0; i<10;i++){
        pthread_mutex_lock(&mutex);
        while(contador == 0){
            pthread_cond_wait(&terItem, &mutex);
        }
        contador--;
        itemASerRemovido = buffer[tail];
        printf("Consome %d\n", itemASerRemovido);
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

    for(int i=0;i<nthread_consumidor;i++){
        pthread_create(&tid_consome[i], NULL, Consome, NULL);
    }

    for(int i=0;i<nthread_produtor;i++){
        pthread_join(tid_produz[i], NULL);
    }

    for(int i=0;i<nthread_consumidor;i++){
        pthread_join(tid_consome[i], NULL);
    }

    return 0;
}