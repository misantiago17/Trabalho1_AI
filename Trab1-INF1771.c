#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "lerArquivos.h"

// Genetic Algorithm
// Inicialize a população com candidatos randomicos
// Avalie cada candidato
// Repita até condição de termino
//	1 - Selecione os pais
// 	2 - Recombine o par de pais
//	3 - Mutacione o reusltado da troca
//	4 - Avalie novos candidatos
//	5 - Selecione individuos para a proxima geração

// Simulated Annealing 

int temperature;
int iteration;

int objetivo (int * sol, int **mat, int t){
	int cont = 0;
	for (int i = 0; i < t-1; i++){
		
		cont += mat[sol[i]][sol[i+1]];
	}
	return cont;
}

int simulatedAnnealing(int delta, int **mat, int t){
	double deltatemp = -1 * delta/temperature;
	double p = exp(deltatemp);

	if (p < 1) return 0;
	else return 1;
}

int *geraSolInicial(int t) {

	int *v = (int *) malloc (t* sizeof(int));
	for (int i=0; i< t; i++){
		v[i] = i;
	}
	return v;

	// Rever isso - fazer gulosa talvez
	/*int posicionou = 0;

	int *v = (int *) malloc (t* sizeof(int));
	for (int i=t-1; i >= 0; i--){

		while(posicionou == 0){
			int num = (rand() % (t + 1 - 0) + 0);

			if (v[num] == 0){
				v[num] = i;
				posicionou = 1;
			}
		}
		posicionou = 0;
	}
	return v;*/
}

// Realiza o Swap completo e retorna o melhor vizinho
int *bestNeighbour(int **m, int t, int *s){ 

	int *vizinhanca = (int *) malloc (t* sizeof(int));
	int *melhorVizinhanca = (int *) malloc (t* sizeof(int));
	int VizinhoTrocado;
	int contIteration = 0;

	for (int h=0; h<t; h++)
		melhorVizinhanca[h] = s[h];

	// pega um valor do vetor original
	for (int i=0; i<t; i++){

		VizinhoTrocado = s[i];

		// faz a troca desse valor em cada uma das posicoes do vetor
		for (int j=0; j<t; j++){

			contIteration++;
			if (contIteration == iteration){
				temperature -= 1; // diminui valor da temperatura a iterações do numero de cidades
				contIteration = 0;
				if(temperature == 0) break;
			}

			// reinicia o vetor a cada loop de troca 
			for (int h=0; h<t; h++)
				vizinhanca[h] = s[h];

			// Faz a troca sem repetição (>)
			// Faz a funcao objetiva do vetor original uma única vez (j=i && i=0)
			if (j > i || (j==i && i==0)){ 	
				
				vizinhanca[i] = vizinhanca[j];
				vizinhanca[j] = VizinhoTrocado;
				int objVizinho = objetivo(vizinhanca,m,t);
				int objMelhorVizinho = objetivo(melhorVizinhanca,m,t);
				// Dado o vetor de vizinhos criado, compara com a melhor vizinhança

				if (objVizinho <= objMelhorVizinho){
					for (int i = 0; i < t; i++){
						melhorVizinhanca[i] = vizinhanca[i];
					}
				} else {
					//chama simulated pra ver se aceita com a probabilidade menor
					int delta = objVizinho - objMelhorVizinho;
					if(simulatedAnnealing(delta, m, t)){
						for (int i = 0; i < t; i++){
							melhorVizinhanca[i] = vizinhanca[i];
						}
					}
				}
			}
		}
		if(temperature == 0) break;
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
	temperature = quantidadeCidades * 5;
	iteration = pow(quantidadeCidades,2);

	clock_t Ticks[2];
    Ticks[0] = clock();

    solCorrente = geraSolInicial(quantidadeCidades);

    while (temperature > 0){

    	solCorrente = bestNeighbour(matrizDistancias, quantidadeCidades, solCorrente);

		printf("\nobjetivo: %d ", objetivo(solCorrente,matrizDistancias,quantidadeCidades));
    }
    //chamada funcao metaheuristica

    printf("\n\nObjetivo da melhor solução : %d\n", objetivo(solCorrente,matrizDistancias, quantidadeCidades));
    puts("Melhor Solução: ");
    for (int i = 0; i < quantidadeCidades;i++){
    	  printf("%d ", solCorrente[i]);
    }


	// Clock da CPU
    Ticks[1] = clock();
    double Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %g ms.\n", Tempo);


	// Fecha o arquivo lido
	fechaArquivo();
	return 0;
}