#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "auxiliaryFunctions.h"

void cholesky(int size, double** inputMatrix, double** matrixL) {
	double tempSum, partOfResult;
	int k, i, j;

	for(i = 0; i < size; i++) {
		for(j = i; j < size; j++) {
			/*Część wspólna obliczeń: zarówno dla elementów na przekątnej jaki i poza*/
			tempSum = 0;
			for(k = 0; k < i; k++) {
				tempSum += matrixL[k][i] * matrixL[k][j];
			}

			partOfResult = inputMatrix[i][j] - tempSum;

			if(i == j) {/*na przekatnej*/
				matrixL[i][j] = sqrt(partOfResult);
			}
			else {/*(i != j) poza przekątną*/
				matrixL[i][j] = partOfResult / matrixL[i][i];
			}
		}
	}
}

void forwardSolutionPhase(int size, double **factorizedMatrix, double *b, double *result) {
	int i, j;
	double tempSum;

	for(i = 0; i < size; i++) {

		tempSum = 0;
		for(j = 0; j < i; j++) {

			tempSum += factorizedMatrix[i][j] * result[j];

		}

		result[i] = (b[i] - tempSum)/factorizedMatrix[i][i];
	}
}

void backwardSolutionPhase(int size, double** factorizedMatrix, double* y, double *result) {
	int i, j;
	double tempSum;

	for(i = size - 1; i >= 0; i--) {

		tempSum = 0;
		for(j = size - 1; j > i; j--) {
			tempSum += factorizedMatrix[i][j] * result[j];
		}

		result[i] = (y[i] - tempSum)/factorizedMatrix[i][i];

	}
}

void matrixTransposition(int size, double** inputMatrix, double **transposed) {
	int i,j;

	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			transposed[i][j] = inputMatrix[j][i];
		}
	}
}



double runAlgorithmCholesky(double **inputMatrix, double *b, int size) {
	double **factorizedMatrix = allocate2DArray(size, size);
	double **factorizedMatrixTrans = allocate2DArray(size, size);
	double *y = allocade1DArray(size);
	double *x = allocade1DArray(size);
	double timeConsumed = 0;


	time_t start = time(NULL);//POMIAR CZASU START

	cholesky(size, inputMatrix, factorizedMatrix);
    //double **factorizedM = matrixTransposition(size, factorizedMT);
    printf("Sfaktoryzowana  macierz Choleskiego (U): \n");
    print2DArray(factorizedMatrix, size, size);


    matrixTransposition(size, factorizedMatrix, factorizedMatrixTrans);

    printf("Sfaktoryzowana transponowana macierz Choleskiego (U): \n");
    print2DArray(factorizedMatrixTrans, size, size);


    forwardSolutionPhase(size, factorizedMatrixTrans, b, y);
    printf("\nWynik po forwardSolutionPhase: \n");
    print1DArray(y, size);


    backwardSolutionPhase(size, factorizedMatrix, y, x);
    printf("\nWynik po backwardSolutionPhase: \n");
    print1DArray(x, size);

    timeConsumed = (double)(time(NULL) - start);//POMIAR CZASU END

    printf("\nSprawdzenie rozwiązania: \n");
    checkSolution(inputMatrix, x, b, size);


    deallocate2D(factorizedMatrix,size);
    deallocate2D(factorizedMatrixTrans,size);
    free(y);
    free(x);

    return timeConsumed;
}
