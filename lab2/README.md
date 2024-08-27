## Introdução

Este projeto compara a precisão entre duas abordagens para o cálculo do produto interno de dois vetores: uma abordagem concorrente e uma abordagem sequencial, focando na variação relativa entre elas.


## Métodos Utilizados

Concorrente: utiliza múltiplas threads para dividir o trabalho do cálculo do produto interno.

Sequencial: realiza o cálculo do produto interno em uma único thread, processando os elementos do vetor um por um.

## Resultados Obtidos

Abaixo estão os resultados obtidos para diferentes tamanhos de vetor (usando 4 threads):

**Tamanho do vetor**: 100  
**Produto interno concorrente**: 2.596.558,75  
**Produto interno sequencial:** 2.596.558,659  
**Variação relativa**: 0.00000003504623435901521589

**Tamanho do vetor**: 1.000  
**Produto interno concorrente**: 27.452.436,00  
**Produto interno sequencial:** 27.452.437,002  
**Variação relativa**: 0.00000003650865920690193889

**Tamanho do vetor**: 10.000  
**Produto interno concorrente**: 279.164.800,00  
**Produto interno sequencial:** 279.164.767,815  
**Variação relativa**: 0.00000011529201060511695687 

**Tamanho do vetor**: 100.000  
**Produto interno concorrente**: 2784972288,00  
**Produto interno sequencial:** 2784974024,986  
**Variação relativa**: 0.00000062369934994421782903  

**Tamanho do vetor**: 1.000.000  
**Produto interno concorrente**: 27701680128,00  
**Produto interno sequencial:** 27702127664,796  
**Variação relativa**: 0.00001615532164578326046467  

**Tamanho do vetor**: 10.000.000  
**Produto interno concorrente**: 277009694720,00  
**Produto interno sequencial:** 277220690696,041  
**Variação relativa**: 0.00076111190719529986381531  

## Análise dos Resultados

A variação relativa entre os resultados concorrentes e sequenciais é extremamente pequena. Essa diferença pode ser atribuída à precisão de ponto flutuante e à ordem em que as operações de soma são realizadas nas threads concorrentes.

Conforme o tamanho do vetor aumenta, a variação relativa entre as abordagens concorrente e sequencial também aumenta. Isso sugere que a imprecisão acumulada pode crescer com o tamanho do vetor.
