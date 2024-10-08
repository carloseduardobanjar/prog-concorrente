# Laboratório 6

## Descrição

Este programa implementa o problema de leitores e escritores com a variação que as operações de escrita têm prioridade sobre as operações de leitura.

## Comportamento do Programa

- Leitura pode ocorrer simultaneamente com outras operações de leitura.
- Escrita é uma operação exclusiva e requer acesso exclusivo ao recurso compartilhado. Nenhuma leitura ou outra escrita pode ocorrer enquanto um escritor está escrevendo.
- Prioridade para escritores: Quando um escritor deseja escrever, novas operações de leitura são bloqueadas até que o escritor tenha terminado. Isso garante que escritores não fiquem em estado de espera indefinidamente se houver muitos leitores.

## Saída do programa

O programa gera logs com as operações realizadas. As colunas do log são:

- Leit/Escr: Define se a operação é de leitura ou escrita.
- Identificador: Um número único para identificar o leitor ou o escritor.
- Ação: O status atual da operação (chegando, lendo/escrevendo, aguardando ou saindo).
- O símbolo (*) indica que o motivo da espera é a regra de prioridade de escrita.

## Exemplo

Compilamos o programa com:

```
gcc -o main main_rwlock.c list_int.c 
```

Em seguida, executamos o programa com 6 threads e obtivemos a seguinte saída:

