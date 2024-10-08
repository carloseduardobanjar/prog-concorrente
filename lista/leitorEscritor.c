#include<stdio.h>
#include<pthread.h>

int leitores = 0;
int escritores = 0;
pthread_cond_t aLer, aEscrever;
pthread_mutex_t mutex;

void EntraLeitura(){
    pthread_mutex_lock(&mutex);
    while(escritores > 0){
        pthread_cond_wait(&aLer, &mutex);
    }
    leitores++;
    pthread_mutex_unlock(&mutex);
}

void SaiLeitura(){
    pthread_mutex_lock(&mutex);
    leitores--;
    if(leitores == 0){
        pthread_cond_signal(&aEscrever);
    }
    pthread_mutex_unlock(&mutex);
}

void Leitor(void * arg){
    EntraLeitura();

    SaiLeitura();
}

void EntraEscrita(){
    pthread_mutex_lock(&mutex);
    while(leitores > 0 || escritores > 0){
        pthread_cond_wait(&aEscrever, &mutex);
    }
    escritores++;
    pthread_mutex_unlock(&mutex);
}

void SaiEscrita(){
    pthread_mutex_lock(&mutex);
    escritor--;
    if(escritor == 0){
        pthread_cond_broadcast(&aLer);
        pthread_cond_signal(&aEscrever);
    }
    pthread_mutex_unlock(&mutex);
}

void * Escritor(void * arg){å
    
    EntraEscrita();

    SaiEscrita();
    
}

void 


int main(void){
    pthread_t tid1, tid2;

    pthread_cond_init(&aLer, NULL);
    pthread_cond_init(&aEscrever, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&tid1, NULL, Le, NULL);
    pthread_create(&tid1, NULL, Escreve, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);


    return 0;
}