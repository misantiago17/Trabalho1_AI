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

int *geraSolInicialRandom(int t){

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

int *geraSolInicial(int t) {

	int *v = (int *) malloc (t* sizeof(int));
	for (int i=0; i< t; i++){
		v[i] = i;
	}
	return v;
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

int *bubbleSort(int *v, int tam){ 
  int aux, i, j; 
 
  for(j=tam-1; j<=1; j--)
  { 
    for(i=0; i>j; i++)
    { 
      if(v[i] > v[i+1])
      { 
        aux=v[i]; 
        v[i]=v[i+1]; 
        v[i+1]=aux; 
      } 
    } 
  } 
  return v;
}

float *roulette( int objindividuo1, int objindividuo2, int objindividuo3, int objindividuo4){
	int first, second, third, fourth;
	int vet[4] = {objindividuo1, objindividuo2, objindividuo3, objindividuo4};
	int *ordenado = bubbleSort(vet, 4);
	int dif = ordenado[3] - ordenado[0];
	int soma = ordenado[0]+ordenado[1]+ordenado[2]+ordenado[3];
	float *prop = (float *) malloc (4* sizeof(float));
	for(int i = 0; i < 4; i++){
		printf("\n%d ", soma);
		prop[i] = (ordenado[i]*100)/soma;
		printf("\n%d ", prop[i]);
	}
	return prop;
}	


int main(int argc, char *argv[]) {
	int **matrizDistancias; // Matriz contendo a distância entre as cidades
	int quantidadeCidades = 0; // Numero de cidades envolvidas no arquivo
	int *solCorrente;
	int metaheuristica = 0;
	clock_t Ticks[2];
    printf("1 - Simulated Annealing\n2 - Genetic Algorithm\n");
	printf("\nEscolha o método que deseja utilizar para resolver o Problema do Caxeiro Viajante: ");
    scanf("%d", &metaheuristica);
	printf("\n");


	// faz a leitura, recebe matriz de distâncias do arquivo escolhido e le a quantidade de cidades envolvidas no arquivo
	escolherArquivo();
	matrizDistancias = leMatrizdistancias();
	quantidadeCidades = leNumeroCidades();

	if(metaheuristica == 1){ //SIMULATED ANNEALING
		temperature = quantidadeCidades * 5;
		iteration = pow(quantidadeCidades,2);

	    Ticks[0] = clock();

	    solCorrente = geraSolInicial(quantidadeCidades);

	    while (temperature > 0){

	    	solCorrente = bestNeighbour(matrizDistancias, quantidadeCidades, solCorrente);

			printf("\nobjetivo: %d ", objetivo(solCorrente,matrizDistancias,quantidadeCidades));
	    }
		// Clock da CPU
	    Ticks[1] = clock();

	}
	else if(metaheuristica == 2){ //GENECTIC ALGORITHM
		
		Ticks[0] = clock();
		int *solInit1, *solInit2, *solInit3, *solInit4;
		solInit1 = geraSolInicialRandom(quantidadeCidades);
		solInit2 = geraSolInicialRandom(quantidadeCidades);
		solInit3 = geraSolInicialRandom(quantidadeCidades);
		solInit4 = geraSolInicialRandom(quantidadeCidades);

		puts("1");
		for(int i = 0 ; i < quantidadeCidades;i++){
			printf("%d ", solInit1[i]);
		}
		puts("\n2");
		for(int i = 0 ; i < quantidadeCidades;i++){
			printf("%d ", solInit2[i]);
		}
		puts("\n3");
		for(int i = 0 ; i < quantidadeCidades;i++){
			printf("%d ", solInit3[i]);
		}

		puts("\n4");
		for(int i = 0 ; i < quantidadeCidades;i++){
			printf("%d ", solInit4[i]);
		}

		roulette(objetivo(solInit1, matrizDistancias, quantidadeCidades),objetivo(solInit2, matrizDistancias, quantidadeCidades),objetivo(solInit3, matrizDistancias, quantidadeCidades),objetivo(solInit4, matrizDistancias, quantidadeCidades));

		//Clock da CPU
	    Ticks[1] = clock();

	} 
	else 
		printf("Valor indisponivel\n");
	

	// printf("\n\nObjetivo da melhor solução : %d\n", objetivo(solCorrente,matrizDistancias, quantidadeCidades));
	// puts("Melhor Solução: ");
	// for (int i = 0; i < quantidadeCidades;i++){
	//    printf("%d ", solCorrente[i]);
	// }
	
	double Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
	printf("\nTempo gasto: %g ms.\n", Tempo);

	// Fecha o arquivo lido
	fechaArquivo();
	return 0;
}