Nº | Leit/Escr | Identificador | Ação           |
---|-----------|---------------|----------------|
1 | Leitor    | 0             | Chegando       |
2 | Leitor    | 0             | Lendo          |
3 | Leitor    | 0             | Saindo         |
4 | Leitor    | 6             | Chegando       |
5 | Leitor    | 6             | Lendo          |
6 | Leitor    | 6             | Saindo         |
7 | Leitor    | 12            | Chegando       |
8 | Leitor    | 12            | Lendo          |
9 | Leitor    | 2             | Chegando       |
10| Leitor    | 2             | Lendo          |
11| Leitor    | 3             | Chegando       |
12| Leitor    | 3             | Lendo          |
13| Leitor    | 3             | Saindo         |
14| Leitor    | 9             | Chegando       |
15| Leitor    | 9             | Lendo          |
16| Leitor    | 1             | Chegando       |
17| Leitor    | 1             | Lendo          |
18| Leitor    | 2             | Saindo         |
19| Leitor    | 8             | Chegando       |
20| Leitor    | 8             | Lendo          |
21| Leitor    | 4             | Chegando       |
22| Leitor    | 4             | Lendo          |
23| Leitor    | 1             | Saindo         |
24| Leitor    | 7             | Chegando       |
25| Leitor    | 7             | Lendo          |
26| Leitor    | 7             | Saindo         |
27| Leitor    | 13            | Chegando       |
28| Leitor    | 13            | Lendo          |
29| Leitor    | 13            | Saindo         |
30| Leitor    | 19            | Chegando       |
31| Leitor    | 19            | Lendo          |
32| Leitor    | 19            | Saindo         |
33| Leitor    | 25            | Chegando       |
34| Leitor    | 25            | Lendo          |
35| Leitor    | 25            | Saindo         |
36| Leitor    | 31            | Chegando       |
37| Leitor    | 31            | Lendo          |
38| Leitor    | 4             | Saindo         |
39| Leitor    | 5             | Chegando       |
40| Leitor    | 5             | Lendo          |
41| Leitor    | 5             | Saindo         |
42| Leitor    | 11            | Chegando       |
43| Leitor    | 11            | Lendo          |
44| Leitor    | 11            | Saindo         |
45| Leitor    | 17            | Chegando       |
46| Leitor    | 17            | Lendo          |
47| Leitor    | 10            | Chegando       |
48| Leitor    | 10            | Lendo          |
49| Leitor    | 10            | Saindo         |
50| Leitor    | 16            | Chegando       |
51| Leitor    | 16            | Lendo          |
52| Leitor    | 16            | Saindo         |
53| Leitor    | 22            | Chegando       |
54| Leitor    | 22            | Lendo          |
55| Leitor    | 22            | Saindo         |
56| Leitor    | 28            | Chegando       |
57| Leitor    | 28            | Lendo          |
58| Leitor    | 8             | Saindo         |
59| Leitor    | 14            | Chegando       |
60| Leitor    | 14            | Lendo          |
61| Leitor    | 14            | Saindo         |
62| Leitor    | 20            | Chegando       |
63| Leitor    | 20            | Lendo          |
64| Leitor    | 12            | Saindo         |
65| Leitor    | 28            | Saindo         |
66| Leitor    | 9             | Saindo         |
67| Leitor    | 15            | Chegando       |
68| Leitor    | 15            | Lendo          |
69| Leitor    | 15            | Saindo         |
70| Escritor  | 21            | Chegando       |
71| Escritor  | 21            | Aguardando     |
72| Leitor    | 18            | Chegando       |
73| Leitor    | 18            | Aguardando (*) |
74| Leitor    | 31            | Saindo         |
75| Leitor    | 37            | Chegando       |
76| Leitor    | 37            | Aguardando (*) |
77| Leitor    | 17            | Saindo         |
78| Leitor    | 23            | Chegando       |
79| Leitor    | 23            | Aguardando (*) |
80| Leitor    | 20            | Saindo         |
81| Leitor    | 26            | Chegando       |
82| Leitor    | 26            | Aguardando (*) |
83| Escritor  | 21            | Escrevendo     |
84| Escritor  | 21            | Saindo         |
85| Leitor    | 37            | Lendo          |
86| Leitor    | 26            | Lendo          |
87| Leitor    | 34            | Chegando       |
88| Leitor    | 34            | Lendo          |
89| Leitor    | 27            | Chegando       |
90| Leitor    | 27            | Lendo          |
100| Leitor    | 27            | Saindo         |
101| Leitor    | 33            | Chegando       |
102| Leitor    | 33            | Lendo          |
103| Leitor    | 33            | Saindo         |
104| Leitor    | 39            | Chegando       |
105| Leitor    | 39            | Lendo          |
106| Leitor    | 39            | Saindo         |
107| Leitor    | 45            | Chegando       |
108| Leitor    | 45            | Lendo          |
109| Leitor    | 45            | Saindo         |
110| Leitor    | 18            | Lendo          |
111| Leitor    | 18            | Saindo         |
112| Leitor    | 24            | Chegando       |
113| Leitor    | 24            | Lendo          |
114| Leitor    | 24            | Saindo         |
115| Leitor    | 30            | Chegando       |
116| Leitor    | 30            | Lendo          |
117| Leitor    | 30            | Saindo         |
118| Leitor    | 36            | Chegando       |
119| Leitor    | 36            | Lendo          |
120| Leitor    | 37            | Saindo         |
121| Leitor    | 43            | Chegando       |
122| Leitor    | 43            | Lendo          |
123| Leitor    | 43            | Saindo         |
124| Leitor    | 49            | Chegando       |
125| Leitor    | 49            | Lendo          |


Observamos que o Escritor 21 solicitou uma operação de escrita (chegando) na linha 71. Nesse momento, já havia operações de leitura sendo realizadas por diversos leitores, e a operação de escrita foi colocada em espera.

Entretanto, assim que o Escritor 21 solicitou a escrita, os leitores que chegaram após essa solicitação, como o Leitor 18 (linha 72), Leitor 37 (linha 75), e outros, ficaram em estado de aguardando ("Aguardando (*)" no log). Isso mostra que novas operações de leitura não puderam começar até que o Escritor 21 fosse atendido.

Assim que o Escritor 21 terminou de escrever (linha 84), os leitores que estavam aguardando (como Leitor 37, Leitor 26) puderam começar a ler novamente. Isso demonstra que o programa deu prioridade à operação de escrita, impedindo que novos leitores iniciassem suas leituras até que a escrita fosse concluída.