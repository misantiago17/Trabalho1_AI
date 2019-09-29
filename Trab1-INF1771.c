#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int *geraSolInicial(int **m) {}

int main(int argc, char *argv[]) {
	FILE *f; 
	int **matDist; // Matriz contendo a distancia entre as cidades
	char s[30]; 
	int tam = 0, i, j, cont,n; 
    clock_t Ticks[2];
    
    //***************** LEITURA DA MATRIZ DE DISTANCIA ENTRE AS CIDADES **********************//
    printf("Arquivos:\n1 - brazil58.tsp\n2 - gr24.tsp\n3 - gr120.tsp\n4 - hk48.tsp\n5 - si175.tsp\n");
    scanf("%d", &n);

	if(n == 1)      f = fopen("brazil58.tsp","rt");
    else if(n == 2) f = fopen("gr24.tsp","rt"); 
    else if(n == 3) f = fopen("gr120.tsp","rt"); 
    else if(n == 4) f = fopen("hk48.tsp","rt"); 
    else if(n == 5) f = fopen("si175.tsp","rt");
    else printf("Arquivo não existente");

	if(f == NULL) {
		printf("Erro na leitura do arquivo\n");
		exit(1);
	}
	while(tam == 0) { 
		fscanf(f,"%s",&s);
		if(strcmp(s, "DIMENSION:") == 0) {
			fscanf(f,"%d",&tam);
		}
	}

	matDist = (int **) malloc(tam * sizeof(int**)); 
	for(i = 0; i < tam; i++) {
		matDist[i] = (int*) malloc(tam * sizeof(int));
		for(j = 0; j < tam; j++) {
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
		cont = ((tam * tam)/2) + (tam/2);
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
		cont = ((tam * tam)/2) - (tam/2);
		while(cont > 0) { 
			if(i >= j)
				j++;
			else if(j == tam) {
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
    geraSolInicial(matDist);
    //chamada funcao metaheuristica
    Ticks[1] = clock();
    double Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    printf("Tempo gasto: %g ms.", Tempo);

	fclose(f);
	return 0;
}
