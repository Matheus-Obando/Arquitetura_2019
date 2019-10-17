/*
*Author: Matheus Obando
*Status: Ongoing
*Date: 17/10/2019
*Note: It still needs a thread implementation logic
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

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

int **multiplyMatrices(int **matrixA, int **matrixB, int order){
    // Multiplica:
    int **result = createMatrix(order);
    int i, j, k,product = 0; 
    for(i = 0; i < order; i++){
        for(j = 0; j < order; j++){
            for(k = 0; k < order; k++){
                product += matrixA[i][k] * matrixB[k][j];
            }
            result[i][j] = product;
            product = 0;
        }
    }
    return result;
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    const int order = 2;
    // Instantiating matrix section
    int **matrixA = createMatrix(order);
    int **matrixB = createMatrix(order);

    putMatrixValues(matrixA, order);
    putMatrixValues(matrixB, order);

    printMatrix(matrixA, order);
    printMatrix(matrixB, order);

    int **matrixC = createMatrix(order);
    matrixC = multiplyMatrices(matrixA,matrixB,order);
    printMatrix(matrixC,order);

    //Threads creating section


    return 0;
}
