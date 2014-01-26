#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "auxiliaryFunctionsOpenMP.h"
#include "choleskyFactorizationOpenMP.h"


void choleskyParallel(int size, double** inputMatrix, double **matrixL) {
	double tempSum, partOfResult;
	int k, i, j;

	for(i=0; i < size; i++) {

		/*obliczanie elementu w wierszu na przekątnej*/
		tempSum = 0;

		#pragma omp parallel for schedule (static) private(k) firstprivate(i)
		for(k = 0; k < i; k++) {
			tempSum += matrixL[k][i] * matrixL[k][i];
		}

		partOfResult = inputMatrix[i][i] - tempSum;
		matrixL[i][i] = sqrt(partOfResult);
		//printf("%d\t",i);

		/*obliczanie elementów poza przekątną*/
		#pragma omp parallel for schedule (static) private(j, k, partOfResult, tempSum) firstprivate(i)
		for(j = i + 1; j < size; j++) {
			tempSum = 0;
			for(k = 0; k < i; k++) {
				tempSum += matrixL[k][i] * matrixL[k][j];
			}

			partOfResult = inputMatrix[i][j] - tempSum;
			matrixL[i][j] = partOfResult / matrixL[i][i];
		}
	}
}

void forwardSolutionPhaseParallel(int size, double **factorizedMatrix, double *b, double *result) {
	int i, j;
	double tempSum;

	for(i = 0; i < size; i++) {

		tempSum = 0;

		#pragma omp parallel for schedule (static) private(j) firstprivate(i)
		for(j = 0; j < i; j++) {
			tempSum += factorizedMatrix[i][j] * result[j];
		}

		result[i] = (b[i] - tempSum)/factorizedMatrix[i][i];
	}
}

void backwardSolutionPhaseParallel(int size, double** factorizedMatrix, double* y, double *result) {
	int i, j;
	double tempSum;


	for(i = size - 1; i >= 0; i--) {

		tempSum = 0;

		#pragma omp parallel for firstprivate(i)  private(j) schedule (static)
		for(j = size - 1; j > i; j--) {
			tempSum += factorizedMatrix[i][j] * result[j];
		}

		result[i] = (y[i] - tempSum)/factorizedMatrix[i][i];

	}
}

void matrixTranspositionParallel(int size, double** inputMatrix, double **transposed) {
	int i,j;

	#pragma omp for schedule (static) private(i, j)
	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			transposed[i][j] = inputMatrix[j][i];
		}
	}
}

double runAlgorithmCholeskyParallel(double **inputMatrix, double *b, int size, double *x) {
	double **factorizedMatrix = allocate2DArray(size, size);
	double **factorizedMatrixTrans = allocate2DArray(size, size);
	double *y = allocade1DArray(size);
	double timeConsumed = 0;

	#pragma omp set dynamic(1);
	time_t start = time(NULL);//POMIAR CZASU START

	choleskyParallel(size, inputMatrix, factorizedMatrix);
    //double **factorizedM = matrixTransposition(size, factorizedMT);
    //printf("Sfaktoryzowana  macierz Choleskiego (U): \n");
    //print2DArray(factorizedMatrix, size, size);


    matrixTranspositionParallel(size, factorizedMatrix, factorizedMatrixTrans);

    printf("Sfaktoryzowana transponowana macierz Choleskiego (U): \n");
    //print2DArray(factorizedMatrixTrans, size, size);


    forwardSolutionPhaseParallel(size, factorizedMatrixTrans, b, y);
    printf("\nWynik po forwardSolutionPhase: \n");
    //print1DArray(y, size);


    backwardSolutionPhaseParallel(size, factorizedMatrix, y, x);
    printf("\nWynik po backwardSolutionPhase: \n");
    //print1DArray(x, size);

    timeConsumed = (double)(time(NULL) - start);//POMIAR CZASU END

    printf("\nSprawdzenie rozwiązania: \n");
    //checkSolution(inputMatrix, x, b, size);


    deallocate2D(factorizedMatrix,size);
    deallocate2D(factorizedMatrixTrans,size);
    free(y);

    return timeConsumed;
}
