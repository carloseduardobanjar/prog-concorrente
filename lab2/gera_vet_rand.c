#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000
#define TEXTO 

int main(int argc, char*argv[]) {
   float *vetor1, *vetor2;
   long int n;
   float elem1, elem2;
   int fator=1;
   FILE * descritorArquivo;
   size_t ret;
   double prod_interno = 0;

   if(argc < 3) {
      fprintf(stderr, "Digite: %s <dimensao> <arquivo saida>\n", argv[0]);
      return 1;
   }
   n = atoi(argv[1]);

   vetor1 = (float*) malloc(sizeof(float) * n);
   vetor2 = (float*) malloc(sizeof(float) * n);
   if(!vetor1 || !vetor2) {
      fprintf(stderr, "Erro de alocao da memoria do vetor\n");
      return 2;
   }

   srand(time(NULL));
   for(long int i=0; i<n; i++) {
        elem1 = (rand() % MAX)/3.0 * fator;
        elem2 = (rand() % MAX)/3.0 * fator;
        vetor1[i] = elem1;
        vetor2[i] = elem2;
        prod_interno += vetor1[i] * vetor2[i];
        fator*=-1;
   }

   #ifdef TEXTO
   fprintf(stdout, "Valor de n: %ld\n", n);
   fprintf(stdout, "Vetor 1: ");
   for(long int i=0; i<n; i++) {
      fprintf(stdout, "%f ",vetor1[i]);
   }
   fprintf(stdout, "\nVetor 2: ");
   for(long int i=0; i<n; i++) {
      fprintf(stdout, "%f ",vetor2[i]);
   }
   fprintf(stdout, "\n");
   fprintf(stdout, "Produto interno=%lf\n", prod_interno);
   #endif

   descritorArquivo = fopen(argv[2], "wb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 3;
   }
   ret = fwrite(&n, sizeof(long int), 1, descritorArquivo);
   ret = fwrite(vetor1, sizeof(float), n, descritorArquivo);
   ret = fwrite(vetor2, sizeof(float), n, descritorArquivo);
   if(ret < n) {
      fprintf(stderr, "Erro de escrita no  arquivo\n");
      return 4;
   }
   ret = fwrite(&prod_interno, sizeof(double), 1, descritorArquivo);

   fclose(descritorArquivo);
   free(vetor1);
   free(vetor2);
   return 0;
}