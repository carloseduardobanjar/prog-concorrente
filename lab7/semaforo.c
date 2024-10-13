#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#define N 15

sem_t slotCheioBuffer1, slotVazioBuffer1, slotCheioBuffer2, slotVazioBuffer2;

char buffer1[N];
char buffer2[2*N];

void *Insere (void*) {
    FILE *arq;
    char *result;
    // Abre um arquivo TEXTO para LEITURA
    arq = fopen("entrada.txt", "r");
    if (arq == NULL)  // Se houve erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        exit(-1);
    }
    while (!feof(arq))
    {
        sem_wait(&slotVazioBuffer1); // Espera o Buffer 1 ficar completamente vazio
        result = fgets(buffer1, N, arq);  // Lê um chunk do arquivo e armazena no Buffer 1
        if (result == NULL) {  // Se chegou ao fim do arquivo
            buffer1[0] = '\0';
        }
        sem_post(&slotCheioBuffer1); // Avisa que o Buffer 1 está cheio (Thread 2)
    }
    fclose(arq);
    pthread_exit(NULL);
}

void * Processa (void*) {
    int i, j, count=0, block_size, n = 0, fim = 1, tam_buffer1;

    while(fim){
        i = 0;
        j = 0;

        sem_wait(&slotCheioBuffer1); // Espera o Buffer 1 estar cheio
        sem_wait(&slotVazioBuffer2); // Espera o Buffer 2 estar vazio 
        tam_buffer1 = strlen(buffer1);
        
        while (i < tam_buffer1) {
            if (n <= 10) {
                block_size = 2 * n + 1;
                for (; count < block_size && i < tam_buffer1; count++) {
                    buffer2[j++] = buffer1[i++];  // Copia os caracteres para Buffer2
                }
                if(count == block_size){
                    buffer2[j++] = '\n';  // Adiciona '\n' após o bloco
                    count = 0;
                    n++;  // Incrementa o valor de n até 11
                }
            } else {
                // Para n >= 10, insere a cada 10 caracteres
                for (; count < 10 && i < tam_buffer1; count++) {
                    buffer2[j++] = buffer1[i++];  // Copia os caracteres para Buffer2
                }
                if(count == 10){
                    count = 0;
                    buffer2[j++] = '\n';
                }
            }
        }
        buffer2[j++] = '\0';
        
        if(tam_buffer1 + 1 != N){ // Verifica se o Buffer 1 não veio completamente cheio
            buffer2[j] = '!'; // Adiciona um delimitador no Buffer 2 para indicar que a Thread 3 pode finalizar
            fim = 0; // Sinaliza que a Thread 2 pode finalizar
        }

        sem_post(&slotVazioBuffer1); // Sinaliza que o Buffer 1 está vazio (para a Thread 1)
        sem_post(&slotCheioBuffer2); //Sinaliza que o Buffer 2 está cheio (para a Thread 3)
    }
    pthread_exit(NULL);
}

void * Imprime(void*){
    int fim = 1, tam_buffer2;

    while(fim){
        sem_wait(&slotCheioBuffer2); // Espera o Buffer 2 ficar cheio
        printf("%s", buffer2);
        tam_buffer2 = strlen(buffer2);  // Calcula o comprimento da string até o primeiro '\0'
        
        if (buffer2[tam_buffer2 + 1] == '!') { // Verifica se existe o delimitador, que marca o fim do processamento 
            fim = 0;
        }
        
        sem_post(&slotVazioBuffer2); //manda sinal dizendo que ja consumiu/printou buffer 2
    }
    pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[]) {
    pthread_t *tid; //identificadores das threads no sistema

    sem_init(&slotCheioBuffer1, 0, 0);
    sem_init(&slotVazioBuffer1, 0, 1);
    sem_init(&slotCheioBuffer2, 0, 0);
    sem_init(&slotVazioBuffer2, 0, 1);

   //--aloca as estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*(3));
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}

   //--cria as threads
   
    if (pthread_create(&tid[0], NULL, Insere, NULL)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
    }
    
    if (pthread_create(&tid[1], NULL, Processa, NULL)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
    }

    if (pthread_create(&tid[2], NULL, Imprime, NULL)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
    }

   //--espera todas as threads terminarem
   for (int t=0; t<2; t++) {
     if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1);
     }
   }

   //--finaliza o semaforo
   sem_destroy(&slotCheioBuffer1);
   sem_destroy(&slotVazioBuffer1);
   sem_destroy(&slotCheioBuffer2);
   sem_destroy(&slotVazioBuffer2);

   return 0;
}