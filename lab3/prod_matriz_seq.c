#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"
#include <string.h>

float *matA, *matB, *matC;

int leMatrizBinario(char *arquivo, float **matriz, int *rows, int *cols){
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

void multiplica_matrizes(float *matA, float *matB, float *matC, int rowsA, int colsA, int colsB) {
   int i, j, k;
    for (i = 0; i < rowsA; i++) {
        for (j = 0; j < colsB; j++) {
            matC[i * colsB + j] = 0;
            for (k = 0; k < colsA; k++) {
                  matC[i * colsB + j] += matA[i * colsA + k] * matB[k * colsB + j];
            }
        }
    }
}

int main(int argc, char* argv[]) {
   int rowsA, colsA, rowsB, colsB;
   char *arquivo1, *arquivo2;
   char arquivo_saida[150];
   FILE * descritorArquivo;
   double inicio, fim, delta;
   
   GET_TIME(inicio);
   if(argc<3) {
      printf("Digite: %s <arquivo-entrada> <arquivo-entrada>>\n", argv[0]);
      return 1;
   }
   arquivo1 = argv[1];
   arquivo2 = argv[2];

   leMatrizBinario(arquivo1, &matA, &rowsA, &colsA);
   leMatrizBinario(arquivo2, &matB, &rowsB, &colsB);

   if (colsA != rowsB){
      printf("Dimensões incompatíveis\n");
   }

   matC = (float *) malloc(sizeof(float) * rowsA * colsB);
   if (matC == NULL) {printf("ERRO--malloc\n"); return 2;}

   GET_TIME(fim);
   delta = fim - inicio;
   printf("Tempo inicializacao: %lf\n", delta);

   GET_TIME(inicio);
   multiplica_matrizes(matA, matB, matC, rowsA, colsA, colsB);
   GET_TIME(fim)   
   delta = fim - inicio;
   printf("Tempo multiplicacao (dimensao %dx%d): %lf\n", rowsA, colsB, delta);
   
   sprintf(arquivo_saida, "saidaseq%d-%d.bin", rowsA, colsB);

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
   GET_TIME(fim)   
   delta = fim - inicio;
   printf("Tempo finalizacao: %lf\n", delta);

   return 0;
}
