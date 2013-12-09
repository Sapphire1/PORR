/*
 * DynamicMemory.h
 *
 *  Created on: Dec 8, 2013
 *      Author: ja
 */

#ifndef DYNAMICMEMORY_H_
#define DYNAMICMEMORY_H_


double *allocade1DArray(int size);
double **allocate2DArray(int numRows, int numCols);
void deallocate2D(double** array, int numRows);

void print2DArray(double **A, int rowsNum,int colsNum);
void print1DArray(double *A, int size);

void checkSolution(double** matrixA, double* matrixX, double* matrixB, int size);

void matrixGeneration(int sizeOfMatrix, int *b, int **A);
#endif /* DYNAMICMEMORY_H_ */
