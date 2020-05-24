#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "Genetico.h"

int randomIndex(int maior, int menor){

    return rand()%(maior-menor+1)+menor;

}

Individuo createPopulation(){   //função responsael por criar cada cromossomo

    Individuo chromossome;
    int i, j, number, flag;

    for(i = 0; i < NUM_CIDADES; i++){
        flag = 0;
        number = randomIndex(NUM_CIDADES, 1);
        chromossome.rota[i] = number;
        
        if(i > 0){
            do{
                flag = 0;
                for(j = 0; j < i; j++){
                    while(chromossome.rota[i] == chromossome.rota[j]){
                        flag = 1;
                        chromossome.rota[i] = randomIndex(NUM_CIDADES, 1);
                    }
                }
            }while(flag == 1);
        }
    }
    chromossome.rota[NUM_CIDADES] = chromossome.rota[0];
    
    for(i = 0; i <= NUM_CIDADES; i++)
        printf("%d | ", chromossome.rota[i]);
    

    return chromossome;
}

void aptidao(Individuo *cromossomo, Cidade *cidades){

    int i, j;
    float soma, xdist, ydist;

    for(i = 0; i < TAM_POPULACAO; i++){
        soma = 0;
        for(j = 0; j < NUM_CIDADES; j++){
            xdist = pow((cidades[(cromossomo[i].rota[j])-1].x - cidades[(cromossomo[i].rota[j+1])-1].x), 2);
            ydist = pow((cidades[(cromossomo[i].rota[j])-1].y - cidades[(cromossomo[i].rota[j+1])-1].y), 2);
            soma += sqrt(xdist+ydist);
        }
        cromossomo[i].aptidao = soma;
        //printf("\nI -> %d | Aptidão = %.2f", i, cromossomo[i].aptidao);
    }
}

Individuo fittest(Individuo *population){   //funcao que retorna o individuo com a melhor aptidao dentre os individuos da populacao

    int i;
    Individuo fit = population[0];

    for(i = 1; i < TAM_POPULACAO; i++){
        if(fit.aptidao > population[i].aptidao)
            fit = population[i];
    }
    //printf("%.3f", fit.aptidao);
    return fit;
}

void selecao(Individuo *population, Individuo *newGen){

    int i, j, flag, number, torneio[TAM_POPULACAO];
    
    for(i = 0; i < TAM_POPULACAO; i++){    //Gera as duplas do torneio
        flag = 0;
        number = randomIndex(TAM_POPULACAO, 1);
        torneio[i] = number;

        if(i > 0){
            do{
                flag = 0;
                for(j = 0; j < i; j++){
                    while(torneio[i] == torneio[j]){
                        flag = 1;
                        torneio[i] = randomIndex(TAM_POPULACAO, 1);
                    }
                }
            }while(flag == 1);
        }
    }
    /*
    printf("\nTorneio Binário:\n");
    for(i = 0; i < TAM_POPULACAO; i++){
        printf("%d, ", torneio[i]);
    }*/

    j = 0;
    for(i = 0; i < TAM_POPULACAO; i+=2){    // Faz o torneio binário das duplas e salva o mais apto no vetor newgen
        if(i+1 <= TAM_POPULACAO){
            if(population[torneio[i]].aptidao < population[torneio[i+1]].aptidao){
                newGen[j] = population[torneio[i]];
                j++;
            }
            else{
                newGen[j] = population[torneio[i+1]];
                j++;
            }
        }    
    }
    /*
    printf("\n-> POOL:\n");
    for(i = 0; i < TAM_POPULACAO/2; i++){
        printf("%.2f, ", newGen[i].aptidao);
    }*/
}

void crossingOver(Individuo *matingPool){

    int crosspoint, i, j, k, flag;
    int child1[NUM_CIDADES], child2[NUM_CIDADES];
    
    for(i = 0; i < TAM_POPULACAO/2; i+=2){
        crosspoint = randomIndex(NUM_CIDADES-3, 3);
        for(j = 0; j < NUM_CIDADES; j++){
            if(j <= crosspoint){
                child1[j] = matingPool[i].rota[j];
                child2[j] = matingPool[i+1].rota[j];
            }
            else{
                child1[j] = matingPool[i+1].rota[j];
                child2[j] = matingPool[i].rota[j];
            }
            
            if(j > 0){      //verificação para evitar repetiçoes no filho 1
                do{
                    flag = 0;
                    for(k = 0; k < j; k++){
                        while(child1[j] == child1[k]){
                            flag = 1;
                            child1[j] = randomIndex(NUM_CIDADES, 1);
                        }
                    }
                }while(flag == 1);
            }

            if(j > 0){      //verificação para evitar repetiçoes no filho 2
                do{
                    flag = 0;
                    for(k = 0; k < j; k++){
                        while(child2[j] == child2[k]){
                            flag = 1;
                            child2[j] = randomIndex(NUM_CIDADES, 1);
                        }
                    }
                }while(flag == 1);
            }
        
            matingPool[i+TAM_POPULACAO/2].rota[j] = child1[j];      //salva o filho 1 no vetor Mating Pool, que é referencia do vetor New Generation
            matingPool[i+1+TAM_POPULACAO/2].rota[j] = child2[j];
        }
        matingPool[i+TAM_POPULACAO/2].rota[NUM_CIDADES] = matingPool[i].rota[0];    //Adiciona na ultima posição, um valor igual ao da primeira
        matingPool[i+1+TAM_POPULACAO/2].rota[NUM_CIDADES] = matingPool[i+1].rota[0];
    }
}

void mutacao(Individuo *vetor){

    float chance;
    int i, pos1, pos2, aux;

    for(i = 0; i < TAM_POPULACAO; i++){
        chance = randomIndex(100, 0);
        chance /= 100;
        //printf("\nchance: %.2f", chance);
        pos1 = randomIndex(NUM_CIDADES-1, 1);
        pos2 = randomIndex(NUM_CIDADES-1, 1);

        if(chance <= TAXA_MUTACAO && pos1 != pos2){     //faz a mutacao caso a chance seja inferior a taxa de mutacao e caso as posições sejam diferentes
            aux = vetor[i].rota[pos1];
            vetor[i].rota[pos1] = vetor[i].rota[pos2];
            vetor[i].rota[pos2] = aux;
            ///printf("\n\nElemento %d foi mutado nas posições %d e %d", i, pos1, pos2);
        }
    }
}

void exibirPopulacao(Individuo *vetor){

    int i, j;

    printf("\nIndivíduos Soluções:\n\n");
    for(i = 0; i < TAM_POPULACAO; i++){
        for(j = 0; j <= NUM_CIDADES; j++){
            printf("%d | ", vetor[i].rota[j]);
        }
        printf("\n");
    }
    printf("\n----------\n");
}
