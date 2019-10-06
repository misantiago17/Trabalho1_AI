#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lerArquivos.h"

// Simulated Annealing 
// 1 - Escolha um estado inicial s (por algoritmo guloso?)
// 2 - Escolha aleatoriamente um estado t, a partir dos vizinhos de s
// 3 - Se f(t) for melhor do que f(s), então s = t
// 4 - senão, com uma probabilidade baixa, faça s = t
// 5 - Repete o passo 2 até parar


int *geraSolInicial(int t) {

	// Fazer algoritmo guloso

	int *v = (int *) malloc (t* sizeof(int));
	for (int i=0; i< t; i++){
		v[i] = i;
	}
	return v;
}

// Quanto que  solução vale no total - repensar na função objetivo
int objetivo (int * sol, int **mat, int t){

	int cont = 0;
	for (int i = 0; i < t; i++){
		cont += mat[i][i+1];
	}
	return cont;
}


// Realiza o Swap completo e retorna o melhor vizinho
int *bestNeighbour(int **m, int t, int *s){ 

	int *vizinhanca = (int *) malloc (t* sizeof(int));
	int *melhorVizinhanca = s;
	int VizinhoTrocado;

	// pega um valor do vetor original
	for (int i=0; i<t; i++){

		VizinhoTrocado = s[i];

		// faz a troca desse valor em cada uma das posicoes do vetor
		for (int j=0; j<t; j++){

			// reinicia o vetor a cada loop de troca 
			for (int h=0; h<t; h++)
				vizinhanca[h] = s[h];

			// Faz a troca sem repetição (>)
			// Faz a funcao objetiva do vetor original uma única vez (j=i && i=0)
			if (j > i || (j==i && i==0)){ 	

				vizinhanca[i] = vizinhanca[j];
				vizinhanca[j] = VizinhoTrocado;

				printf("resposta Objetivo da vizinhanca: %d\n",objetivo(vizinhanca,m,t));
				printf("resposta Objetivo melhor vizinhanca: %d\n",objetivo(melhorVizinhanca,m,t));

				// Dado o vetor de vizinhos criado, compara com a melhor vizinhança
				if (objetivo(vizinhanca,m,t) < objetivo(melhorVizinhanca,m,t)){
					melhorVizinhanca = vizinhanca;
				}
			}
		}
	}

	free(vizinhanca);

	return melhorVizinhanca;
}

int main(int argc, char *argv[]) {

	int **matrizDistancias; // Matriz contendo a distância entre as cidades
	int quantidadeCidades = 0; // Numero de cidades envolvidas no arquivo

    int *solCorrente;

	// faz a leitura, recebe matriz de distâncias do arquivo escolhido e le a quantidade de cidades envolvidas no arquivo
	escolherArquivo();
	matrizDistancias = leMatrizdistancias();
	quantidadeCidades = leNumeroCidades();

	// Passar isso para alguma função
	for(int i = 0; i < quantidadeCidades; i++){
        for (int j = 0; j < quantidadeCidades; j++){

            if (i > j){
                printf("Matriz Distancias[%d][%d]: %d\n\n",i,j,matrizDistancias[i][j]);
            }
        }
    }

	clock_t Ticks[2];
    Ticks[0] = clock();
    //solCorrente = geraSolInicial(qtdCidades);
    //solCorrente = bestNeighbour(matrizDistancias, qtdCidades, solCorrente);
    //chamada funcao metaheuristica

	// Clock da CPU
    Ticks[1] = clock();
    double Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    printf("Tempo gasto: %g ms.\n", Tempo);

    /*for (i=0; i < qtdCidades;i++)
    	printf("%d ", solCorrente[i]);

	printf("\n");*/

	// Fecha o arquivo lido
	fechaArquivo();

	return 0;
}
