/*
*Author: Matheus Obando
*Status: Finished (can still update)
*Date: 17/10/2019
*Note: Done
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h> // use -lpthread -lrt on terminal to compilate
#include<sys/time.h>

#define PEQUENA 500
#define MEDIA  1000
#define GRANDE 2000

int **matrixA, **matrixB, **matrixC;
int divRes, divMod, order, nthreads;

int getMatrixOrder(char *size){
    if(strcmp(size, "pequena") == 0){
        return PEQUENA;
    }
    else if(strcmp(size, "media") == 0){
        return MEDIA;
    }
    else if(strcmp(size, "grande") == 0){
        return GRANDE;
    }
    else{
        return 0;
    }
}

int **createMatrix(int order){
    int i;
    int **matrix = (int **)malloc(order*sizeof(int*)); // It will alocate the number of rows
    for (i = 0; i < order; i++){
        matrix[i] = (int*)malloc(order*sizeof(int)); // It will alocate the number of columns on each row
    }
    return matrix;
}

void putMatrixValues(int **matrix, int order){
    int i,j;
    for(i = 0; i < order; i++){
        for(j = 0; j < order; j++){
            matrix[i][j] = rand()%10; // It generates pseudo values, so there is a need to seed it
        }
    }
}

void printMatrix(int **matrix, int order){
    int i,j;
    for(i = 0; i < order; i++){
        for(j = 0; j < order; j++){
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void multiplyMatrices(int **matrixA, int **matrixB, int **matrixC, int order){
    int i, j, k,product = 0; 
    for(i = 0; i < order; i++){
        for(j = 0; j < order; j++){
            for(k = 0; k < order; k++){
                product += matrixA[i][k] * matrixB[k][j];
            }
            matrixC[i][j] = product;
            product = 0;
        }
    }
}

void *multiplyMatricesThread(void *arg){ 
    int i, j, k, product = 0, *aux = arg;
    int id = *aux;
    int rest;

    if((id+1) == nthreads){
        rest = divMod;
    }
    else{
        rest = 0;
    }

    for(i = (id*divRes); i < ((id+1)*divRes)+rest; i++){
        for(j = 0; j < order; j++){
            for(k = 0; k < order; k++){
                product += matrixA[i][k] * matrixB[k][j];
            } 
            matrixC[i][j] = product;
            product = 0;
        }
    }
}

int main(int argc, char const *argv[])
{
    struct timeval start, end; // To get the time lapse

    // Initial feedback from command line
    srand(time(NULL));
    printf("TAMANHO DA MATRIZ: %s\n",argv[1]); // matrix 'size' argument position on command line
    nthreads = strtol(argv[2],NULL,10);
    printf("QUANTIDADE DE THREADS: %d\n\n",nthreads);

    //Matrix creating section
    order = getMatrixOrder(argv[1]);
    divRes = order/nthreads;
    divMod = order%nthreads;

    matrixA = createMatrix(order);
    matrixB = createMatrix(order);
    matrixC = createMatrix(order);

    putMatrixValues(matrixA, order);
    putMatrixValues(matrixB, order);

    //Remove the commentary to see the values inside both matrices
    //printMatrix(matrixA, order);
    //printMatrix(matrixB, order);

    if (nthreads == 1){
        gettimeofday(&start, NULL);
        multiplyMatrices(matrixA, matrixB, matrixC, order); //index still setted with 0, it will not affect the execution
        //printMatrix(matrixC,order);
        gettimeofday(&end, NULL);

	    long seconds = (end.tv_sec - start.tv_sec);
	    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
	    printf("Tempo decorrido da multiplicação: %ld segundos e %ld microssegundos\n\n", seconds, micros);
    }

    else if(nthreads > 1){

        //Threads creating section
        pthread_t *threads = (pthread_t*)malloc(nthreads*sizeof(pthread_t));
        int *id = (int*)malloc(nthreads*sizeof(int));

        gettimeofday(&start, NULL);
        for(int i = 0; i < nthreads; i++){
            id[i] = i;
            pthread_create(&threads[i], NULL, multiplyMatricesThread, &id[i]);
        }
        for(int i = 0; i < nthreads; i++){
            pthread_join(threads[i], NULL);
        }
        gettimeofday(&end, NULL);

	    long seconds = (end.tv_sec - start.tv_sec);
	    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
	    printf("Tempo decorrido da multiplicação: %ld segundos e %ld microssegundos\n\n", seconds, micros);
        //printMatrix(matrixC, order); // product matrix

    }
    else{
        printf("Error, numero de threads invalido\n");
    }

    return 0;
}
