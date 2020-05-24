/*
    Autor: Fabrício Henrique da Silva
    Data: 02/05/2020
    Atualização: 24/05/2020
    Algoritmo Genético: Tratamento do problema do Caixeiro Viajante

*/

#include <locale.h>

#include "Genetico.c"

#define MAX_GERACOES 2000

int main()
{
    setlocale(LC_ALL, "");
    srand(time(NULL));

    int i, j, k;
    Individuo population[TAM_POPULACAO], newGeneration[TAM_POPULACAO];
    Cidade cidades[NUM_CIDADES];
    FILE *arqCidades;

    /* Carregando dados */
    arqCidades = fopen("cities.txt", "r");

    if(!arqCidades){ 
        printf("\nErro ao abrir arquivo de cidades.\n");
        return 1;
    }

    printf("Matriz de coordenadas das cidades:\n\n");
    for(i = 0; i < NUM_CIDADES; i++){
        fscanf(arqCidades, "%d %d %d ", &cidades[i].id, &cidades[i].x, &cidades[i].y);
        fgets(cidades[i].name, 30, arqCidades);
        //puts(cidades[i].name);
    }
    
    fclose(arqCidades);
    printf("\n----------");

    // printf
    printf("\nPopulação inicial:\n\n");
    // Gerando individuos/soluções/cromossomos aleatorios da população inicial
    for(i = 0; i < TAM_POPULACAO; i++){
        population[i] = createPopulation();
        printf("\n");
    }
    aptidao(&population[0], &cidades[0]);

    Individuo globalSolution = fittest(&population[0]);
    exibirPopulacao(&population[0]);

    printf("\nA melhor solução é: %f", globalSolution.aptidao);
    
    for(i = 0; i < MAX_GERACOES; i++){
        
        selecao(&population[0], &newGeneration[0]);

        crossingOver(&newGeneration[0]);

        mutacao(&newGeneration[0]);

        for(j = 0; j < TAM_POPULACAO; j++){
            population[j] = newGeneration[j];
        }

        aptidao(&population[0], &cidades[0]);
        
        Individuo bestSolution = fittest(&population[0]);

        if(bestSolution.aptidao < globalSolution.aptidao)
            globalSolution = bestSolution;

        /*
        if(i % 10 == 0){
            exibirPopulacao(&population[0]);
        }*/
        
        printf("\nGeração %d -> Melhor Solução = %f", i, bestSolution.aptidao);
    }

    printf("\n---------------------------------------\n");
    printf("\nA melhor de todas as soluções foi: %f\n", globalSolution.aptidao);
    for(k = 0; k <= NUM_CIDADES; k++)
        printf("%d | ", globalSolution.rota[k]);
    printf("\n");

    printf("\nTrajeto entre as cidades: \n");
    for(k = 0; k <= NUM_CIDADES; k++)
        puts(cidades[globalSolution.rota[k]-1].name);

    printf("\n");

    return 0;
}
