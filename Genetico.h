#ifndef GENETICO_H
#define GENETICO_H

#define TAM_POPULACAO 64    //por enquanto, funciona apenas para multiplos de 4
#define NUM_CIDADES 29
#define TAXA_MUTACAO 0.05

typedef struct{

    float aptidao;
    int rota[NUM_CIDADES+1];

} Individuo;

typedef struct{

    int id;
    int x;
    int y;
    char name[30];
    
} Cidade;

Individuo createPopulation();
void aptidao(Individuo *cromossomo, Cidade *cidades);
void selecionar(Individuo *vetor, int *selecao);
void crossingOver(Individuo *matingPool);
void exibirPopulacao(Individuo *vetor);
void mutacao(Individuo *vetor);

#endif