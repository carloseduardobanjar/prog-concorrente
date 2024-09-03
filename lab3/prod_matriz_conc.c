#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *matA, *matB, *matC; 
int nthreads; 

typedef struct{
   int id;
   int rowsA;
   int colsA;
   int colsB;
} tArgs;

void * CalculaProdutoMatriz(void *arg) {
   int i, j, k;
   tArgs *args = (tArgs*) arg;
   int rowsA = args->rowsA;
   int colsA = args->colsA;
   int colsB = args->colsB;

   for(i = args->id; i < rowsA; i += nthreads) {
      for(j = 0; j < colsB; j++) {
         matC[i * colsB + j] = 0;
         for(k = 0; k < colsA; k++) {
               matC[i * colsB + j] += matA[i * colsA + k] * matB[k * colsB + j];
         }
      }
   }
   pthread_exit(NULL);
}

int le_arquivo(char *arquivo, float **matriz, int *rows, int *cols){
   FILE * descritorArquivo;
   int ret, tam;
   descritorArquivo = fopen(arquivo, "rb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 2;
   }

   ret = fread(rows, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
      return 3;
   }
   ret = fread(cols, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
      return 3;
   }
   tam = *(rows) * *(cols);

   *matriz = (float*) malloc(sizeof(float) * tam);
   if(!(*matriz)) {
      fprintf(stderr, "Erro de alocao da memoria da matriz\n");
      return 3;
   }

   ret = fread(*matriz, sizeof(float), tam, descritorArquivo);
   if(ret < tam) {
      fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
      return 4;
   }
   return 0;
}

int main(int argc, char* argv[]) {
   pthread_t *tid;
   tArgs *args;
   int rowsA, colsA, rowsB, colsB, i;
   char *arquivo1, *arquivo2;
   FILE * descritorArquivo;
   char arquivo_saida[150];
   double inicio, fim, delta;

   GET_TIME(inicio);
   if(argc<4) {
      printf("Digite: %s <arquivo-entrada> <arquivo-entrada> <numero de threads>\n", argv[0]);
      return 1;
   }
   arquivo1 = argv[1];
   arquivo2 = argv[2];
   nthreads = atoi(argv[3]);
   
   le_arquivo(arquivo1, &matA, &rowsA, &colsA);
   le_arquivo(arquivo2, &matB, &rowsB, &colsB);

   if (nthreads > rowsA) nthreads=rowsA;

   if (colsA != rowsB){
      printf("Dimensões incompatíveis\n");
      return 2;
   }

   matC = (float *) malloc(sizeof(float) * rowsA * colsB);
   if (matC == NULL) {printf("ERRO--malloc\n"); return 2;}

   GET_TIME(fim);
   delta = fim - inicio;
   printf("Tempo inicializacao: %lf\n", delta);

   GET_TIME(inicio);
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}
   args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args==NULL) {puts("ERRO--malloc"); return 2;}

   for(i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->rowsA = rowsA;
      (args+i)->colsA = colsA;
      (args+i)->colsB = colsB;
      if(pthread_create(tid+i, NULL, CalculaProdutoMatriz, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   for(i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   GET_TIME(fim);
   delta = fim - inicio;
   printf("Tempo multiplicacao (dimensao %dx%d) (nthreads %d): %lf\n", rowsA, colsB, nthreads, delta);

   sprintf(arquivo_saida, "saidaconc%d-%d.bin", rowsA, colsB);

   descritorArquivo = fopen(arquivo_saida, "wb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 3;
   }
   fwrite(matC, sizeof(float), rowsA * colsB, descritorArquivo);
   fclose(descritorArquivo);

   GET_TIME(inicio);
   free(matA);
   free(matB);
   free(matC);
   free(args);
   free(tid);
   GET_TIME(fim)   
   delta = fim - inicio;
   printf("Tempo finalizacao: %lf\n", delta);

   return 0;
}
