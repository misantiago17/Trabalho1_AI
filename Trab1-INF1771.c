#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

// Faz o swap
/*int *bestNeighbour(int **m, int t, int *s){ 

	int aux, *melhor = s;

	for (int i = 0; i < t; i++){
		for (int j = 1; j < t; j++){

			atual = s;

			printf("", atual[])

			aux = melhor[i];
			melhor[i] = melhor[j];
			melhor[j] = aux;
			if(objetivo(melhor,m,t) < objetivo(melhor,m,t))
				melhor = melhor;
		}
	}
	return melhor;
}*/

// essa funcao tem muita repetição - otimizar
int *bestNeighbour(int **m, int t, int *s){ 

	int *vizinhanca = (int *) malloc (t* sizeof(int));
	int *melhorVizinhanca = s;
	int VizinhoTrocado;

	// pega um valor do vetor original
	for (int i=0; i<t; i++){

		VizinhoTrocado = s[i];

		/*printf("Tamanho T: %d\n", t);

		printf("Vetor original mandado:");
		for (int h=0; h < t;h++){
			printf("%d ", s[h]);
		}		
		printf("\n");

		printf("Vizinho a ser trocado: %d \n", VizinhoTrocado);*/

		printf("\n\nValor de i: %d\n\n", i);

		// faz a troca desse valor em cada uma das posicoes do vetor
		for (int j=0; j<t; j++){

			// reinicia o vetor a cada loop de troca 
			for (int h=0; h<t; h++)
				vizinhanca[h] = s[h];

			/*printf("Vetor vizinhanca reiniciado:");
			for (int k=0; k < t;k++){
				printf("%d ", vizinhanca[k]);
			}		
			printf("\n");*/


			// Não faz a troca caso seja igual -> testa o vetor vizinhanca como ele está - tem repetição (otimizar)

			if (j != i){ // Faz a troca
				
				/*printf("Indices:\ni:%d\nj:%d\n\n",i,j);

				printf("i inicial: %d\n", vizinhanca[i]); // tem que ser zero
				printf("j inicial: %d\n", vizinhanca[j]);
				printf("\n");*/

				vizinhanca[i] = vizinhanca[j];
				//printf("Item original que vai pra tras: %d\n", vizinhanca[j]);
				vizinhanca[j] = VizinhoTrocado;

				/*printf("i final: %d\n", vizinhanca[i]);
				printf("j final: %d\n", vizinhanca[j]);	// tem que ser zero
				printf("-------------\n");*/
			}

			/*printf("Vetor vizinhanca feito:");
			for (int p=0; p < t;p++){
				printf("%d ", vizinhanca[p]);
			}		
			printf("\n");*/

			printf("resposta Objetivo da vizinhanca: %d\n",objetivo(vizinhanca,m,t));
			printf("resposta Objetivo melhor vizinhanca: %d\n",objetivo(melhorVizinhanca,m,t));

			if (objetivo(vizinhanca,m,t) < objetivo(melhorVizinhanca,m,t)){
				melhorVizinhanca = vizinhanca;
			}
		}
	}

	free(vizinhanca);

	return melhorVizinhanca;
}

int main(int argc, char *argv[]) {
	FILE *f; 
	int **matDist; // Matriz contendo a distancia entre as cidades
	char s[30]; 
	int qtdCidades = 0, i, j, cont,n; 
    clock_t Ticks[2];
    int *solCorrente;
    //***************** LEITURA DA MATRIZ DE DISTANCIA ENTRE AS CIDADES **********************//
    printf("Arquivos:\n1 - brazil58.tsp\n2 - gr24.tsp\n3 - gr120.tsp\n4 - hk48.tsp\n5 - si175.tsp\n");
    scanf("%d", &n);

	if(n == 1)      f = fopen("brazil58.tsp","rt");
    else if(n == 2) f = fopen("gr24.tsp","rt"); 
    else if(n == 3) f = fopen("gr120.tsp","rt"); 
    else if(n == 4) f = fopen("hk48.tsp","rt"); 
    else if(n == 5) f = fopen("si175.tsp","rt");
    else {
    	puts("Arquivo não existente");
    	exit(1);
    }

    if(f == NULL) {
		printf("Erro na leitura do arquivo\n");
		exit(1);
	}
	while(qtdCidades == 0) { 
		fscanf(f,"%s",&s);
		if(strcmp(s, "DIMENSION:") == 0) {
			fscanf(f,"%d",&qtdCidades);
		}
	}

	matDist = (int **) malloc(qtdCidades * sizeof(int**)); 
	for(i = 0; i < qtdCidades; i++) {
		matDist[i] = (int*) malloc(qtdCidades * sizeof(int));
		for(j = 0; j < qtdCidades; j++) {
			matDist[i][j] = 0;
		}
	}

	i = 0; j = 0;
	while(strcmp(s,"EDGE_WEIGHT_FORMAT:") != 0) 
		fscanf(f,"%s",&s);
    
	fscanf(f,"%s",&s);
	if(strcmp(s,"LOWER_DIAG_ROW") == 0 || strcmp(s,"LOWER_ROW") == 0) { 
		while(strcmp(s,"EDGE_WEIGHT_SECTION") != 0) 
			fscanf(f,"%s",&s);
		cont = ((qtdCidades * qtdCidades)/2) + (qtdCidades/2);
        printf("%d",cont);
		while(cont > 0) { 
			fscanf(f,"%d",&matDist[i][j]);
			matDist[j][i] = matDist[i][j];
			printf("matDist[%d][%d]: %d\n",i,j,matDist[i][j]);
			if(matDist[i][j] == 0) {
				i++;
				j = 0;
			}
			else
				j++;
			cont--;
		}
	}
	if(strcmp(s,"UPPER_DIAG_ROW") == 0 || strcmp(s,"UPPER_ROW") == 0) { 
		while(strcmp(s,"EDGE_WEIGHT_SECTION") != 0) 
			fscanf(f,"%s",&s);
		cont = ((qtdCidades * qtdCidades)/2) - (qtdCidades/2);
		while(cont > 0) { 
			if(i >= j)
				j++;
			else if(j == qtdCidades) {
				i++;
				j = 0;
			}
			else {
				fscanf(f,"%d",&matDist[i][j]);
				matDist[j][i] = matDist[i][j];
				printf("matDist[%d][%d]: %d\n",i,j,matDist[i][j]);
				cont--;
				j++;
			}
		}
	}
    //***************** LEITURA DA MATRIZ DE DISTANCIA ENTRE AS CIDADES **********************//
    Ticks[0] = clock();
    solCorrente = geraSolInicial(qtdCidades);
    solCorrente = bestNeighbour(matDist, qtdCidades, solCorrente);
    //chamada funcao metaheuristica

	// Clock da CPU
    Ticks[1] = clock();
    double Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    printf("Tempo gasto: %g ms.\n", Tempo);

    for (i=0; i < qtdCidades;i++)
    	printf("%d ", solCorrente[i]);

	printf("\n");

	fclose(f);
	return 0;
}
