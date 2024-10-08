//Programa concorrente que cria e faz operacoes sobre uma lista de inteiros

#include <stdio.h>
#include <stdlib.h>
#include "list_int.h"
#include <pthread.h>
#include "timer.h"

#define QTDE_OPS 50 //quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 100 //quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 //valor maximo a ser inserido

//lista compartilhada iniciada 
struct list_node_s* head_p = NULL; 
//qtde de threads no programa
int nthreads;

pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;
int leit=0, escr=0, querEscrever = 0;

void gera_log(const char* papel, int id, const char* acao) {
   printf("| %-9s | %-13d | %-14s |\n", papel, id, acao);
}

void EntraLeitura(int id) {
   pthread_mutex_lock(&mutex);
   gera_log("Leitor", id, "Chegando");
   while(escr > 0 || querEscrever) {
      if(escr == 0 && querEscrever){
         gera_log("Leitor", id, "Aguardando (*)");
      } else{
         gera_log("Leitor", id, "Aguardando");
      }
      
      pthread_cond_wait(&cond_leit, &mutex);
   }
   gera_log("Leitor", id, "Lendo");
   leit++;
   pthread_mutex_unlock(&mutex);
}

void SaiLeitura(int id) {
   pthread_mutex_lock(&mutex);
   gera_log("Leitor", id, "Saindo");
   leit--;
   if(leit==0) pthread_cond_signal(&cond_escr);
   pthread_mutex_unlock(&mutex);
}

void EntraEscrita(int id) {
   pthread_mutex_lock(&mutex);
   querEscrever++;
   gera_log("Escritor", id, "Chegando");
   while((leit>0) || (escr>0)) {
      gera_log("Escritor", id, "Aguardando");
      pthread_cond_wait(&cond_escr, &mutex);
   }
   gera_log("Escritor", id, "Escrevendo");
   querEscrever--;
   escr++;
   pthread_mutex_unlock(&mutex);
}

void SaiEscrita(int id) {
   pthread_mutex_lock(&mutex);
   gera_log("Escritor", id, "Saindo");
   escr--;
   pthread_cond_signal(&cond_escr);
   pthread_cond_broadcast(&cond_leit);
   pthread_mutex_unlock(&mutex);
}

//tarefa das threads
void* tarefa(void* arg) {
   long int id = (long int) arg;
   int op;
   int in, out, read; 
   in=out=read = 0; 

   //realiza operacoes de consulta (98%), insercao (1%) e remocao (1%)
   for(long int i=id; i<QTDE_OPS; i+=nthreads) {
      op = rand() % 100;
      if(op<98) {
	      EntraLeitura(i);
         Member(i%MAX_VALUE, head_p);   /* Ignore return value */
	      SaiLeitura(i);         
	      read++;
      } else if(98<=op && op<99) {
         EntraEscrita(i);  
         Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
         SaiEscrita(i);    
	      in++;
      } else if(op>=99) {
	      EntraEscrita(i);     
         Delete(i%MAX_VALUE, &head_p);  /* Ignore return svalue */
	      SaiEscrita(i);    
	      out++;
      }
   }
   //registra a qtde de operacoes realizadas por tipo
   // printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
   pthread_exit(NULL);
}

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   pthread_t *tid;
   double ini, fim, delta;

   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&cond_leit,NULL);
   pthread_cond_init(&cond_escr,NULL);
   
   //verifica se o numero de threads foi passado na linha de comando
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]); return 1;
   }
   nthreads = atoi(argv[1]);

   //insere os primeiros elementos na lista
   for(int i=0; i<QTDE_INI; i++)
      Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
   

   //aloca espaco de memoria para o vetor de identificadores de threads no sistema
   tid = malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {  
      printf("--ERRO: malloc()\n"); return 2;
   }

   //tomada de tempo inicial
   GET_TIME(ini);

   printf("|--------------------------------------------|\n");
   printf("| %-9s | %-13s | %-16s |\n", "Leit/Escr", "Identificador", "Ação");
   printf("|-----------|---------------|----------------|\n");
   
   //cria as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
         printf("--ERRO: pthread_create()\n"); return 3;
      }
   }
   
   //aguarda as threads terminarem
   for(int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), NULL)) {
         printf("--ERRO: pthread_join()\n"); return 4;
      }
   }
   printf("|--------------------------------------------|\n");

   //tomada de tempo final
   GET_TIME(fim);
   delta = fim-ini;
   printf("Tempo: %lf\n", delta);

   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&cond_leit);
   pthread_cond_destroy(&cond_escr);

   //libera o espaco de memoria do vetor de threads
   free(tid);
   //libera o espaco de memoria da lista
   Free_list(&head_p);

   return 0;
}  /* main */

