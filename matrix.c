/*
*Author: Matheus Obando
*Status: Ongoing
*Date: 17/10/2019
*Note: Multiplication with threads still not working
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h> // use -lpthread -lrt on terminal to compilate with the pthread.h library
#include<time.h>
#include<semaphore.h>

#define PEQUENA 8
#define MEDIA  40
#define GRANDE 80

int **matrixA, **matrixB, **matrixC;
int divRes, order, nthreads;

sem_t mutex;

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
    // Multiplica:
    //int **result = createMatrix(order);
    int i, j, k,product = 0; 
    for(i = 0; i < order; i++){
        for(j = 0; j < order; j++){
            for(k = 0; k < order; k++){
                product += matrixA[i][k] * matrixB[k][j];
            }
            // Critical section 
            //sem_wait(&mutex);
            matrixC[i][j] = product;
            product = 0;
            //sem_post(&mutex);
            // End of critical section
        }
    }
}

void *multiplyMatricesThread(void *arg){
    // Multiplying: 
    int i, j, k, product = 0, *aux = arg;
    int id = *aux; 
    for(i = (id*divRes); i < (id+1)*divRes; i++){
        for(j = (id*divRes); j < (id+1)*divRes; j++){
            // Critical section
            sem_wait(&mutex);
            for(k = (id*divRes); k < (id+1)*divRes; k++){
                product += matrixA[i][k] * matrixB[k][j];
            } 
            matrixC[i][j] = product;
            product = 0;
            sem_post(&mutex);
            // End of critical section
        }
    }
}

int main(int argc, char const *argv[])
{
    // Initial feedback from command line
    srand(time(NULL));
    printf("TAMANHO DA MATRIZ: %s\n",argv[1]); // matrix 'size' argument position on command line
    nthreads = argv[2][0] - '0';
    printf("QUANTIDADE DE THREADS: %d\n\n",nthreads);

    //Matrix creating section
    order = getMatrixOrder(argv[1]);
    divRes = order/nthreads;
    matrixA = createMatrix(order);
    matrixB = createMatrix(order);
    matrixC = createMatrix(order);

    putMatrixValues(matrixA, order);
    putMatrixValues(matrixB, order);

    printMatrix(matrixA, order);
    printMatrix(matrixB, order);

    if (nthreads == 1){
        multiplyMatrices(matrixA, matrixB, matrixC, order); // index still setted with 0, it will not affect the execution
        printMatrix(matrixC,order);
    }

    else if(nthreads > 1){
        //Threads creating section
    
        pthread_t *threads = (pthread_t*)malloc(nthreads*sizeof(pthread_t));
        int *id = (int*)malloc(nthreads*sizeof(int));

        for(int i = 0; i < nthreads; i++){
            id[i] = i;
            pthread_create(&threads[i], NULL, multiplyMatricesThread, &id[i]);
        }

        for(int i = 0; i < nthreads; i++){
            pthread_join(threads[i], NULL);
        }

        printMatrix(matrixC, order);    
    }

    else{
        printf("Erro, número de threads inválido\n");
    }

    return 0;
}
