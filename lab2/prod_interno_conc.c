/* Disciplina: Computacao Concorrente */
/* Profa.: Silvana Rossetto */
/* Lab2:  */
/* Codigo: Soma todos os elementos de um vetor de floats */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 

long int n;
float *vetor1, *vetor2;
int nthreads;

void *CalculaProdutoInterno (void *tid) {
   long int id = (long int) tid;
   int ini, fim, bloco;
   float prod_interno_local = 0, *ret;

   bloco = n/nthreads;
   ini = id*bloco;
   fim = ini + bloco;
   if (id==(nthreads-1)) fim = n;

   for(int i=ini; i<fim; i++) {
      prod_interno_local += vetor1[i] * vetor2[i];
   }

   ret = malloc(sizeof(float));
   if (ret!=NULL) *ret = prod_interno_local;
   else printf("--ERRO: malloc() thread\n");
   pthread_exit((void*) ret);
}

int main(int argc, char *argv[]) {
   FILE *arq;
   size_t ret;
            
   double prod_interno_seq;
   
   float prod_interno_global=0, *prod_interno_retorno_threads, var_relativa;

   pthread_t *tid_sistema;

   if(argc < 3) { printf("Use: %s <arquivo de entrada> <numero de threads> \n", argv[0]); exit(-1); }

   arq = fopen(argv[1], "rb");
   if(arq==NULL) { printf("--ERRO: fopen()\n"); exit(-1); }

   ret = fread(&n, sizeof(long int), 1, arq);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
      return 3;
   }

   vetor1 = malloc (sizeof(float) * n);
   if(vetor1==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }
   ret = fread(vetor1, sizeof(float), n, arq);
   if(ret < n) {
      fprintf(stderr, "Erro de leitura dos elementos do vetor\n");
      return 4;
   }
   vetor2 = malloc (sizeof(float) * n);
   if(vetor2==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }
   ret = fread(vetor2, sizeof(float), n, arq);
   if(ret < n) {
      fprintf(stderr, "Erro de leitura dos elementos do vetor\n");
      return 4;
   }

   nthreads = atoi(argv[2]);
   if(nthreads>n) nthreads = n;

   tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid_sistema==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }

   for(long int i=0; i<nthreads; i++) {
      
      if (pthread_create(&tid_sistema[i], NULL, CalculaProdutoInterno, (void*) i)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
      }
   }

   for(int i=0; i<nthreads; i++) {
         if (pthread_join(tid_sistema[i], (void *) &prod_interno_retorno_threads)) {
            printf("--ERRO: pthread_join()\n"); exit(-1);
         }
         prod_interno_global += *prod_interno_retorno_threads;
         free(prod_interno_retorno_threads);
   }

   ret = fread(&prod_interno_seq, sizeof(double), 1, arq); 
   var_relativa = (prod_interno_seq - prod_interno_global) / prod_interno_seq;

   printf("Produto interno concorrente: %.26f\n", prod_interno_global);
   printf("Produto interno sequencial: %.26lf\n", prod_interno_seq);
   printf("Variação relativa: %.26lf\n", var_relativa);

   free(vetor1);
   free(vetor2);
   free(tid_sistema);
   fclose(arq);
   return 0;
}
